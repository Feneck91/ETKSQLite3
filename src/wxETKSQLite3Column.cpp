/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3Column.h
// Library:     wxETKSQLite3
// Purpose:     Code for column managment
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/04
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "wxETKSQLite3Column.h"
#include "wxETKSQLite3ValueBind.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                     wxETKSQLite3ColumnAttributes                                      //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
wxETKSQLite3ColumnAttributes::wxETKSQLite3ColumnAttributes()
    : m_bAddForInsertRequest(true)
{
}

wxETKSQLite3ColumnAttributes::wxETKSQLite3ColumnAttributes(const wxETKSQLite3ValueBind & _rBindValue,bool _bAddForInsertRequest)
    : m_bAddForInsertRequest(_bAddForInsertRequest)
    , m_pBindValue(_rBindValue.Clone())
{
}

wxETKSQLite3ColumnAttributes::wxETKSQLite3ColumnAttributes(const wxETKSQLite3ColumnAttributes &_rColumnAttributes)
{
    operator=(_rColumnAttributes);
}

wxETKSQLite3ColumnAttributes::~wxETKSQLite3ColumnAttributes()
{
    // Nothing to do to delete m_pBindValue
}

void wxETKSQLite3ColumnAttributes::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bForInsertRequest) const
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

void wxETKSQLite3ColumnAttributes::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
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

bool wxETKSQLite3ColumnAttributes::IsNull() const
{
    wxASSERT_MSG(m_pBindValue.IsNotNull(),wxT("IsNull on null bind value (attempt to call on a static column member ?)"));
    if (m_pBindValue.IsNotNull())
    {
        return m_pBindValue->IsNull();
    }
    return false;
}

void wxETKSQLite3ColumnAttributes::SetNull(bool _bIsNull) const
{
    wxASSERT_MSG(m_pBindValue.IsNotNull(),wxT("SetNull on null bind value (attempt to call on a static column member ?)"));
    if (m_pBindValue.IsNotNull())
    {
        return m_pBindValue->SetNull(_bIsNull);
    }
}

const wxETKSQLite3ColumnAttributes & wxETKSQLite3ColumnAttributes::operator=(const wxETKSQLite3ColumnAttributes &_rColumnAttributes)
{
    // Let pointer on same element, use shared pointer to know when delete it
    m_pBindValue = _rColumnAttributes.m_pBindValue;
    m_bAddForInsertRequest  = _rColumnAttributes.m_bAddForInsertRequest;

    return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                           wxETKSQLite3Column                                          //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

wxETKSQLite3Column::wxETKSQLite3Column(int _iColumnIndex)
    : m_strTableName()
    , m_strColumnName()
    , m_ColumnAttributes()
    , m_iColumnIndex(_iColumnIndex)
{
}

wxETKSQLite3Column::wxETKSQLite3Column(wxString _strTableName,wxString _strColumnName)
    : m_strTableName(_strTableName)
    , m_strColumnName(_strColumnName)
    , m_ColumnAttributes()
    , m_iColumnIndex(-1)
{
}

wxETKSQLite3Column::wxETKSQLite3Column(const wxETKSQLite3Column &_rColumn,const wxETKSQLite3ColumnAttributes &_rColumnAttributes)
    : m_strTableName(_rColumn.GetTableName())
    , m_strColumnName(_rColumn.GetColumnName())
    , m_ColumnAttributes(_rColumnAttributes)
    , m_iColumnIndex(_rColumn.GetColumnIndex())
{
}

wxETKSQLite3Column::wxETKSQLite3Column(const wxETKSQLite3Column &_rColumn)
{
    operator=(_rColumn);
}

wxETKSQLite3Column::~wxETKSQLite3Column()
{
}

int wxETKSQLite3Column::GetColumnIndex() const
{
    return m_iColumnIndex;
}

bool wxETKSQLite3Column::HasColumnIndex() const
{
    return GetColumnIndex() != -1;
}

wxString wxETKSQLite3Column::GetTableName() const
{
    return m_strTableName;
}

void wxETKSQLite3Column::SetTableName(wxString _strTableName)
{
    m_strTableName = _strTableName;
}

wxString wxETKSQLite3Column::GetColumnName() const
{
    return m_strColumnName;
}

wxString wxETKSQLite3Column::GetFullName() const
{
    return wxString::Format(_T("%s.%s"),GetTableName().wx_str(),GetColumnName().wx_str());
}

void wxETKSQLite3Column::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bForInsertRequest) const
{
    m_ColumnAttributes.BindTo(_rstmt,_iIndex,_bForInsertRequest);
}

