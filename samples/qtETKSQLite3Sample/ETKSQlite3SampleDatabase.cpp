#include "ETKSQlite3SampleDatabase.h"
#include "Database.h"

CETKSQlite3SampleDatabase::CETKSQlite3SampleDatabase()
    : wxETKSQLite3Database()
{
}

CETKSQlite3SampleDatabase::~CETKSQlite3SampleDatabase()
{
}

wxString CETKSQlite3SampleDatabase::GetSQLDatabaseSchema() const
{
    return   CRecordTGeneral::GetSQLTableConstruction()
           + CRecordTCustomer::GetSQLTableConstruction()
           + CRecordTItem::GetSQLTableConstruction()
           + CRecordTPurchase::GetSQLTableConstruction()
           + QString("INSERT INTO \"TGeneral\" (\"Version\",\"CreationDate\") VALUES (\"%1\", \"%2\");").arg(1).arg(QDateTime::currentDateTime().toString(Qt::ISODate));
}

void CETKSQlite3SampleDatabase::ExecuteConfigAfterOpenOrCreate() throw(wxSQLite3Exception)
{
    wxASSERT_MSG(IsOpened(),wxT("Database is not opened!"));
    if (IsOpened())
    {   // Activate synchronous = OFF or other configuration
        //GetDatabase()->ExecuteUpdate("PRAGMA cache_size=500000;");
        //GetDatabase()->ExecuteUpdate("PRAGMA synchronous=OFF");
        //GetDatabase()->ExecuteUpdate("PRAGMA count_changes=OFF");
        //GetDatabase()->ExecuteUpdate("PRAGMA temp_store=MEMORY");
    }
    wxETKSQLite3Database::ExecuteConfigAfterOpenOrCreate();
}