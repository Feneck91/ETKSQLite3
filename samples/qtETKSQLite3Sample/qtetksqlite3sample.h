#ifndef QTETKSQLITE3SAMPLE_H
#define QTETKSQLITE3SAMPLE_H

#include <QMainWindow>
#include <QSettings>
#include "ui_qtetksqlite3sample.h"
#include "Database.h"

class CETKSQlite3SampleDatabase;

class qtETKSQLite3Sample : public QMainWindow
{
    Q_OBJECT

    QSettings m_settings;
    bool m_bDontUpdate;
private:
    Ui::qtETKSQLite3SampleClass ui;
    /**
     * Database instance.
     */
    CETKSQlite3SampleDatabase *m_pDatabase;

public:
    qtETKSQLite3Sample(QWidget *parent = 0, Qt::WindowFlags flags = Qt::WindowFlags());
    ~qtETKSQLite3Sample();
    void InsertItem(const wxETKSQLite3ResultSet<CRecordTCustomer> &_rResultSetTCustomer,long _lRowCount,double _dSumPrices);

    void UpdateGUI();
    void UpdateTCustomers();
    void UpdateTCustomers2();

private slots:
    void OnOpenDatabase();
    void OnCloseDatabase();
    void OnCreateDatabase();
    void OnItemChanged(QTableWidgetItem *pItem);
};

#endif // QTETKSQLITE3SAMPLE_H
