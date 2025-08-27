/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3Record.cpp
// Library:     ETKSQLite3
// Purpose:     Code for database table/request managment
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/05
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "ETKSQLite3Record.h"
#include "ETKSQLite3Database.h"
#include "ETKSQLite3RequestInserter.h"

ETKSQLite3Record::ETKSQLite3Record()
{
}

ETKSQLite3Record::ETKSQLite3Record(wxString _strTableName)
    : m_strTableName(_strTableName)
{
}

ETKSQLite3Record::~ETKSQLite3Record()
{
}

ETKSQLite3Column ETKSQLite3Record::GetAllColumn(wxString _strTableName)
{
    return ETKSQLite3Column(_strTableName,_T("*"));
}

wxString ETKSQLite3Record::GetTableName() const
{
    return m_strTableName;
}

void ETKSQLite3Record::AddColumn(const ETKSQLite3Column &_rColumn)
{
    m_lstColumns.push_back(_rColumn);
}

const ETKSQLite3Record & ETKSQLite3Record::ChangeTableName(wxString _strTableName)
{
    m_strTableName = _strTableName;
    for (tdListColumns::iterator it=m_lstColumns.begin();it!=m_lstColumns.end();++it)
    {
        (*it).SetTableName(_strTableName);
    }

    return *this;
}

ETKSQLite3Expression ETKSQLite3Record::As(wxString _strTableName) const
{
    ETKSQLite3Expression exprRet;
    for (tdListColumns::const_iterator it=m_lstColumns.begin();it!=m_lstColumns.end();++it)
    {   // Add all columns of record into this
        exprRet << it->As(_strTableName);
    }

    return exprRet;
}

void ETKSQLite3Record::ChangeAs(wxString _strTableName)
{
    m_strTableName = _strTableName;
    for (tdListColumns::iterator it=m_lstColumns.begin();it!=m_lstColumns.end();++it)
    {
        (*it).ChangeAs(_strTableName);
    }
}

const ETKSQLite3Record & ETKSQLite3Record::MergeRecord(const ETKSQLite3Record &_rRecord)
{
    for (tdListColumns::const_iterator it=_rRecord.m_lstColumns.begin();it!=_rRecord.m_lstColumns.end();++it)
    {   // Add all columns of record into this
        m_lstColumns.push_back(*it);
    }

    return *this;
}

size_t ETKSQLite3Record::GetColumnCount() const
{
    return m_lstColumns.size();
}

const ETKSQLite3Column & ETKSQLite3Record::GetColumn(size_t _nIndex) const
{
    wxASSERT_MSG(_nIndex>=0 && _nIndex<m_lstColumns.size(),wxT("Index out of bound"));
    return *(m_lstColumns.begin() + _nIndex);
}

const ETKSQLite3Column * ETKSQLite3Record::FindColumnByName(wxString _strColumnName) const
{
    for (tdListColumns::const_iterator it=m_lstColumns.begin();it!=m_lstColumns.end();++it)
    {
        if ((*it).GetColumnName() == _strColumnName)
        {
            return &(*it);
        }
    }

    return NULL;
}

const ETKSQLite3Column * ETKSQLite3Record::FindColumnByFullName(wxString _strColumnFullName) const
{
    for (tdListColumns::const_iterator it=m_lstColumns.begin();it!=m_lstColumns.end();++it)
    {
        if ((*it).GetFullName() == _strColumnFullName)
        {
            return &(*it);
        }
    }

    return NULL;
}

const ETKSQLite3Column & ETKSQLite3Record::operator[](const ETKSQLite3Column &_rColumn) const
{
    wxString strColumnName(_rColumn.GetFullName());
    for (tdListColumns::const_iterator it=m_lstColumns.begin();it!=m_lstColumns.end();++it)
    {
        if ((*it).GetFullName() == strColumnName)
        {
            return (*it);
        }
    }

    wxString strError = wxT("ETKSQLite3Record::operator[] is called on bad static column instance, the column cannot be found!");
    wxLogFatalError(strError);
    wxFAIL_MSG(strError);
    throw wxSQLite3Exception(-1,wxString(_T("Error while operator[] is called! Check your source code!"))); // throw exception !
}

const ETKSQLite3ColumnAttributes & ETKSQLite3Record::GetColumnAttributes(int _Index) const
{
    return GetColumn(_Index).GetColumnAttributes();
}
