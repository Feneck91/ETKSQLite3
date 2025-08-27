/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3Column.h
// Library:     ETKSQLite3
// Purpose:     Code for column managment
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/04
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "ETKSQLite3Column.h"
#include "ETKSQLite3ValueBind.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                      ETKSQLite3ColumnAttributes                                       //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
ETKSQLite3ColumnAttributes::ETKSQLite3ColumnAttributes()
    : m_bAddForInsertRequest(true)
{
}

ETKSQLite3ColumnAttributes::ETKSQLite3ColumnAttributes(const ETKSQLite3ValueBind & _rBindValue,bool _bAddForInsertRequest)
    : m_bAddForInsertRequest(_bAddForInsertRequest)
    , m_pBindValue(_rBindValue.Clone())
{
}

ETKSQLite3ColumnAttributes::ETKSQLite3ColumnAttributes(const ETKSQLite3ColumnAttributes &_rColumnAttributes)
{
    operator=(_rColumnAttributes);
}

ETKSQLite3ColumnAttributes::~ETKSQLite3ColumnAttributes()
{
    // Nothing to do to delete m_pBindValue
}

void ETKSQLite3ColumnAttributes::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bForInsertRequest) const
{
    wxASSERT_MSG(m_pBindValue.IsNotNull(),wxT("BindTo on null bind value (attempt to bind on a static column member ?)"));
    if (m_pBindValue.IsNotNull())
    {
        if ((!m_bAddForInsertRequest && _bForInsertRequest) || IsNull())
        {   // Bind null if this field should not be add for insert request else it doesn't work
            // It is probably a primary key with integer autoincrement field
            // Bind null if this field is mark as NULL field, ignore binded data and set it to null
            _rstmt.BindNull(_iIndex);
        }
        else
        {
            m_pBindValue->BindTo(_rstmt,_iIndex,m_bAddForInsertRequest,_bForInsertRequest);
        }
    }
}

void ETKSQLite3ColumnAttributes::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
{
    wxASSERT_MSG(m_pBindValue.IsNotNull(),wxT("BindFrom on null bind value (attempt to bind on a static column member ?)"));
    if (m_pBindValue.IsNotNull())
    {
        if (_rResultSet.IsNull(_iIndex))
        {
            SetNull();
        }
        else
        {
            SetNull(false); // This field is not null
            m_pBindValue->BindFrom(_rResultSet,_iIndex);
        }
    }
}

bool ETKSQLite3ColumnAttributes::IsNull() const
{
    wxASSERT_MSG(m_pBindValue.IsNotNull(),wxT("IsNull on null bind value (attempt to call on a static column member ?)"));
    if (m_pBindValue.IsNotNull())
    {
        return m_pBindValue->IsNull();
    }
    return false;
}

void ETKSQLite3ColumnAttributes::SetNull(bool _bIsNull) const
{
    wxASSERT_MSG(m_pBindValue.IsNotNull(),wxT("SetNull on null bind value (attempt to call on a static column member ?)"));
    if (m_pBindValue.IsNotNull())
    {
        return m_pBindValue->SetNull(_bIsNull);
    }
}

