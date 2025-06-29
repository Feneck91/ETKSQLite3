/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3Record.cpp
// Library:     wxETKSQLite3
// Purpose:     Code for database table/request managment
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/05
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "wxETKSQLite3Record.h"
#include "wxETKSQLite3Database.h"
#include "wxETKSQLite3RequestInserter.h"

wxETKSQLite3Record::wxETKSQLite3Record()
{
}

wxETKSQLite3Record::wxETKSQLite3Record(wxString _strTableName)
    : m_strTableName(_strTableName)
{
}

wxETKSQLite3Record::~wxETKSQLite3Record()
{
}

wxETKSQLite3Column wxETKSQLite3Record::GetAllColumn(wxString _strTableName)
{
    return wxETKSQLite3Column(_strTableName,_T("*"));
}

wxString wxETKSQLite3Record::GetTableName() const
{
    return m_strTableName;
}

void wxETKSQLite3Record::AddColumn(const wxETKSQLite3Column &_rColumn)
{
    m_lstColumns.push_back(_rColumn);
}

const wxETKSQLite3Record & wxETKSQLite3Record::ChangeTableName(wxString _strTableName)
{
    m_strTableName = _strTableName;
    for (tdListColumns::iterator it=m_lstColumns.begin();it!=m_lstColumns.end();++it)
    {
        (*it).SetTableName(_strTableName);
    }

    return *this;
}

wxETKSQLite3Expression wxETKSQLite3Record::As(wxString _strTableName) const
{
    wxETKSQLite3Expression exprRet;
    for (tdListColumns::const_iterator it=m_lstColumns.begin();it!=m_lstColumns.end();++it)
    {   // Add all columns of record into this
        exprRet << it->As(_strTableName);
    }

    return exprRet;
}

void wxETKSQLite3Record::ChangeAs(wxString _strTableName)
{
    m_strTableName = _strTableName;
    for (tdListColumns::iterator it=m_lstColumns.begin();it!=m_lstColumns.end();++it)
    {
        (*it).ChangeAs(_strTableName);
    }
}

const wxETKSQLite3Record & wxETKSQLite3Record::MergeRecord(const wxETKSQLite3Record &_rRecord)
{
    for (tdListColumns::const_iterator it=_rRecord.m_lstColumns.begin();it!=_rRecord.m_lstColumns.end();++it)
    {   // Add all columns of record into this
        m_lstColumns.push_back(*it);
    }

    return *this;
}

size_t wxETKSQLite3Record::GetColumnCount() const
{
    return m_lstColumns.size();
}

const wxETKSQLite3Column & wxETKSQLite3Record::GetColumn(size_t _nIndex) const
{
    wxASSERT_MSG(_nIndex>=0 && _nIndex<m_lstColumns.size(),wxT("Index out of bound"));
    return *(m_lstColumns.begin() + _nIndex);
}

const wxETKSQLite3Column * wxETKSQLite3Record::FindColumnByName(wxString _strColumnName) const
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

const wxETKSQLite3Column * wxETKSQLite3Record::FindColumnByFullName(wxString _strColumnFullName) const
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

const wxETKSQLite3Column & wxETKSQLite3Record::operator[](const wxETKSQLite3Column &_rColumn) const throw(wxSQLite3Exception)
{
    wxString strColumnName(_rColumn.GetFullName());
    for (tdListColumns::const_iterator it=m_lstColumns.begin();it!=m_lstColumns.end();++it)
    {
        if ((*it).GetFullName() == strColumnName)
        {
            return (*it);
        }
    }

    wxString strError = wxT("wxETKSQLite3Record::operator[] is called on bad static column instance, the column cannot be found!");
    wxLogFatalError(strError);
    wxFAIL_MSG(strError);
    throw wxSQLite3Exception(-1,wxString(_T("Error while operator[] is called! Check your source code!"))); // throw exception !
}

const wxETKSQLite3ColumnAttributes & wxETKSQLite3Record::GetColumnAttributes(int _Index) const
{
    return GetColumn(_Index).GetColumnAttributes();
}
