/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3ResultSetBase.cpp
// Library:     ETKSQLite3
// Purpose:     Code for database resultset with automatic binding (base class)
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/09/30
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "ETKSQLite3ResultSetBase.h"

ETKSQLite3ResultSetBase::ETKSQLite3ResultSetBase()
    : wxSQLite3ResultSet()
    , m_bBindDone(false)
{
}

ETKSQLite3ResultSetBase::~ETKSQLite3ResultSetBase()
{
}

ETKSQLite3ResultSetBase::ETKSQLite3ResultSetBase(const ETKSQLite3ResultSetBase& _rResultSet)
    : wxSQLite3ResultSet(_rResultSet)
    , m_arrayBindIndex(_rResultSet.m_arrayBindIndex)
    , m_bBindDone(_rResultSet.m_bBindDone)
{
}

ETKSQLite3ResultSetBase::ETKSQLite3ResultSetBase(const wxSQLite3ResultSet & _rResultSet)
    : wxSQLite3ResultSet(_rResultSet)
    , m_bBindDone(false)
{
}

const ETKSQLite3ResultSetBase & ETKSQLite3ResultSetBase::operator=(const ETKSQLite3ResultSetBase & _rResultSet)
{
    wxSQLite3ResultSet::operator=(_rResultSet);
    m_arrayBindIndex = _rResultSet.m_arrayBindIndex;
    m_bBindDone      = _rResultSet.m_bBindDone;

    return *this;
}

const ETKSQLite3ResultSetBase & ETKSQLite3ResultSetBase::operator=(const wxSQLite3ResultSet &_rResultSet)
{
    wxSQLite3ResultSet::operator=(_rResultSet);
    m_bBindDone = false;
    m_arrayBindIndex.clear();

    return *this;
}

bool ETKSQLite3ResultSetBase::NextRow()
{
    bool bRet = wxSQLite3ResultSet::NextRow();
    if (bRet)
    {
        if (!m_bBindDone)
        {
            bRet = InitBindFrom();
        }
        else if (m_arrayBindIndex.size() == 0)
        {
            bRet = false; // If binding ok and bind array is empty, it is an error (strange to call twice on error)
            wxFAIL_MSG(wxT("NextRow() called more than once with binding error"));
        }
        if (bRet)
        {
            BindFrom();
        }
    }
    return bRet;
}

int ETKSQLite3ResultSetBase::FindColumnIndex(const ETKSQLite3Column &_rColumn)
{
    if (_rColumn.HasColumnIndex())
    {
        return _rColumn.GetColumnIndex();
    }
    else
    {
        for (int iIndexColumn=0;iIndexColumn<wxSQLite3ResultSet::GetColumnCount();++iIndexColumn)
        {
            if (   wxSQLite3ResultSet::GetColumnName(iIndexColumn) == _rColumn.GetColumnName()
                && wxSQLite3ResultSet::GetTableName(iIndexColumn) == _rColumn.GetTableName())
            {
                return iIndexColumn;
            }
        }
    }
    throw wxSQLite3Exception(WXSQLITE_ERROR, wxT("Invalid field index"));
}