void wxETKSQLite3Column::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
{
    m_ColumnAttributes.BindFrom(_rResultSet,_iIndex);
}

const wxETKSQLite3ColumnAttributes & wxETKSQLite3Column::GetColumnAttributes() const
{
    return m_ColumnAttributes;
}

const wxETKSQLite3ColumnAttributes & wxETKSQLite3Column::operator*() const
{
    return m_ColumnAttributes;
}

const wxETKSQLite3ColumnAttributes * wxETKSQLite3Column::operator->() const
{
    return &m_ColumnAttributes;
}

#ifdef __WXDEBUG__
bool wxETKSQLite3Column::IsBindedValueExists() const
{
    return m_ColumnAttributes.IsBindedValueExists();
}
#endif

const wxETKSQLite3Column & wxETKSQLite3Column::operator=(const wxETKSQLite3Column &_rColumn)
{
    m_strTableName      = _rColumn.m_strTableName;
    m_strColumnName     = _rColumn.m_strColumnName;
    m_ColumnAttributes  = _rColumn.m_ColumnAttributes;
    m_iColumnIndex      = _rColumn.m_iColumnIndex;

    return *this;
}

//-------------------------------
// Column expression operators
wxETKSQLite3Expression wxETKSQLite3Column::operator==(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationEqual,
                                  wxETKSQLite3Expression(*this),
                                  _rExpression
                                 );
}

wxETKSQLite3Expression wxETKSQLite3Column::operator==(const wxETKSQLite3Column &_rColumn) const
{
    // Join between columns
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationEqual,
                                  wxETKSQLite3Expression(*this),
                                  wxETKSQLite3Expression(_rColumn)
                                 );
}

wxETKSQLite3Expression wxETKSQLite3Column::Join(const wxETKSQLite3Column &_rColumn) const
{
    // Join between columns
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationJoin,
                                  wxETKSQLite3Expression(*this),
                                  wxETKSQLite3Expression(_rColumn)
                                 );
}

wxETKSQLite3Expression wxETKSQLite3Column::LeftJoin(const wxETKSQLite3Column &_rColumn) const
{
    // Left join between columns
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationLeftJoin,
                                  wxETKSQLite3Expression(*this),
                                  wxETKSQLite3Expression(_rColumn)
                                 );
}

wxETKSQLite3Expression wxETKSQLite3Column::InnerJoin(const wxETKSQLite3Column &_rColumn) const
{
    // Inner join between columns
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationInnerJoin,
                                  wxETKSQLite3Expression(*this),
                                  wxETKSQLite3Expression(_rColumn)
                                 );
}

// NOT SUPPORTED
//wxETKSQLite3Expression wxETKSQLite3Column::RightJoin(const wxETKSQLite3Column &_rColumn) const
//{
//    // Left join between columns
//    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationRightJoin,
//                                  wxETKSQLite3Expression(*this),
//                                  wxETKSQLite3Expression(_rColumn)
//                                 );
//}
//
//wxETKSQLite3Expression wxETKSQLite3Column::FullJoin(const wxETKSQLite3Column &_rColumn) const
//{
//    // Left join between columns
//    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationFullJoin,
//                                  wxETKSQLite3Expression(*this),
//                                  wxETKSQLite3Expression(_rColumn)
//                                 );
//}

