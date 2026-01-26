/////////////////////////////////////////////////////////////////////////////
/**
 * @file ETKSQLite3Record.cpp
 * @brief Implementation file for record management (table / request).
 *
 * Code for database table/request managment.
 *
 * @author Stéphane Château du header
 * @date Created: 2011/08/05
 * @date Modified: 2025/09/01
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#include "ETKSQLite3Record.h"
#include "ETKSQLite3Database.h"
#include "ETKSQLite3RequestInserter.h"

ETKSQLite3Record::ETKSQLite3Record()
{
}

ETKSQLite3Record::ETKSQLite3Record(etkString _strTableName)
    : m_strTableName(_strTableName)
{
}

ETKSQLite3Record::~ETKSQLite3Record()
{
}

ETKSQLite3Column ETKSQLite3Record::GetAllColumn(etkString _strTableName)
{
    return ETKSQLite3Column(_strTableName,_T("*"));
}

etkString ETKSQLite3Record::GetTableName() const
{
    return m_strTableName;
}

void ETKSQLite3Record::AddColumn(const ETKSQLite3Column &_rColumn)
{
    m_lstColumns.push_back(_rColumn);
}

const ETKSQLite3Record & ETKSQLite3Record::ChangeTableName(etkString _strTableName)
{
    m_strTableName = _strTableName;
    for (tdListColumns::iterator it=m_lstColumns.begin();it!=m_lstColumns.end();++it)
    {
        (*it).SetTableName(_strTableName);
    }

    return *this;
}

ETKSQLite3Expression ETKSQLite3Record::As(etkString _strAliasTableName) const
{
    ETKSQLite3Expression expression;

    for (tdListColumns::const_iterator it=m_lstColumns.begin();it!=m_lstColumns.end();++it)
    {   // Add all columns of record into this
        expression << ETKSQLite3Column(_strAliasTableName, (*it).GetColumnName());
    }

    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationAsSelectFrom, expression, ETKSQLite3Expression(ETKSQLite3Expression::eOperationAs, ETKSQLite3Value(GetTableName(), ETKSQLite3Value::eExpressionTypeValue), ETKSQLite3Value(_strAliasTableName, ETKSQLite3Value::eExpressionTypeValue)));
}

void ETKSQLite3Record::ChangeAs(etkString _strTableName)
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
    wxASSERT_MSG(_nIndex>=0 && _nIndex<m_lstColumns.size(), wxT("Index out of bound"));
    return *(m_lstColumns.begin() + _nIndex);
}

const ETKSQLite3Column * ETKSQLite3Record::FindColumnByName(etkString _strColumnName) const
{
    for (tdListColumns::const_iterator it=m_lstColumns.begin();it!=m_lstColumns.end();++it)
    {
        if ((*it).GetColumnName() == _strColumnName)
        {
            return &(*it);
        }
    }

    return nullptr;
}

const ETKSQLite3Column * ETKSQLite3Record::FindColumnByFullName(etkString _strColumnFullName) const
{
    for (tdListColumns::const_iterator it=m_lstColumns.begin();it!=m_lstColumns.end();++it)
    {
        if ((*it).GetFullName() == _strColumnFullName)
        {
            return &(*it);
        }
    }

    return nullptr;
}

const ETKSQLite3Column & ETKSQLite3Record::operator[](const ETKSQLite3Column &_rColumn) const
{
    etkString strColumnName(_rColumn.GetFullName());
    for (tdListColumns::const_iterator it=m_lstColumns.begin();it!=m_lstColumns.end();++it)
    {
        if ((*it).GetFullName() == strColumnName)
        {
            return (*it);
        }
    }

    etkString strError = wxT("ETKSQLite3Record::operator[] is called on bad static column instance, the column cannot be found!");
    wxLogFatalError(strError);
    wxFAIL_MSG(strError);
    throw wxSQLite3Exception(-1,etkString(_T("Error while operator[] is called! Check your source code!"))); // throw exception !
}

const ETKSQLite3ColumnAttributes & ETKSQLite3Record::GetColumnAttributes(int _Index) const
{
    return GetColumn(_Index).GetColumnAttributes();
}
