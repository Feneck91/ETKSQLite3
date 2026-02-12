#include "qtetksqlite3sample.h"
#include "ETKSQlite3SampleDatabase.h"
#include <QFileDialog>
#include <QElapsedTimer>
#include <fstream>
#include <iomanip>

qtETKSQLite3Sample::qtETKSQLite3Sample(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
    , m_settings("qtETKSQLite3","qtETKSQLite3Sample")
    , m_bDontUpdate(false)
{
    ui.setupUi(this);

    wxConfigBase::Set(&m_settings);
    m_pDatabase = new CETKSQlite3SampleDatabase();

    if (m_pDatabase->GetDatabaseFilePath().isEmpty())
    {
        if (m_pDatabase->Open())
        {
            UpdateTCustomers();
        }
    }

    UpdateGUI();
}

qtETKSQLite3Sample::~qtETKSQLite3Sample()
{
    delete m_pDatabase;
}

void qtETKSQLite3Sample::UpdateGUI()
{
    ui.m_pLabelDatabaseName->setText(m_pDatabase->IsOpened() ? m_pDatabase->GetDatabaseFilePath() : QString("No database opened!"));
    ui.m_pbtnOpen->setEnabled(!m_pDatabase->IsOpened());
    ui.m_pbtnClose->setEnabled(m_pDatabase->IsOpened());
    ui.m_pbtnCreate->setEnabled(!m_pDatabase->IsOpened());
}

void qtETKSQLite3Sample::UpdateTCustomers()
{
    while (ui.m_pTableCustomer->rowCount() > 0)
    {
        ui.m_pTableCustomer->removeRow(0);
    }

    if (m_pDatabase->IsOpened())
    {
        // Prepare request
        CRecordTCustomer recordCustomer;
        long lRowCount;
        double dSumPrices;
        CRecordTItem recordItem;

        // Selector
        ETKSQLite3RequestSelector selector = m_pDatabase->GetSelector();
        ETKSQLite3ResultSet<CRecordTCustomer> resultSetTCustomer;

        // Create the criterion that count the number of purchase for this customer, it is also a SELECT too.
        ETKSQLite3Criterion countPurchase(ETKSQLite3Criterion::eRequestTypeSelect);
        countPurchase<<dbCount();
        countPurchase.SetWhere(CRecordTCustomer::COLUMN_FIELD_id == CRecordTPurchase::COLUMN_FIELD_idCustomer);
        // Set SetFrom is mandatory else in "automatic" mode it takes the 2 tables TCustomer && TPurchase: the computed result is not correct!
        //
        // 3 differents way to set the FROM:
        // 1> Directly with string format. Returns by the static member CRecordTPurchase::TABLE_NAME
        // countPurchase.SetFrom(CRecordTPurchase::TABLE_NAME);
        // 2> Get the tables columns by asking ETKSQLite3Record::GetAllColumn(x) to create 'TABLE.*' only the table name is used
        // countPurchase.SetFrom(ETKSQLite3Record::GetAllColumn(CRecordTPurchase::TABLE_NAME));
        // 3> Get one table column, don't care about the column name, only the table is important so give one of the table column
        countPurchase.SetFrom(CRecordTPurchase::COLUMN_FIELD_id);

        // Create the criterion that count the sum of all items the customer ha bought, it is also a SELECT too.
        ETKSQLite3Criterion sumPrice(ETKSQLite3Criterion::eRequestTypeSelect);
        sumPrice<<dbSum(CRecordTItem::COLUMN_FIELD_Price);
        sumPrice.SetWhere(CRecordTPurchase::COLUMN_FIELD_idItem == CRecordTItem::COLUMN_FIELD_id && CRecordTPurchase::COLUMN_FIELD_idCustomer == CRecordTCustomer::COLUMN_FIELD_id);
        // Set SetFrom is mandatory else in "automatic" mode it takes the 3 tables TCustomer && TPurchase && TItem: the computed result is not correct!
        //
        // 3 differents way to set the FROM
        // 1> Directly with string format. But must cast to wxString else the expression cannot be created from a QString
        // sumPrice.SetFrom(wxString(QString("%1,%2").arg(CRecordTItem::TABLE_NAME).arg(CRecordTPurchase::TABLE_NAME)));
        // 2> Get 2 tables columns, one for each. To get one ask ETKSQLite3Record::GetAllColumn(x) to create 'TABLE.*' only the table name is used
        // sumPrice.SetFrom(ETKSQLite3Record::GetAllColumn(CRecordTItem::TABLE_NAME) + ETKSQLite3Record::GetAllColumn(CRecordTPurchase::TABLE_NAME));
        // 3> Get 2 tables columns, don't care about the column name, only the table is important so give one of the table column
        sumPrice.SetFrom(CRecordTItem::COLUMN_FIELD_id + CRecordTPurchase::COLUMN_FIELD_id);

        selector << CRecordTCustomer()<< dbAs(countPurchase ,"NbPurchase") << dbAs(sumPrice ,"SumPrice");
        /* SAME AS :
        selector << dbSQL("  TCustomer.*,"
                          "    ((SELECT COUNT (*) FROM TPurchase WHERE (TCustomer.id = TPurchase.idCustomer))) AS 'NbPurchase',"
                          "    ((SELECT SUM (TItem.Price) FROM TItem,TPurchase WHERE ((TPurchase.idItem = TItem.id AND TPurchase.idCustomer = TCustomer.id)))) AS 'SumPrice'");
        selector.GetCriterionRequest().SetFrom("TCustomer");
        */

        resultSetTCustomer = selector.ExecuteQuery();

        // resultSetTCustomer.AddBind(x,ETKSQLite3ValueBind(&lRowCount)); <-- Works without the dbAs instruction, but should know the x index
        resultSetTCustomer.AddBind("NbPurchase",ETKSQLite3ValueBind(&lRowCount)); // <-- Simplest way is to bind
        resultSetTCustomer.AddBind("SumPrice",ETKSQLite3ValueBind(&dSumPrices));  //     the result with the columns name

        while(resultSetTCustomer.NextRow())
        {
            InsertItem(resultSetTCustomer,lRowCount,dSumPrices);
        }

        etkString strToto("gfdsgdfs");

    }
}

void qtETKSQLite3Sample::UpdateTCustomers2()
{
    while (ui.m_pTableCustomer->rowCount() > 0)
    {
        ui.m_pTableCustomer->removeRow(0);
    }

    if (m_pDatabase->IsOpened())
    {
        // Prepare request
        CRecordTCustomer recordCustomer;
        long lRowCount;
        double dSumPrices;
        CRecordTItem recordItem;

        ETKSQLite3ResultSet<CRecordTCustomer>   resultSetTCustomer;
        ETKSQLite3ResultSet<ETKSQLite3Record> resultNbItemsAndSum;     // Empty resultset, does contains nothing
        ETKSQLite3RequestSelector selector = m_pDatabase->GetSelector();
        ETKSQLite3RequestSelector selectorNbItems = m_pDatabase->GetSelector();

        selector << CRecordTCustomer();
        selectorNbItems << dbCount(CRecordTPurchase::COLUMN_FIELD_id) // SELECT COUNT(TPurchase.*)
                        << dbSum(CRecordTItem::COLUMN_FIELD_Price);
        selectorNbItems.Where(   CRecordTPurchase::COLUMN_FIELD_idCustomer == *resultSetTCustomer[CRecordTCustomer::COLUMN_FIELD_id]
                              && CRecordTPurchase::COLUMN_FIELD_idItem == CRecordTItem::COLUMN_FIELD_id
                             );

        resultSetTCustomer = selector.ExecuteQuery();
        resultNbItemsAndSum.AddBind(0,ETKSQLite3ValueBind(&lRowCount));
        resultNbItemsAndSum.AddBind(1,ETKSQLite3ValueBind(&dSumPrices));

        while(resultSetTCustomer.NextRow())
        {
            resultNbItemsAndSum = selectorNbItems.ExecuteQuery();
            resultNbItemsAndSum.NextRow();
            InsertItem(resultSetTCustomer,lRowCount,dSumPrices);
        }
    }
}

void qtETKSQLite3Sample::OnOpenDatabase()
{
    if (!m_pDatabase->IsOpened())
    {
        QString strFilenameToOpen = QFileDialog::getOpenFileName(this, tr("Open database..."),QDir::currentPath(), tr("Database (*.db3 *sqlite3);;All Files (*)"));
        if (!strFilenameToOpen.isEmpty())
        {
            m_pDatabase->Open(strFilenameToOpen);
            UpdateGUI();
            UpdateTCustomers();
        }
    }
}

void qtETKSQLite3Sample::OnCloseDatabase()
{
    if (m_pDatabase->IsOpened())
    {
        m_pDatabase->Close();
        UpdateTCustomers();
        UpdateGUI();
    }
}

void qtETKSQLite3Sample::OnCreateDatabase()
{
    QString strFilename = QFileDialog::getSaveFileName(this,tr("Create Database"),QDir::currentPath(),tr("Database (*.db3 *sqlite3);;All Files (*)") );
    if (!strFilename.isNull())
    {
        m_pDatabase->Create(strFilename);

        CRecordTCustomer recordCustomer;
        CRecordTItem     recordItem;
        CRecordTPurchase recordPurchase;

        ETKSQLite3RequestInserter inserterCustomer = m_pDatabase->GetInserter();
        ETKSQLite3RequestInserter inserterItems = m_pDatabase->GetInserter();
        ETKSQLite3RequestInserter inserterPurchase = m_pDatabase->GetInserter();

        //inserter<<recordCustomer[CRecordTCustomer::COLUMN_FIELD_Name]
        //        <<recordCustomer[CRecordTCustomer::COLUMN_FIELD_FirstName]
        //        <<recordCustomer[CRecordTCustomer::COLUMN_FIELD_Sexe];
        inserterCustomer<<recordCustomer;   // Customer table
        inserterItems<<recordItem;          // Items table
        inserterPurchase<<recordPurchase;   // Purchase table

        try
        {
            // You can add lot of elements with only one transaction, else it's take a while for lot of rows!
            wxSQLite3Transaction transaction(m_pDatabase->GetDatabase());

            // Item : TV
            recordItem.SetName("TV");
            recordItem.SetPrice(839.99);
            inserterItems.ExecuteWithoutTransaction();
            qint64 qi64RowIDTV = inserterItems.GetLastRowId();

            // Item : Camera
            recordItem.SetName("Camera");
            recordItem.SetPrice(229.99);
            inserterItems.ExecuteWithoutTransaction();
            qint64 qi64RowIDCamera = inserterItems.GetLastRowId();

            // Item : MP3
            recordItem.SetName("mp3");
            recordItem.SetPrice(49.99);
            inserterItems.ExecuteWithoutTransaction();
            qint64 qi64RowIDMP3 = inserterItems.GetLastRowId();

            // Item : Computer
            recordItem.SetName("Computer");
            recordItem.SetPrice(499.99);
            inserterItems.ExecuteWithoutTransaction();
            qint64 qi64RowIDComputer = inserterItems.GetLastRowId();
            wxUnusedVar(qi64RowIDComputer);

            // Item : short
            recordItem.SetName("Short");
            recordItem.SetPrice(22.45);
            inserterItems.ExecuteWithoutTransaction();
            qint64 qi64RowIDShort = inserterItems.GetLastRowId();

            // Item : Basket
            recordItem.SetName("Basket");
            recordItem.SetPrice(68.55);
            inserterItems.ExecuteWithoutTransaction();
            qint64 qi64RowIDBasket = inserterItems.GetLastRowId();

            // Item : Ballon
            recordItem.SetName("Ballon");
            recordItem.SetPrice(32.64);
            inserterItems.ExecuteWithoutTransaction();
            qint64 qi64RowIDBallon = inserterItems.GetLastRowId();

            // You can also write -> recordCustomer[CRecordTCustomer::COLUMN_FIELD_Birthday]->SetNull(true);
            // It set this column to NULL (or not if parameter is false)
            // Snader Martin : buy 2 items
            recordCustomer.SetName("Snader");
            recordCustomer.SetFirstName("Martin");
            recordCustomer.SetSexe(eSexeMale);
            recordCustomer.SetBirthday(QDate(1980,2,15));       // 1980/02/15
            recordCustomer.SetBirthdayTime(QTime(13,35,22));    // 13:35:32
            inserterCustomer.ExecuteWithoutTransaction();
            recordPurchase.SetidCustomer(inserterCustomer.GetLastRowId()); // Last inserted element ID
            recordPurchase.SetidItem(qi64RowIDTV);
            inserterPurchase.ExecuteWithoutTransaction();
            recordPurchase.SetidItem(qi64RowIDShort);
            inserterPurchase.ExecuteWithoutTransaction();

            // Bartow David : buy 0 items
            recordCustomer.SetName("Bartow");
            recordCustomer.SetFirstName("David");
            recordCustomer.SetSexe(eSexeMale);
            recordCustomer.SetBirthday(QDate(1982,3,15));       // 1982/03/20
            recordCustomer.SetBirthdayTime(QTime(8,12,14));     // 08:12:14
            inserterCustomer.ExecuteWithoutTransaction();

            // Monis James : buy 4 items
            recordCustomer.SetName("Monis");
            recordCustomer.SetFirstName("James");
            recordCustomer.SetSexe(eSexeMale);
            recordCustomer.SetBirthday(QDate(1978,6,25));       // 1978/06/25
            recordCustomer.SetBirthdayTime(QTime(22,35,2));     // 22:35:02
            inserterCustomer.ExecuteWithoutTransaction();
            recordPurchase.SetidCustomer(inserterCustomer.GetLastRowId()); // Last inserted element ID
            recordPurchase.SetidItem(qi64RowIDMP3);
            inserterPurchase.ExecuteWithoutTransaction();
            recordPurchase.SetidItem(qi64RowIDShort);
            inserterPurchase.ExecuteWithoutTransaction();
            recordPurchase.SetidItem(qi64RowIDBasket);
            inserterPurchase.ExecuteWithoutTransaction();
            recordPurchase.SetidItem(qi64RowIDBallon);
            inserterPurchase.ExecuteWithoutTransaction();

            // Gray Jim: buy 3 items
            recordCustomer.SetName("Gray");
            recordCustomer.SetFirstName("Jim");
            recordCustomer.SetSexe(eSexeMale);
            recordCustomer.SetBirthday(QDate(1978,6,25));   // 1978/06/25
            recordCustomer.SetBirthdayTime(QTime(10,15,0)); // 10:15:00
            inserterCustomer.ExecuteWithoutTransaction();
            recordPurchase.SetidCustomer(inserterCustomer.GetLastRowId()); // Last inserted element ID
            recordPurchase.SetidItem(qi64RowIDTV);
            inserterPurchase.ExecuteWithoutTransaction();
            recordPurchase.SetidItem(qi64RowIDCamera);
            inserterPurchase.ExecuteWithoutTransaction();
            recordPurchase.SetidItem(qi64RowIDMP3);
            inserterPurchase.ExecuteWithoutTransaction();

            // Gray Jim: buy 3 items
            recordCustomer.SetName("Gello");
            recordCustomer.SetFirstName("Sarah");
            recordCustomer.SetSexe(eSexeFemale);
            recordCustomer.SetBirthday(QDate(2001,2,15));    // 2001/02/15
            recordCustomer.SetBirthdayTime(QTime(03,17,0)); // 3:17:0
            inserterCustomer.ExecuteWithoutTransaction();
            recordPurchase.SetidCustomer(inserterCustomer.GetLastRowId()); // Last inserted element ID
            recordPurchase.SetidItem(qi64RowIDShort);
            inserterPurchase.ExecuteWithoutTransaction();
            recordPurchase.SetidItem(qi64RowIDBallon);
            inserterPurchase.ExecuteWithoutTransaction();

            transaction.Commit();
        }
        catch(wxSQLite3Exception &)
        {   // Error executing transation, is automatically rolled back
            // Nothing to log, already done
        }

        UpdateTCustomers();
        UpdateGUI();
    }
}

void qtETKSQLite3Sample::InsertItem(const ETKSQLite3ResultSet<CRecordTCustomer> &_rResultSetTCustomer,long _lRowCount,double _dSumPrices)
{
    m_bDontUpdate = true;
    const ETKSQLite3Column *pColSumPrice = _rResultSetTCustomer.FindColumnByName("SumPrice");

    int iRow = ui.m_pTableCustomer->rowCount();
    ui.m_pTableCustomer->insertRow(iRow);
    ui.m_pTableCustomer->setItem(iRow,0,new QTableWidgetItem(QString("%1").arg(_rResultSetTCustomer.Getid())));
    ui.m_pTableCustomer->setItem(iRow,1,new QTableWidgetItem(_rResultSetTCustomer.GetName()));
    ui.m_pTableCustomer->setItem(iRow,2,new QTableWidgetItem(_rResultSetTCustomer.GetFirstName()));
    ui.m_pTableCustomer->setItem(iRow,3,new QTableWidgetItem(_rResultSetTCustomer.GetSexe() == eSexeMale ? "Male" : "Female"));
    ui.m_pTableCustomer->setItem(iRow,4,new QTableWidgetItem(_rResultSetTCustomer.GetBirthday().toString(Qt::ISODate)));
    ui.m_pTableCustomer->setItem(iRow,5,new QTableWidgetItem(_rResultSetTCustomer.GetBirthdayTime().toString(Qt::ISODate)));
    QString strTotot = _rResultSetTCustomer.GetBirthday().toString(Qt::ISODate);
    ui.m_pTableCustomer->setItem(iRow,6,new QTableWidgetItem(QString("%1").arg(_lRowCount)));
    ui.m_pTableCustomer->setItem(iRow,7,new QTableWidgetItem(QString("%1").arg((*pColSumPrice)->IsNull()
        ? "none"
        : QString("%1").arg(_dSumPrices)
        )));
    // Cols 0 / 6 / 7 cannot be modified
    ui.m_pTableCustomer->item(iRow,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    ui.m_pTableCustomer->item(iRow,6)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    ui.m_pTableCustomer->item(iRow,7)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    m_bDontUpdate = false;
}

void qtETKSQLite3Sample::OnItemChanged(QTableWidgetItem *pItem)
{
    if (!m_bDontUpdate)
    {
        m_bDontUpdate = true;
        ETKSQLite3RequestSelector selector = m_pDatabase->GetSelector();
        ETKSQLite3RequestUpdater updater = m_pDatabase->GetUpdater();

        // ID is mandatory : update only this item
        bool bOk,bUpdate(false);

        int iRow = pItem->row(); // Get the row of the modified item
        wxLongLong llID = ui.m_pTableCustomer->item(iRow,0)->text().toLongLong(&bOk);

        ETKSQLite3ResultSet<CRecordTCustomer> resultSetTCustomer;
        selector<<resultSetTCustomer;
        selector.Where(CRecordTCustomer::COLUMN_FIELD_id == llID);
        resultSetTCustomer = selector.ExecuteQuery();
        resultSetTCustomer.NextRow();

        updater<<resultSetTCustomer[CRecordTCustomer::COLUMN_FIELD_id];
        updater.Where(CRecordTCustomer::COLUMN_FIELD_id == wxLongLong(llID));

        switch(pItem->column())
        {
            case 1: // Name
            {
                if (resultSetTCustomer.GetName() != pItem->text())
                {
                    updater<<resultSetTCustomer[CRecordTCustomer::COLUMN_FIELD_Name];
                    resultSetTCustomer.SetName(pItem->text());
                    bUpdate = true;
                }
                break;
            }
            case 2: // First Name
            {
                if (resultSetTCustomer.GetFirstName() != pItem->text())
                {
                    updater<<resultSetTCustomer[CRecordTCustomer::COLUMN_FIELD_FirstName];
                    resultSetTCustomer.SetFirstName(pItem->text());
                    bUpdate = true;
                }
                break;
            }
            case 3: // Sexe
            {
                updater<<resultSetTCustomer[CRecordTCustomer::COLUMN_FIELD_Sexe];
                if (pItem->text().compare("male",Qt::CaseInsensitive) == 0)
                {
                    if (resultSetTCustomer.GetSexe() != eSexeMale)
                    {
                        resultSetTCustomer.SetSexe(eSexeMale);
                        pItem->setText("Male");
                        bUpdate = true;
                    }
                }
                else if (pItem->text().compare("female",Qt::CaseInsensitive) == 0)
                {
                    if (resultSetTCustomer.GetSexe() != eSexeFemale)
                    {
                        resultSetTCustomer.SetSexe(eSexeFemale);
                        pItem->setText("Female");
                        bUpdate = true;
                    }
                }
                else
                {   // Restaure old value
                    pItem->setText(resultSetTCustomer.GetSexe() == eSexeMale ? "Male" : "Female");
                }
                break;
            }
            case 4: // Birthday Date
            {
                updater<<resultSetTCustomer[CRecordTCustomer::COLUMN_FIELD_Birthday];
                QDate dtComputed = QDate::fromString(pItem->text(),Qt::ISODate);
                if (dtComputed.isValid())
                {
                    if (resultSetTCustomer.GetBirthday() != dtComputed)
                    {
                        resultSetTCustomer.SetBirthday(dtComputed);
                        bUpdate = true;
                    }
                }
                else
                {   // Restaure old value
                    pItem->setText(resultSetTCustomer.GetBirthday().toString(Qt::ISODate));
                }
                break;
            }
            case 5: // Birthday Time
            {
                updater<<resultSetTCustomer[CRecordTCustomer::COLUMN_FIELD_Birthday];
                QTime dtComputed = QTime::fromString(pItem->text(),Qt::ISODate);
                if (dtComputed.isValid())
                {
                    if (resultSetTCustomer.GetBirthdayTime() != dtComputed)
                    {
                        resultSetTCustomer.SetBirthdayTime(dtComputed);
                        bUpdate = true;
                    }
                }
                else
                {   // Restaure old value
                    pItem->setText(resultSetTCustomer.GetBirthdayTime().toString(Qt::ISODate));
                }
                break;
            }
        }
        if (bUpdate)
        {   // Update database
            updater.ExecuteWithTransaction();
        }
        m_bDontUpdate = false;
   }
}