wxETKSQLite3Expression wxETKSQLite3Column::In(const wxETKSQLite3RequestSelector &_rSelector) const
{
    return wxETKSQLite3Expression(*this).In(wxETKSQLite3Expression(_rSelector));
}

wxETKSQLite3Expression wxETKSQLite3Column::In(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(*this).In(wxETKSQLite3Expression(_rExpression));
}


wxETKSQLite3Expression wxETKSQLite3Column::Like(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationLike,
                                  wxETKSQLite3Expression(*this),
                                  _rExpression
                                 );
}

wxETKSQLite3Expression wxETKSQLite3Column::As(wxString _strTableName,wxString _strAsName) const
{
    wxETKSQLite3Column column(*this);
    column.SetTableName(_strTableName);
    return wxETKSQLite3Expression(wxETKSQLite3Value(GetFullName(),wxETKSQLite3Expression::eOperationAs), // GetFullName() is not used into select but used into inner join (very important)
                                  wxETKSQLite3Expression(column),
                                  // _strTableName+column.GetColumnName() is the automatic alias
                                  wxETKSQLite3Expression(_strAsName.IsEmpty()
                                                         ? _strTableName+column.GetColumnName()
                                                         : _strAsName,
                                                         wxETKSQLite3Expression::eExpressionTypeValue)
                                 );
}

void wxETKSQLite3Column::ChangeAs(wxString _strTableName,wxString _strAsName)
{
    wxASSERT_MSG(m_ColumnAttributes.IsBindedValueExists(),wxT("Don't call wxETKSQLite3Column::ChangeAs function on static column without value to bind!"));

    m_strColumnName = ComputeColumnChangeAs(_strTableName,_strAsName).GetColumnName();
}

wxETKSQLite3Column wxETKSQLite3Column::ComputeColumnChangeAs(wxString _strTableName,wxString _strAsName) const
{
    return wxETKSQLite3Column(GetTableName(),_strAsName.IsEmpty()
                              ? _strTableName + GetColumnName()
                              : _strAsName);
}

wxETKSQLite3Expression wxETKSQLite3Column::IsNull() const
{
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationIsNull,
                                  wxETKSQLite3Expression(*this),
                                  wxETKSQLite3Expression()
                                 );
}

wxETKSQLite3Expression wxETKSQLite3Column::IsNotNull() const
{
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationIsNotNull,
                                  wxETKSQLite3Expression(*this),
                                  wxETKSQLite3Expression()
                                 );
}

wxETKSQLite3Expression wxETKSQLite3Column::operator!=(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationDifferent,
                                  wxETKSQLite3Expression(*this),
                                  _rExpression
                                 );
}

wxETKSQLite3Expression wxETKSQLite3Column::operator>(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(*this) > _rExpression;
}

wxETKSQLite3Expression wxETKSQLite3Column::operator>=(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(*this) >= _rExpression;
}

wxETKSQLite3Expression wxETKSQLite3Column::operator<(const wxETKSQLite3Expression &_rValue) const
{
    return wxETKSQLite3Expression(*this) < _rValue;
}

wxETKSQLite3Expression wxETKSQLite3Column::operator<=(const wxETKSQLite3Expression &_rValue) const
{
    return wxETKSQLite3Expression(*this) <= _rValue;
}


wxETKSQLite3Expression wxETKSQLite3Column::operator*(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationMult,
                                  wxETKSQLite3Expression(*this),
                                  _rExpression
                                 );
}

wxETKSQLite3Expression wxETKSQLite3Column::operator/(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationDiv,
                                  wxETKSQLite3Expression(*this),
                                  _rExpression
                                 );
}

wxETKSQLite3Expression wxETKSQLite3Column::operator+(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationAdd,
                                  wxETKSQLite3Expression(*this),
                                  _rExpression
                                 );
}

wxETKSQLite3Expression wxETKSQLite3Column::operator-(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationSub,
                                  wxETKSQLite3Expression(*this),
                                  _rExpression
                                 );
}

// Column expression operators
//-------------------------------