const ETKSQLite3ColumnAttributes & ETKSQLite3ColumnAttributes::operator=(const ETKSQLite3ColumnAttributes &_rColumnAttributes)
{
    // Let pointer on same element, use shared pointer to know when delete it
    m_pBindValue = _rColumnAttributes.m_pBindValue;
    m_bAddForInsertRequest  = _rColumnAttributes.m_bAddForInsertRequest;

    return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                            ETKSQLite3Column                                           //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

ETKSQLite3Column::ETKSQLite3Column(int _iColumnIndex)
    : m_strTableName()
    , m_strColumnName()
    , m_ColumnAttributes()
    , m_iColumnIndex(_iColumnIndex)
{
}

ETKSQLite3Column::ETKSQLite3Column(wxString _strTableName,wxString _strColumnName)
    : m_strTableName(_strTableName)
    , m_strColumnName(_strColumnName)
    , m_ColumnAttributes()
    , m_iColumnIndex(-1)
{
}

ETKSQLite3Column::ETKSQLite3Column(const ETKSQLite3Column &_rColumn,const ETKSQLite3ColumnAttributes &_rColumnAttributes)
    : m_strTableName(_rColumn.GetTableName())
    , m_strColumnName(_rColumn.GetColumnName())
    , m_ColumnAttributes(_rColumnAttributes)
    , m_iColumnIndex(_rColumn.GetColumnIndex())
{
}

ETKSQLite3Column::ETKSQLite3Column(const ETKSQLite3Column &_rColumn)
{
    operator=(_rColumn);
}

ETKSQLite3Column::~ETKSQLite3Column()
{
}

int ETKSQLite3Column::GetColumnIndex() const
{
    return m_iColumnIndex;
}

bool ETKSQLite3Column::HasColumnIndex() const
{
    return GetColumnIndex() != -1;
}

wxString ETKSQLite3Column::GetTableName() const
{
    return m_strTableName;
}

void ETKSQLite3Column::SetTableName(wxString _strTableName)
{
    m_strTableName = _strTableName;
}

wxString ETKSQLite3Column::GetColumnName() const
{
    return m_strColumnName;
}

wxString ETKSQLite3Column::GetFullName() const
{
    return wxString::Format(_T("%s.%s"),GetTableName().wx_str(),GetColumnName().wx_str());
}

void ETKSQLite3Column::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bForInsertRequest) const
{
    m_ColumnAttributes.BindTo(_rstmt,_iIndex,_bForInsertRequest);
}

void ETKSQLite3Column::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
{
    m_ColumnAttributes.BindFrom(_rResultSet,_iIndex);
}

const ETKSQLite3ColumnAttributes & ETKSQLite3Column::GetColumnAttributes() const
{
    return m_ColumnAttributes;
}

const ETKSQLite3ColumnAttributes & ETKSQLite3Column::operator*() const
{
    return m_ColumnAttributes;
}

const ETKSQLite3ColumnAttributes * ETKSQLite3Column::operator->() const
{
    return &m_ColumnAttributes;
}

#ifdef __WXDEBUG__
bool ETKSQLite3Column::IsBindedValueExists() const
{
    return m_ColumnAttributes.IsBindedValueExists();
}
#endif

const ETKSQLite3Column & ETKSQLite3Column::operator=(const ETKSQLite3Column &_rColumn)
{
    m_strTableName      = _rColumn.m_strTableName;
    m_strColumnName     = _rColumn.m_strColumnName;
    m_ColumnAttributes  = _rColumn.m_ColumnAttributes;
    m_iColumnIndex      = _rColumn.m_iColumnIndex;

    return *this;
}

//-------------------------------
// Column expression operators
ETKSQLite3Expression ETKSQLite3Column::operator==(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationEqual,
                                  ETKSQLite3Expression(*this),
                                  _rExpression
                                 );
}

ETKSQLite3Expression ETKSQLite3Column::operator==(const ETKSQLite3Column &_rColumn) const
{
    // Join between columns
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationEqual,
                                  ETKSQLite3Expression(*this),
                                  ETKSQLite3Expression(_rColumn)
                                 );
}

ETKSQLite3Expression ETKSQLite3Column::Join(const ETKSQLite3Column &_rColumn) const
{
    // Join between columns
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationJoin,
                                  ETKSQLite3Expression(*this),
                                  ETKSQLite3Expression(_rColumn)
                                 );
}

ETKSQLite3Expression ETKSQLite3Column::LeftJoin(const ETKSQLite3Column &_rColumn) const
{
    // Left join between columns
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationLeftJoin,
                                  ETKSQLite3Expression(*this),
                                  ETKSQLite3Expression(_rColumn)
                                 );
}

ETKSQLite3Expression ETKSQLite3Column::InnerJoin(const ETKSQLite3Column &_rColumn) const
{
    // Inner join between columns
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationInnerJoin,
                                  ETKSQLite3Expression(*this),
                                  ETKSQLite3Expression(_rColumn)
                                 );
}

// NOT SUPPORTED
//ETKSQLite3Expression ETKSQLite3Column::RightJoin(const ETKSQLite3Column &_rColumn) const
//{
//    // Left join between columns
//    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationRightJoin,
//                                  ETKSQLite3Expression(*this),
//                                  ETKSQLite3Expression(_rColumn)
//                                 );
//}
//
//ETKSQLite3Expression ETKSQLite3Column::FullJoin(const ETKSQLite3Column &_rColumn) const
//{
//    // Left join between columns
//    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationFullJoin,
//                                  ETKSQLite3Expression(*this),
//                                  ETKSQLite3Expression(_rColumn)
//                                 );
//}

ETKSQLite3Expression ETKSQLite3Column::In(const ETKSQLite3RequestSelector &_rSelector) const
{
    return ETKSQLite3Expression(*this).In(ETKSQLite3Expression(_rSelector));
}

ETKSQLite3Expression ETKSQLite3Column::In(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(*this).In(ETKSQLite3Expression(_rExpression));
}


ETKSQLite3Expression ETKSQLite3Column::Like(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationLike,
                                  ETKSQLite3Expression(*this),
                                  _rExpression
                                 );
}

ETKSQLite3Expression ETKSQLite3Column::As(wxString _strTableName,wxString _strAsName) const
{
    ETKSQLite3Column column(*this);
    column.SetTableName(_strTableName);
    return ETKSQLite3Expression(ETKSQLite3Value(GetFullName(),ETKSQLite3Expression::eOperationAs), // GetFullName() is not used into select but used into inner join (very important)
                                  ETKSQLite3Expression(column),
                                  // _strTableName+column.GetColumnName() is the automatic alias
                                  ETKSQLite3Expression(_strAsName.IsEmpty()
                                                         ? _strTableName+column.GetColumnName()
                                                         : _strAsName,
                                                         ETKSQLite3Expression::eExpressionTypeValue)
                                 );
}

void ETKSQLite3Column::ChangeAs(wxString _strTableName,wxString _strAsName)
{
    wxASSERT_MSG(m_ColumnAttributes.IsBindedValueExists(),wxT("Don't call ETKSQLite3Column::ChangeAs function on static column without value to bind!"));

    m_strColumnName = ComputeColumnChangeAs(_strTableName,_strAsName).GetColumnName();
}

ETKSQLite3Column ETKSQLite3Column::ComputeColumnChangeAs(wxString _strTableName,wxString _strAsName) const
{
    return ETKSQLite3Column(GetTableName(),_strAsName.IsEmpty()
                              ? _strTableName + GetColumnName()
                              : _strAsName);
}

ETKSQLite3Expression ETKSQLite3Column::IsNull() const
{
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationIsNull,
                                  ETKSQLite3Expression(*this),
                                  ETKSQLite3Expression()
                                 );
}

ETKSQLite3Expression ETKSQLite3Column::IsNotNull() const
{
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationIsNotNull,
                                  ETKSQLite3Expression(*this),
                                  ETKSQLite3Expression()
                                 );
}

ETKSQLite3Expression ETKSQLite3Column::operator!=(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationDifferent,
                                  ETKSQLite3Expression(*this),
                                  _rExpression
                                 );
}

ETKSQLite3Expression ETKSQLite3Column::operator>(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(*this) > _rExpression;
}

ETKSQLite3Expression ETKSQLite3Column::operator>=(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(*this) >= _rExpression;
}

ETKSQLite3Expression ETKSQLite3Column::operator<(const ETKSQLite3Expression &_rValue) const
{
    return ETKSQLite3Expression(*this) < _rValue;
}

ETKSQLite3Expression ETKSQLite3Column::operator<=(const ETKSQLite3Expression &_rValue) const
{
    return ETKSQLite3Expression(*this) <= _rValue;
}


ETKSQLite3Expression ETKSQLite3Column::operator*(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationMult,
                                  ETKSQLite3Expression(*this),
                                  _rExpression
                                 );
}

ETKSQLite3Expression ETKSQLite3Column::operator/(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationDiv,
                                  ETKSQLite3Expression(*this),
                                  _rExpression
                                 );
}

ETKSQLite3Expression ETKSQLite3Column::operator+(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationAdd,
                                  ETKSQLite3Expression(*this),
                                  _rExpression
                                 );
}

ETKSQLite3Expression ETKSQLite3Column::operator-(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationSub,
                                  ETKSQLite3Expression(*this),
                                  _rExpression
                                 );
}

// Column expression operators
//-------------------------------
