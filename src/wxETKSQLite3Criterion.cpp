/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3Criterion.h
// Library:     wxETKSQLite3
// Purpose:     Use to make criterion to use into request and for format SQL.
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/04
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "wxETKSQLite3Criterion.h"
#include "wxETKSQLite3Column.h"
#include "wxETKSQLite3ResultSet.h"
#include "wxETKSQLite3VariantDatas.h"
#include "wxETKSQLite3RequestSelector.h"

#include <algorithm>
#include <wx/tokenzr.h>

// Classe used to concat the for_each of all elements, separates with with comma.
struct EXPORT_IMPORT wxETKSQLite3StringConcat
{
    wxString  m_strValue;

    wxString  operator()(const std::pair<wxString const,bool> &_Value)
    {
        if (_Value.second)
        {
            if (m_strValue.Len()>0)
            {
                m_strValue += _T(",");
            }
            m_strValue += _Value.first;
        }
        return m_strValue;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                         wxETKSQLite3Value                                             //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
wxETKSQLite3Value::wxETKSQLite3Value(eExpressionOrOperationType _expressionType)
    : m_ExpressionType(_expressionType)
{
}

// Copy constructor
wxETKSQLite3Value::wxETKSQLite3Value(const wxETKSQLite3Value &_rOther)
{
    operator=(_rOther);
}

wxETKSQLite3Value::wxETKSQLite3Value(const wxETKSQLite3ColumnAttributes &_rColumnAttributes)
    : m_Value(new wxETKSQLite3VariantDataColumnAttributes(_rColumnAttributes))
    , m_ExpressionType(eExpressionColumnAttributes)
{
}

wxETKSQLite3Value::wxETKSQLite3Value(const wxETKSQLite3ValueBind &_rValueBind)
    : m_Value(new wxETKSQLite3VariantDataValueBind(_rValueBind))
    , m_ExpressionType(eExpressionValueBind)
{
}

wxETKSQLite3Value::wxETKSQLite3Value(const wxETKSQLite3RequestSelector& _rSelector)
    : m_Value(new wxETKSQLite3VariantDataRequestSelector(_rSelector))
    , m_ExpressionType(eExpressionRequestSelector)
{
}

// Value constructor (as variant)
wxETKSQLite3Value::wxETKSQLite3Value(const wxVariant &_rValue)
    : m_Value(_rValue)
    , m_ExpressionType(eExpressionTypeValue)
{
}

wxETKSQLite3Value::wxETKSQLite3Value(const struct tm &_rtmValue)
    : m_Value(wxDateTime(_rtmValue).MakeUTC())
    , m_ExpressionType(eExpressionTypeValue)
{
}

wxETKSQLite3Value::wxETKSQLite3Value(const wxDateTime &_rdtValue)
    : m_Value(wxDateTime(_rdtValue).MakeUTC())
    , m_ExpressionType(eExpressionTypeValue)
{
}

wxETKSQLite3Value::wxETKSQLite3Value(long _lValue)
    : m_Value(_lValue)
    , m_ExpressionType(eExpressionTypeValue)
{
}

wxETKSQLite3Value::wxETKSQLite3Value(int _iValue)
    : m_Value((long) _iValue)
    , m_ExpressionType(eExpressionTypeValue)
{
}

wxETKSQLite3Value::wxETKSQLite3Value(double _dValue)
    : m_Value(_dValue)
    , m_ExpressionType(eExpressionTypeValue)
{
}

wxETKSQLite3Value::wxETKSQLite3Value(const wxLongLong &_rllValue)
    : m_Value(new wxETKSQLite3VariantDataLongLong(_rllValue))
    , m_ExpressionType(eExpressionTypeValue)
{
}

wxETKSQLite3Value::wxETKSQLite3Value(const wxChar *_pszValue,eExpressionOrOperationType _expressionType)
    : m_Value(wxString(_pszValue))
    , m_ExpressionType(_expressionType)
{
}

wxETKSQLite3Value::wxETKSQLite3Value(wxString _strValue,eExpressionOrOperationType _expressionType)
    : m_Value(_strValue)
    , m_ExpressionType(_expressionType)
{
}

wxETKSQLite3Value::~wxETKSQLite3Value()
{
    Clear();
}

const wxETKSQLite3Value & wxETKSQLite3Value::operator=(const wxETKSQLite3Value &_rExpression)
{
    m_Value = _rExpression.m_Value;
    SetExpressionOrOperationType(_rExpression.GetExpressionOrOperationType());

    return *this;
}

const wxVariant & wxETKSQLite3Value::GetVariant() const
{
    wxASSERT(   GetExpressionOrOperationType() == eExpressionColumnNameFull
             || GetExpressionOrOperationType() == eExpressionColumnNameOnly
             || GetExpressionOrOperationType() == eExpressionTypeValue
             || GetExpressionOrOperationType() == eExpressionTypeValueQuoteString
             || GetExpressionOrOperationType() == eExpressionColumnAttributes
             || GetExpressionOrOperationType() == eExpressionValueBind
             || GetExpressionOrOperationType() == eExpressionTypeSQL
             || GetExpressionOrOperationType() == eOperationCount
             || GetExpressionOrOperationType() == eOperationAsJoin
             || GetExpressionOrOperationType() == eExpressionRequestSelector
             );

    return m_Value;
}

#ifdef __WXDEBUG__
const wxVariant & wxETKSQLite3Value::GetVariantDebug() const
{
    return m_Value;
}
#endif

wxString wxETKSQLite3Value::GetColumnName() const
{
    wxASSERT(   GetExpressionOrOperationType() == eExpressionColumnNameFull
             || GetExpressionOrOperationType() == eExpressionColumnNameOnly
             || GetExpressionOrOperationType() == eOperationCount);

    wxString strNameColumn(GetVariant().GetString());
    int iFindIndex = strNameColumn.Find(_T('.'));
    if (iFindIndex != wxNOT_FOUND)
    {   // Find column name
        strNameColumn = strNameColumn.Right(strNameColumn.length() - iFindIndex - 1);
    }

    return strNameColumn;
}

wxString wxETKSQLite3Value::GetTableName() const
{
    wxASSERT(   GetExpressionOrOperationType() == eExpressionColumnNameFull
             || GetExpressionOrOperationType() == eExpressionColumnNameOnly
             || GetExpressionOrOperationType() == eOperationAsJoin); // AsJoin operation ask for table name too

    wxString strNameColumn(GetVariant().GetString());
    int iFindIndex = strNameColumn.Find(_T('.'));
    if (iFindIndex != wxNOT_FOUND)
    {   // Trouver le nom de la table
        strNameColumn = strNameColumn.Left(iFindIndex);
    }
    else
    {
        wxFAIL_MSG(wxString::Format(wxT("Table name (%s) of type %d is not a Column type"),strNameColumn.wx_str(),GetExpressionOrOperationType())); // Strange, the table name doesn't exist!
        strNameColumn.clear();
    }

    return strNameColumn;
}

void wxETKSQLite3Value::BindTo(wxSQLite3Statement &_rstmt,int &_riIndex,bool _bForInsertRequest) const
{
    if (GetExpressionOrOperationType() == eExpressionColumnAttributes)
    {
        wxETKSQLite3VariantDataColumnAttributes *pColumnAttributes = dynamic_cast<wxETKSQLite3VariantDataColumnAttributes *>(GetVariant().GetData());
        wxASSERT(pColumnAttributes != NULL); // Should never arrives
        if (pColumnAttributes != NULL)
        {
            pColumnAttributes->BindTo(_rstmt,_riIndex++,_bForInsertRequest);
        }
    }
    else if (GetExpressionOrOperationType() == eExpressionValueBind)
    {
        wxETKSQLite3VariantDataValueBind *pValueBind = dynamic_cast<wxETKSQLite3VariantDataValueBind *>(GetVariant().GetData());
        wxASSERT(pValueBind != NULL); // Should never arrives
        if (pValueBind != NULL)
        {
            pValueBind->BindTo(_rstmt,_riIndex++,true,_bForInsertRequest);
        }
    }
    else if (GetExpressionOrOperationType() == eExpressionRequestSelector)
    {
        wxETKSQLite3VariantDataRequestSelector *pRequestSelector = dynamic_cast<wxETKSQLite3VariantDataRequestSelector *>(GetVariant().GetData());
        wxASSERT(pRequestSelector != NULL); // Should never arrives
        if (pRequestSelector != NULL)
        {
            pRequestSelector->GetCriterionRequest().BindTo(_rstmt,_riIndex);
        }
    }
}

wxString wxETKSQLite3Value::GetAsString() const
{
    wxString strCriterionAsString;

    switch (GetExpressionOrOperationType())
    {
        case eExpressionTypeNothing :
        {
            break;
        }
        case eExpressionOrderByASC :
        {
            strCriterionAsString.Printf(_T("%s ASC"),m_Value.GetString().c_str());
            break;
        }
        case eExpressionOrderByDESC :
        {
            strCriterionAsString.Printf(_T("%s DESC"),m_Value.GetString().c_str());
            break;
        }
        case eOperationMin :
        case eOperationMax :
        case eOperationSum :
        case eOperationCount : // For case when function is called on column
        {
            if (!IsValueEmpty())
            {
                strCriterionAsString = m_Value.GetString();
            }
            // no break, continue! (overwrite in case of date)
        }
        case eExpressionTypeSQL :
        case eExpressionTypeValue :
        {
            if (m_Value.GetType() == _T("double"))
            {   // Must override local else the '.' could become ',' (in french) and SQL don't like it
                strCriterionAsString.Printf(_T("%f"),m_Value.GetDouble());
                strCriterionAsString.Replace(_T(","),_T("."));
            }
            else if (m_Value.GetType() == _T("time"))
            {
                strCriterionAsString = m_Value.GetDateTime().Format(_T("'%H:%M:%S'"));
            }
            else if (m_Value.GetType() == _T("datetime"))
            {
                strCriterionAsString = m_Value.GetDateTime().Format(_T("'%Y-%m-%d %H:%M:%S'"));
            }
            else
            {
                strCriterionAsString = m_Value.GetString();
            }
            break;
        }
        // Simple conversion to string
        case eExpressionColumnNameFull :
        case eExpressionColumnNameOnly :
        case eExpressionColumnAttributes :
        case eExpressionValueBind :
        {
            strCriterionAsString = m_Value.GetString();
            break;
        }
        case eExpressionRequestSelector :
        {   // Add () around the value
            strCriterionAsString.Printf(_T("(%s)"),m_Value.GetString().c_str());
            break;
        }
        case eExpressionTypeValueQuoteString :
        {
            strCriterionAsString.Printf(_T("'%s'"),QuoteString(m_Value.GetString()).c_str());
            break;
        }
        case eOperationOr :
        case eOperationAnd :
        case eOperationNot :
        case eOperationUpper :
        case eOperationUpperOrEqual :
        case eOperationLower :
        case eOperationLowerOrEqual :
        case eOperationEqual :
        case eOperationLike :
        case eOperationAs :
        case eOperationAsSelect :
        case eOperationAsJoin :
        case eOperationDifferent :
        case eOperationAssignmentInsert :
        case eOperationAssignmentUpdate :
        case eOperationAndJoin :
        case eOperationJoin :
        case eOperationInnerJoin :
        case eOperationLeftJoin :
        case eOperationRightJoin :
        case eOperationFullJoin :
        case eOperationIn :
        case eOperationCast :
        case eOperationIsNull :
        case eOperationIsNotNull :
        case eOperationMult :
        case eOperationDiv :
        case eOperationAdd :
        case eOperationSub :
        case eOperationAddComma :
        case eOperationDistinct :
        {
            wxASSERT(false);
            break;
        }
    }
    return strCriterionAsString;
}

bool wxETKSQLite3Value::IsValueEmpty() const
{
    return m_Value.IsNull();
}

wxETKSQLite3Value::eExpressionOrOperationType wxETKSQLite3Value::GetExpressionOrOperationType() const
{
    return m_ExpressionType;
}

void wxETKSQLite3Value::SetExpressionOrOperationType(wxETKSQLite3Value::eExpressionOrOperationType _expressionType)
{
    m_ExpressionType = _expressionType;
}

void wxETKSQLite3Value::Clear()
{
    m_Value.Clear();
    SetExpressionOrOperationType(eExpressionTypeNothing);
}

bool wxETKSQLite3Value::IsAndOrOR() const
{
    bool bRet = false;

    switch (GetExpressionOrOperationType())
    {
        case eOperationAnd :
        case eOperationAndJoin :
        case eOperationOr :
        {
            bRet = true; // Ok, It's an OR or AND operator
            break;

        }
        case eExpressionTypeNothing :
        case eExpressionColumnNameFull :
        case eExpressionColumnNameOnly :
        case eExpressionColumnAttributes :
        case eExpressionValueBind :
        case eExpressionRequestSelector :
        case eExpressionOrderByASC :
        case eExpressionOrderByDESC :
        case eExpressionTypeValue :
        case eExpressionTypeValueQuoteString :
        case eExpressionTypeSQL :
        case eOperationNot :
        case eOperationUpper :
        case eOperationUpperOrEqual :
        case eOperationLower :
        case eOperationLowerOrEqual :
        case eOperationEqual :
        case eOperationLike :
        case eOperationAs :
        case eOperationAsSelect :
        case eOperationAsJoin :
        case eOperationDifferent :
        case eOperationCount :
        case eOperationMin :
        case eOperationMax :
        case eOperationSum :
        case eOperationAssignmentInsert :
        case eOperationAssignmentUpdate :
        case eOperationJoin :
        case eOperationInnerJoin :
        case eOperationLeftJoin :
        case eOperationRightJoin :
        case eOperationFullJoin :
        case eOperationIn :
        case eOperationCast :
        case eOperationIsNull :
        case eOperationIsNotNull :
        case eOperationMult :
        case eOperationDiv :
        case eOperationAdd :
        case eOperationSub :
        case eOperationAddComma :
        case eOperationDistinct :
        {   // Don't care for other values
            break;
        }
    }

    return bRet;
}

wxString wxETKSQLite3Value::QuoteString(wxString _strValue) const
{
    _strValue.Replace(_T("'"),_T("''"));
    return _strValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                      wxETKSQLite3Expression                                           //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __WXDEBUG__
unsigned long wxETKSQLite3Expression::s_ulCurrentCounterId = 0;
#endif

//----------------------------
// Constructors / Destructor

// Default expression
wxETKSQLite3Expression::wxETKSQLite3Expression(eExpressionOrOperationType _expressionType)
    : wxETKSQLite3Value(_expressionType)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

// Copy constructor
wxETKSQLite3Expression::wxETKSQLite3Expression(const wxETKSQLite3Expression &_rExpression)
    : m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
    operator=(_rExpression);
}

// Two expressions
wxETKSQLite3Expression::wxETKSQLite3Expression(eExpressionOrOperationType _expressionType,const wxETKSQLite3Expression &_rExpression1,const wxETKSQLite3Expression &_rExpression2)
    : wxETKSQLite3Value(_expressionType)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
    Assign2Expressions(_rExpression1,_rExpression2);
}

wxETKSQLite3Expression::wxETKSQLite3Expression(const wxETKSQLite3Value &_rValueAndExpressionType,const wxETKSQLite3Expression &_rExpression1,const wxETKSQLite3Expression &_rExpression2)
    : wxETKSQLite3Value(_rValueAndExpressionType)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
    Assign2Expressions(_rExpression1,_rExpression2);
}

// With column
wxETKSQLite3Expression::wxETKSQLite3Expression(const wxETKSQLite3Column &_rColumn,eExpressionOrOperationType _expressionType)
    : wxETKSQLite3Value(_rColumn.GetFullName(),_expressionType)  // Let full name is it can be needed but the AsString function will return only column name
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
    wxASSERT(GetExpressionOrOperationType() == eExpressionColumnNameFull || GetExpressionOrOperationType() == eExpressionColumnNameOnly);
}

// Binding column with attributes
wxETKSQLite3Expression::wxETKSQLite3Expression(const wxETKSQLite3ColumnAttributes &_rColumnAttributes)
    : wxETKSQLite3Value(_rColumnAttributes)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

// Binding column with attributes
wxETKSQLite3Expression::wxETKSQLite3Expression(const wxETKSQLite3ValueBind &_rValueBind)
    : wxETKSQLite3Value(_rValueBind)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

// With wxETKSQLite3Value value
wxETKSQLite3Expression::wxETKSQLite3Expression(const wxETKSQLite3Value &_rValue)
    : wxETKSQLite3Value(_rValue)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

wxETKSQLite3Expression::wxETKSQLite3Expression(const wxVariant &_rValue)
    : wxETKSQLite3Value(_rValue)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

wxETKSQLite3Expression::wxETKSQLite3Expression(const struct tm &_rtmValue)
    : wxETKSQLite3Value(_rtmValue)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

wxETKSQLite3Expression::wxETKSQLite3Expression(const wxDateTime &_rdtValue)
    : wxETKSQLite3Value(_rdtValue)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

wxETKSQLite3Expression::wxETKSQLite3Expression(long _lValue)
    : wxETKSQLite3Value(_lValue)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

wxETKSQLite3Expression::wxETKSQLite3Expression(int _iValue)
    : wxETKSQLite3Value(_iValue)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

wxETKSQLite3Expression::wxETKSQLite3Expression(double _dValue)
    : wxETKSQLite3Value(_dValue)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

wxETKSQLite3Expression::wxETKSQLite3Expression(const wxLongLong &_rllValue)
    : wxETKSQLite3Value(_rllValue)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

wxETKSQLite3Expression::wxETKSQLite3Expression(const wxChar *_pszValue,eExpressionOrOperationType _expressionType)
    : wxETKSQLite3Value(_pszValue,_expressionType)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

wxETKSQLite3Expression::wxETKSQLite3Expression(wxString _value,eExpressionOrOperationType _expressionType)
    : wxETKSQLite3Value(_value,_expressionType)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

wxETKSQLite3Expression::wxETKSQLite3Expression(const wxETKSQLite3RequestSelector& _rSelector)
    : wxETKSQLite3Value(_rSelector)
    , m_pExpression1(NULL)
    , m_pExpression2(NULL)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

// Destructor
wxETKSQLite3Expression::~wxETKSQLite3Expression()
{
    Clear();
}

// Two expressions, used ONLY into 2 constructors, private function
void wxETKSQLite3Expression::Assign2Expressions(const wxETKSQLite3Expression &_rExpression1,const wxETKSQLite3Expression &_rExpression2)
{
    if (IsAndOrOR())
    {   // Often, one && (and) is done with empty expressions, created big tree with empty nodes!
        // The following tests are used to simplify expression at creation
        if (_rExpression1.GetExpressionOrOperationType() == eExpressionTypeNothing)
        {
            if (_rExpression2.GetExpressionOrOperationType() == eExpressionTypeNothing)
            {   // One AND or OR with 2 empty expressions? Nothing to do
                SetExpressionOrOperationType(eExpressionTypeNothing);
            }
            else
            {   // One AND or OR with only one expression (expr 2)? This is equal to this one
                operator = (_rExpression2);
            }
        }
        else
        {
            if (_rExpression2.GetExpressionOrOperationType() == eExpressionTypeNothing)
            {   // One AND or OR with only one expression (expr 1)? This is equal to this one
                operator = (_rExpression1);
            }
            else
            {   // One AND or OR with 2 non empty expressions: Initialize the 2 expressions
                m_pExpression1 = new wxETKSQLite3Expression(_rExpression1);
                m_pExpression2 = new wxETKSQLite3Expression(_rExpression2);
            }
        }
    }
    else
    {
        if (_rExpression1.GetExpressionOrOperationType() != eExpressionTypeNothing)
        {
            m_pExpression1 = new wxETKSQLite3Expression(_rExpression1);
        }
        if (_rExpression2.GetExpressionOrOperationType() != eExpressionTypeNothing)
        {
            m_pExpression2 = new wxETKSQLite3Expression(_rExpression2);
        }

        wxASSERT_MSG(m_pExpression1 != NULL || m_pExpression2 == NULL,wxT("Expression 1 is NULL and Expression 2 is not, it is not allowed! Check your source code!"));
    }
}

// Constructors / Destructor
//----------------------------


//----------------------------
// Operators

// Assign
const wxETKSQLite3Expression &wxETKSQLite3Expression::operator=(const wxETKSQLite3Expression &_rExpression)
{
    Clear();
    wxETKSQLite3Value::operator=(_rExpression);

    if (_rExpression.m_pExpression1 != NULL)
    {
        m_pExpression1 = new wxETKSQLite3Expression(*_rExpression.m_pExpression1);
    }

    if (_rExpression.m_pExpression2 != NULL)
    {
        m_pExpression2 = new wxETKSQLite3Expression(*_rExpression.m_pExpression2);
    }

    return *this;
}

// Assign with request selector
const wxETKSQLite3Expression & wxETKSQLite3Expression::operator=(const wxETKSQLite3RequestSelector& _rSelector)
{
    Clear();
    wxETKSQLite3Value::operator=(_rSelector);
    return *this;
}

// Logical OR operator.
wxETKSQLite3Expression wxETKSQLite3Expression::operator||(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(eOperationOr,*this,_rExpression);
}

// Logical AND operator.
wxETKSQLite3Expression wxETKSQLite3Expression::operator&&(const wxETKSQLite3Expression &_rExpression) const
{
    if (_rExpression.IsJoin())
    {
        wxASSERT_MSG(GetExpressionOrOperationType()==eExpressionTypeNothing || IsJoin(),wxT("This is JOIN, expression must be Nothing or JOIN, check your source code!"));
        return wxETKSQLite3Expression(eOperationAndJoin,*this,_rExpression);
    }
    return wxETKSQLite3Expression(eOperationAnd,*this,_rExpression);
}

// Logical == operator.
wxETKSQLite3Expression wxETKSQLite3Expression::operator==(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(eOperationEqual,*this,_rExpression);
}

// Logical == operator on column.
wxETKSQLite3Expression wxETKSQLite3Expression::operator==(const wxETKSQLite3Column &_rColumn) const
{   // Call same operator but with expression that contains the full column name.
    return operator==(wxETKSQLite3Expression(_rColumn.GetFullName(),eExpressionTypeValue));
}

// Logical > operator.
wxETKSQLite3Expression wxETKSQLite3Expression::operator>(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(eOperationUpper,*this,_rExpression);
}

wxETKSQLite3Expression wxETKSQLite3Expression::operator>(const wxETKSQLite3Column &_rColumn) const
{   // Call same operator but with expression that contains the full column name.
    return operator>(wxETKSQLite3Expression(_rColumn));
}

// Logical >= operator.
wxETKSQLite3Expression wxETKSQLite3Expression::operator>=(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(eOperationUpperOrEqual,*this,_rExpression);
}

wxETKSQLite3Expression wxETKSQLite3Expression::operator>=(const wxETKSQLite3Column &_rColumn) const
{   // Call same operator but with expression that contains the full column name.
    return operator>=(wxETKSQLite3Expression(_rColumn));
}

// Logical < operator.
wxETKSQLite3Expression wxETKSQLite3Expression::operator<(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(eOperationLower,*this,_rExpression);
}

wxETKSQLite3Expression wxETKSQLite3Expression::operator<(const wxETKSQLite3Column &_rColumn) const
{   // Call same operator but with expression that contains the full column name.
    return operator<(wxETKSQLite3Expression(_rColumn));
}

// Logical <= operator.
wxETKSQLite3Expression wxETKSQLite3Expression::operator<=(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(eOperationLowerOrEqual,*this,_rExpression);
}

wxETKSQLite3Expression wxETKSQLite3Expression::operator<=(const wxETKSQLite3Column &_rColumn) const
{   // Call same operator but with expression that contains the full column name.
    return operator<=(wxETKSQLite3Expression(_rColumn.GetFullName(),eExpressionTypeValue));
}

wxETKSQLite3Expression wxETKSQLite3Expression::operator!() const
{
    return wxETKSQLite3Expression(eOperationNot,*this,wxETKSQLite3Expression());
}

wxETKSQLite3Expression wxETKSQLite3Expression::Like(const wxETKSQLite3Expression &_rExpression) const
{
    return wxETKSQLite3Expression(eOperationLike,*this,_rExpression);
}

wxETKSQLite3Expression wxETKSQLite3Expression::In(const wxETKSQLite3Expression &_rExpression) const
{
    // Must be column type
    wxASSERT(GetExpressionOrOperationType() == eExpressionColumnNameFull);
    // Often formatted directly into SQL or selector, If not case, code should be analyzed and be upgraded, making new tests!
    wxASSERT(_rExpression.GetExpressionOrOperationType() == eExpressionTypeSQL || _rExpression.GetExpressionOrOperationType() == eExpressionRequestSelector);
    if (!_rExpression.IsNull())
    {   // If In is not empty
        return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationIn,*this,_rExpression);
    }

    return wxETKSQLite3Expression(); // If the 'in' is empty, return empty expression
}

wxETKSQLite3Expression wxETKSQLite3Expression::Join(const wxETKSQLite3Expression &_rExpressionIn) const
{
    if (!_rExpressionIn.IsNull())
    {   // If In is not empty
        wxETKSQLite3Expression exprThis(*this);
        if (GetExpressionOrOperationType() == eOperationAs)
        {   // 'This' is an alias, change type to be correctly generate SQL: JOIN Table Alias ON (expression)
            exprThis.SetExpressionOrOperationType(wxETKSQLite3Value::eOperationAsJoin);
        }
        // else generate SQL: JOIN Table ON Table.Column = Table2.Column2
        return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationJoin,exprThis,_rExpressionIn);
    }

    return wxETKSQLite3Expression(); // If the 'inner join' is empty, return empty expression
}

wxETKSQLite3Expression wxETKSQLite3Expression::InnerJoin(const wxETKSQLite3Expression &_rExpressionIn) const
{
    if (!_rExpressionIn.IsNull())
    {   // If In is not empty
        wxETKSQLite3Expression exprThis(*this);
        if (GetExpressionOrOperationType() == eOperationAs)
        {   // 'This' is an alias, change type to be correctly generate SQL: INNER JOIN Table Alias ON (expression)
            exprThis.SetExpressionOrOperationType(wxETKSQLite3Value::eOperationAsJoin);
        }
        // else generate SQL: INNER JOIN Table ON Table.Column = Table2.Column2
        return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationInnerJoin,exprThis,_rExpressionIn);
    }

    return wxETKSQLite3Expression(); // If the 'inner join' is empty, return empty expression
}

wxETKSQLite3Expression wxETKSQLite3Expression::LeftJoin(const wxETKSQLite3Expression &_rExpressionIn) const
{
    if (!_rExpressionIn.IsNull())
    {   // If In is not empty
        wxETKSQLite3Expression exprThis(*this);
        if (GetExpressionOrOperationType() == eOperationAs)
        {   // 'This' is an alias, change type to be correctly generate SQL: LEFT JOIN Table Alias ON (expression)
            exprThis.SetExpressionOrOperationType(wxETKSQLite3Value::eOperationAsJoin);
        }
        // else generate SQL: LEFT JOIN Table ON Table.Column = Table2.Column2
        return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationLeftJoin,exprThis,_rExpressionIn);
    }

    return wxETKSQLite3Expression(); // If the 'inner join' is empty, return empty expression
}

// Operators
//----------------------------

void wxETKSQLite3Expression::Clear()
{
    if (m_pExpression1 != NULL)
    {
        delete m_pExpression1;
        m_pExpression1 = NULL;
    }
    if (m_pExpression2 != NULL)
    {
        delete m_pExpression2;
        m_pExpression2 = NULL;
    }

    wxETKSQLite3Value::Clear();
}

bool wxETKSQLite3Expression::CanHaveOrderBy() const
{
    bool bRet = true;

    switch (GetExpressionOrOperationType())
    {
        case eOperationCount :
        case eOperationMin   :
        case eOperationMax   :
        case eOperationSum   :
        {
            bRet = false; // Order by is not allowed
            break;
        }
        case eExpressionTypeNothing :
        case eExpressionColumnNameFull :
        case eExpressionColumnNameOnly :
        case eExpressionColumnAttributes :
        case eExpressionValueBind :
        case eExpressionRequestSelector :
        case eExpressionOrderByASC :
        case eExpressionOrderByDESC :
        case eExpressionTypeValue :
        case eExpressionTypeValueQuoteString :
        case eExpressionTypeSQL :
        case eOperationOr :
        case eOperationAnd :
        case eOperationNot :
        case eOperationUpper :
        case eOperationUpperOrEqual :
        case eOperationLower :
        case eOperationLowerOrEqual :
        case eOperationEqual :
        case eOperationLike :
        case eOperationAs :
        case eOperationAsSelect :
        case eOperationAsJoin :
        case eOperationDifferent :
        case eOperationAssignmentInsert :
        case eOperationAssignmentUpdate :
        case eOperationAndJoin :
        case eOperationJoin :
        case eOperationInnerJoin :
        case eOperationLeftJoin :
        case eOperationRightJoin :
        case eOperationFullJoin :
        case eOperationIn :
        case eOperationCast :
        case eOperationIsNull :
        case eOperationIsNotNull :
        case eOperationMult :
        case eOperationDiv :
        case eOperationAdd :
        case eOperationSub :
        case eOperationAddComma :
        case eOperationDistinct :
        {   // Don't care for other values
            break;
        }
    }

    return bRet;
}

const wxETKSQLite3Expression & wxETKSQLite3Expression::Add(const wxETKSQLite3Expression &_rExpression)
{
    // Used to change operation type on AS depending if it is used on selector or somewhere else
    eExpressionOrOperationType expressionToAddType = (_rExpression.GetExpressionOrOperationType() == eOperationAs)
                                                        ? eOperationAsSelect
                                                        : _rExpression.GetExpressionOrOperationType();

    switch (GetExpressionOrOperationType())
    {
        case eOperationAsJoin :
        {
            wxASSERT_MSG(false,wxT("Cannot add value to AsJoin operation"));
            break;
        }
        case eExpressionTypeNothing :
        {
            // Current expression is empty: 1st Assignment
            (*this) = _rExpression;
            SetExpressionOrOperationType(expressionToAddType); // To change eOperationAs to eOperationAsSelect if needed
            break;
        }
        case eExpressionOrderByASC :
        case eExpressionOrderByDESC :
        {
            // Order by already assign with OrderBy: only OrderBy could be added to OrderBy!!
            wxASSERT(_rExpression.GetExpressionOrOperationType()==eExpressionOrderByASC || _rExpression.GetExpressionOrOperationType()==eExpressionOrderByDESC);
            if (m_pExpression1 == NULL)
            {
                m_pExpression1 = new wxETKSQLite3Expression(_rExpression);
                m_pExpression1->SetExpressionOrOperationType(expressionToAddType); // To change eOperationAs to eOperationAsSelect if needed
            }
            else
            {   // Go more down recursivly,  to put after all expression, to let this one after the last
                // OrderBy, to make SQL request with this at the end of the other, the order is important for OrderBy
                m_pExpression1->Add(_rExpression);
            }
            break;
        }
        case eOperationAddComma :
        {
            // Go more down recursivly,  to put after all expression, to let this one after the last
            // one, to request SQL order.
            m_pExpression2->Add(_rExpression);
            break;
        }
        case eOperationAssignmentInsert :
        {   // All columns name to Expression 1 tree (left tree)
            // All assign to expression 2 tree (right tree)
            wxASSERT(m_pExpression1 != NULL);
            wxASSERT(m_pExpression2 != NULL);
            wxASSERT(_rExpression.m_pExpression1->GetExpressionOrOperationType() == eExpressionColumnNameOnly);
            m_pExpression1->Add(*_rExpression.m_pExpression1);
            m_pExpression2->Add(*_rExpression.m_pExpression2);
            break;
        }
        case eOperationAssignmentUpdate :
        {
            wxASSERT(m_pExpression1 != NULL);
            wxASSERT(m_pExpression2 != NULL);
            m_pExpression2->Add(_rExpression);
            break;
        }
        case eOperationAs :
        case eOperationAsSelect :
        case eExpressionColumnNameFull :
        case eExpressionColumnNameOnly :
        case eExpressionColumnAttributes :
        case eExpressionValueBind :
        case eExpressionRequestSelector :  // Used ?
        case eExpressionTypeValue :
        case eExpressionTypeValueQuoteString :
        case eExpressionTypeSQL :
        case eOperationOr :
        case eOperationAnd :
        case eOperationNot :
        case eOperationUpper :
        case eOperationUpperOrEqual :
        case eOperationLower :
        case eOperationLowerOrEqual :
        case eOperationEqual :
        case eOperationLike :
        case eOperationDifferent :
        case eOperationCount :
        case eOperationMin :
        case eOperationMax :
        case eOperationSum :
        case eOperationAndJoin :
        case eOperationJoin :
        case eOperationInnerJoin :
        case eOperationLeftJoin :
        case eOperationRightJoin :
        case eOperationFullJoin :
        case eOperationIn :
        case eOperationCast :
        case eOperationIsNull :
        case eOperationIsNotNull :
        case eOperationMult :
        case eOperationDiv :
        case eOperationAdd :
        case eOperationSub :
        case eOperationDistinct :
        {   // Else add between each element
            eExpressionOrOperationType expressionType = IsJoin() ? eOperationAndJoin : eOperationAddComma;
            if (m_pExpression1 == NULL)
            {
                m_pExpression1 = new wxETKSQLite3Expression(*this);
                m_pExpression2 = new wxETKSQLite3Expression(_rExpression);
                m_pExpression2->SetExpressionOrOperationType(expressionToAddType); // To change eOperationAs to eOperationAsSelect if needed
                SetExpressionOrOperationType(expressionType);
            }
            else
            {   // Expression 1 already exists
                wxETKSQLite3Expression * exprTemp = new wxETKSQLite3Expression(*this);
                delete m_pExpression1;
                m_pExpression1 = exprTemp;

                if (m_pExpression2 != NULL)
                {
                    delete m_pExpression2;
                }

                m_pExpression2 = new wxETKSQLite3Expression(_rExpression);
                m_pExpression2->SetExpressionOrOperationType(expressionToAddType); // To change eOperationAs to eOperationAsSelect if needed
                SetExpressionOrOperationType(expressionType);
            }
            break;
        }
    }
    return *this;
}

const wxETKSQLite3Expression & wxETKSQLite3Expression::Add(const wxETKSQLite3Criterion &_rCriterion)
{
    wxASSERT_MSG(_rCriterion.GetRequestType() == wxETKSQLite3Criterion::eRequestTypeSelect,wxT("Adding criterion to expression MUST be a select criterion!"));
    if (_rCriterion.GetRequestType() == wxETKSQLite3Criterion::eRequestTypeSelect)
    {
        return Add(wxETKSQLite3Expression(wxETKSQLite3RequestSelector(_rCriterion)));
    }
    return *this;
}

wxETKSQLite3Expression & wxETKSQLite3Expression::operator<<(const wxETKSQLite3Expression &_rExpression)
{
    Add(_rExpression);
    return *this;
}

wxString wxETKSQLite3Expression::GetAsStringForOperator(wxString strOperator) const
{
    wxString strReturn;

    if (m_pExpression1==NULL || m_pExpression1->GetExpressionOrOperationType() == eExpressionTypeNothing)
    {
        if (m_pExpression2!=NULL && m_pExpression2->GetExpressionOrOperationType() != eExpressionTypeNothing)
        {   // Expression 1 is empty and expresssion 2 is empty
            strReturn = m_pExpression2->GetAsString();
        }
    }
    else if (m_pExpression2==NULL || m_pExpression2->GetExpressionOrOperationType() == eExpressionTypeNothing)
    {   // Expression is not empty ans 2 is empty
        strReturn = m_pExpression1->GetAsString();
    }
    else
    {   // Both Expression 1 and 2 are not empty
        strReturn.Printf(_T("(%s %s %s)"),m_pExpression1->GetAsString().c_str(),strOperator.c_str(),m_pExpression2->GetAsString().c_str());
    }

    return strReturn;
}

wxString wxETKSQLite3Expression::GetAsString() const
{
    wxString strOperatorAsString;

    switch (GetExpressionOrOperationType())
    {
        case eExpressionTypeNothing :
        {   // Nothing to do
            break;
        }
        case eExpressionOrderByASC :
        case eExpressionOrderByDESC :
        {
            strOperatorAsString = wxETKSQLite3Value::GetAsString();
            if (m_pExpression1 != NULL)
            {   // Add all Orderby
                strOperatorAsString += _T(",") + m_pExpression1->GetAsString();
            }
            break;
        }
        case eOperationAddComma :
        {
            strOperatorAsString.Printf(_T("%s,%s"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationOr :
        {
            strOperatorAsString = GetAsStringForOperator(_T("OR"));
            break;
        }
        case eOperationAnd :
        {
            strOperatorAsString = GetAsStringForOperator(_T("AND"));
            break;
        }
        case eOperationUpper :
        {
            strOperatorAsString.Printf(_T("%s > %s"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationUpperOrEqual :
        {
            strOperatorAsString.Printf(_T("%s >= %s"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationLower :
        {
            strOperatorAsString.Printf(_T("%s < %s"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationLowerOrEqual :
        {
            strOperatorAsString.Printf(_T("%s <= %s"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationEqual :
        {
            strOperatorAsString.Printf(_T("%s = %s"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationLike :
        {
            strOperatorAsString.Printf(_T("%s LIKE %s"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationAs :
        {   // The name hase changed, used only this one into expression
            strOperatorAsString.Printf(_T("%s"),m_pExpression1->GetAsString().c_str());
            break;
        }
        case eOperationAsSelect :
        {   // Into select, generate AS into expression
            strOperatorAsString.Printf(_T("(%s) AS %s"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationAsJoin :
        {   // Generate after the join keyword, when using alias, must generate 2 tables names
            strOperatorAsString.Printf(_T("%s %s"),GetTableName().c_str(),m_pExpression1->GetTableName().c_str());
            break;
        }
        case eOperationDifferent :
        {
            strOperatorAsString.Printf(_T("%s <> %s"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationNot :
        {
            strOperatorAsString.Printf(_T("NOT (%s)"),m_pExpression1->GetAsString().c_str());
            break;
        }
        case eOperationMult :
        {
            strOperatorAsString.Printf(_T("(%s * %s)"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationDiv :
        {
            strOperatorAsString.Printf(_T("(%s / %s)"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationAdd :
        {
            strOperatorAsString.Printf(_T("(%s + %s)"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationSub :
        {
            strOperatorAsString.Printf(_T("(%s - %s)"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eExpressionTypeValue :
        case eExpressionTypeValueQuoteString :
        case eExpressionTypeSQL : // Must contains one value type expression (with column name) + One SQL formatted expression
        case eExpressionRequestSelector :
        case eExpressionColumnNameFull :
        {
            strOperatorAsString = wxETKSQLite3Value::GetAsString();
            break;
        }
        case eExpressionColumnNameOnly :
        {
            strOperatorAsString = wxETKSQLite3Value::GetColumnName();
            break;
        }
        case eOperationJoin :
        {   // Must contains two expression of column name type
            wxASSERT(m_pExpression1 != NULL);
            wxASSERT(m_pExpression2 != NULL);
            wxASSERT(m_pExpression1->GetExpressionOrOperationType() == eExpressionColumnNameFull);
            wxASSERT(m_pExpression2->GetExpressionOrOperationType() == eExpressionColumnNameFull);

            strOperatorAsString.Printf(_T(" JOIN %s ON %s = %s"),m_pExpression2->GetTableName().c_str(),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationInnerJoin :
        case eOperationLeftJoin :
        case eOperationRightJoin :
        case eOperationFullJoin :
        {   // Must contains two expression of column name type
            wxString strJoinOperator;
            switch (GetExpressionOrOperationType())
            {
                case eOperationInnerJoin :
                    strJoinOperator = _T("INNER JOIN");
                    break;
                case eOperationLeftJoin :
                    strJoinOperator = _T("LEFT JOIN");
                    break;
                case eOperationRightJoin :
                    strJoinOperator = _T("RIGHT JOIN");
                    break;
                case eOperationFullJoin :
                    strJoinOperator = _T("FULL JOIN");
                    break;
                default :
                    wxFAIL; // Not allowed
            }

            wxASSERT(m_pExpression1 != NULL);
            wxASSERT(m_pExpression2 != NULL);
            if (   m_pExpression1->GetExpressionOrOperationType() == eExpressionColumnNameFull
                && m_pExpression2->GetExpressionOrOperationType() == eExpressionColumnNameFull
               )
            {   // Take table name
                strOperatorAsString.Printf(_T(" %s %s ON %s = %s"),strJoinOperator.c_str(),m_pExpression2->GetTableName().c_str(),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            }
            else
            {
                strOperatorAsString.Printf(_T(" %s %s ON %s"),strJoinOperator.c_str(),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            }
            break;
        }
        case eOperationIn:
        {   // Must contains one expression of column name type + one SQL formatted expression
            wxASSERT(m_pExpression1 != NULL);
            wxASSERT(m_pExpression2 != NULL);
            wxASSERT(m_pExpression1->GetExpressionOrOperationType() == eExpressionColumnNameFull);
            wxASSERT(m_pExpression2->GetExpressionOrOperationType() == eExpressionTypeSQL);
            strOperatorAsString.Printf(_T(" %s IN (%s)"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationCount :
        {
            if (m_pExpression1 != NULL)
            {
                if (m_pExpression1->GetExpressionOrOperationType() == eExpressionColumnNameFull)
                {
                    strOperatorAsString.Printf(_T("COUNT (%s)"),m_pExpression1->GetColumnName().c_str());
                }
                else
                {
                    strOperatorAsString.Printf(_T("COUNT (%s)"),m_pExpression1->GetAsString().c_str());
                }
            }
            else
            {
                wxString strTmpString = wxETKSQLite3Value::GetAsString();
                if (strTmpString.length() == 0 || wxETKSQLite3Value::GetColumnName()==_T("*"))
                {   // If nothing into count parameters, take (*)
                    strOperatorAsString = _T("COUNT (*)");
                }
                else
                {
                    strOperatorAsString.Printf(_T("COUNT (%s)"),strTmpString.c_str());
                }
            }
            break;
        }
        case eOperationMin:
        {
            wxString strTmpString = wxETKSQLite3Value::GetAsString();
            if (strTmpString.length() == 0)
            {   // If nothing into min parameters, take (*)
                strOperatorAsString = _T("MIN (*)");
            }
            else
            {
                strOperatorAsString.Printf(_T("MIN (%s)"),strTmpString.c_str());
            }
            break;
        }
        case eOperationMax:
        {
            wxString strTmpString = wxETKSQLite3Value::GetAsString();
            if (strTmpString.length() == 0)
            {   // If nothing into max parameters, take (*)
                strOperatorAsString = _T("MAX (*)");
            }
            else
            {
                strOperatorAsString.Printf(_T("MAX (%s)"),strTmpString.c_str());
            }
            break;
        }
        case eOperationSum:
        {
            wxString strTmpString = wxETKSQLite3Value::GetAsString();
            if (strTmpString.length() == 0)
            {   // If nothing into sum parameters, take (*)
                strOperatorAsString = _T("SUM (*)");
            }
            else
            {
                strOperatorAsString.Printf(_T("SUM (%s)"),strTmpString.c_str());
            }
            break;
        }
        case eOperationCast :
        {
            strOperatorAsString.Printf(_T("CAST(%s AS %s)"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationIsNull :
        {
            strOperatorAsString.Printf(_T("(%s IS NULL)"),m_pExpression1->GetAsString().c_str());
            break;
        }
        case eOperationIsNotNull :
        {
            strOperatorAsString.Printf(_T("(%s IS NOT NULL)"),m_pExpression1->GetAsString().c_str());
            break;
        }
        case eOperationDistinct :
        {
            strOperatorAsString.Printf(_T("DISTINCT %s"),m_pExpression1->GetAsString().c_str());
            break;
        }
        case eExpressionColumnAttributes :
        case eExpressionValueBind :
        {
            strOperatorAsString = wxETKSQLite3Value::GetAsString();
            break;
        }
        case eOperationAssignmentInsert :
        {   // Must contains one expression of column name type + one value or binded data
            wxASSERT(m_pExpression1 != NULL);
            wxASSERT(m_pExpression2 != NULL);
            strOperatorAsString.Printf(_T("(%s) VALUES (%s)"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationAssignmentUpdate :
        {   // Must contains one expression of column name type + one value or binded data
            wxASSERT(m_pExpression1 != NULL);
            wxASSERT(m_pExpression2 != NULL);
            // Cannot easily add the SET keyword, don't put it
            strOperatorAsString.Printf(_T("%s=%s"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
        case eOperationAndJoin :
        {   // Must contains 2 expressions of JOIN type
            wxASSERT(m_pExpression1 != NULL && m_pExpression1->IsJoin());
            wxASSERT(m_pExpression2 != NULL && m_pExpression2->IsJoin());
            // Cannot easily add the SET keyword, don't put it
            strOperatorAsString.Printf(_T("%s%s"),m_pExpression1->GetAsString().c_str(),m_pExpression2->GetAsString().c_str());
            break;
        }
    }
    return strOperatorAsString;
}

void wxETKSQLite3Expression::BindTo(wxSQLite3Statement &_rstmt,int &_riIndex,bool _bForInsertRequest) const
{
    wxETKSQLite3Value::BindTo(_rstmt,_riIndex,_bForInsertRequest);

    if (m_pExpression1 != NULL)
    {
        m_pExpression1->BindTo(_rstmt,_riIndex,_bForInsertRequest);
    }
    if (m_pExpression2 != NULL)
    {
        m_pExpression2->BindTo(_rstmt,_riIndex,_bForInsertRequest);
    }
}

wxString wxETKSQLite3Expression::FormatRequest() const
{
    return GetAsString();
}

wxETKSQLite3Expression::tdStringList & wxETKSQLite3Expression::UpdateTablesList(tdStringList &_rlstTablesList) const
{
    switch (GetExpressionOrOperationType())
    {
        case eOperationMin :
        case eOperationMax :
        case eOperationSum :
        case eOperationCount :
        {   // Count elements: if the count/min/max/sum is made on column, put the table name into table lists
            if (!wxETKSQLite3Value::IsValueEmpty())
            {   // If there is a value for the count/min/max/sum it is because it was made with column class: It should have TABLE.COLUMN
                // Must find the table name and add it (if not already exist) int _rlstTablesList
                // Be careful: call the function from wxETKSQLite3Value class and not the derived class (not use derived virtual methode)
                wxString strValueString = wxETKSQLite3Value::GetAsString();
                int iFoundPoint = strValueString.Find(_T('.'));
                if (iFoundPoint != wxNOT_FOUND)
                {   // Find table name
                    // Add element (if exist) and initialize the value to true, if the element already exists, nothing is modified
                    _rlstTablesList.insert(tdStringList::value_type(strValueString.Left(iFoundPoint),true));
                }
            }
            break;
        }
        case eOperationJoin :
        case eOperationInnerJoin :
        case eOperationLeftJoin :
        case eOperationRightJoin :
        case eOperationFullJoin :
        case eOperationAsSelect :
        {   // Remove from table list the table name into the second parameter of the LEFT JOIN, else the from will
            // contains both table name and generate incorrect expression
            // Take the column name
            wxString strNameTable2 = m_pExpression2->GetAsString();
            int iFoundPoint = strNameTable2.Find(_T('.'));
            if (iFoundPoint != wxNOT_FOUND)
            {
                strNameTable2 = strNameTable2.Left(iFoundPoint);
                // Do not us the insert function but [] operator, it create the element if not exists and return element.
                _rlstTablesList[strNameTable2] = false; // It is now into the table list with false attribute
            }
            return _rlstTablesList; // Do not call UpdateTablesList on m_pExpression1 and m_pExpression2 else
                                    // this function will add it and I just removed it
        }
        case eExpressionColumnNameFull :
        case eExpressionColumnNameOnly :
        {   // Add all columns table
            // Take columns names
            wxString strNameTable = wxETKSQLite3Value::GetTableName();
            // Add element (if exists) and init it with true value, if the element exits, nothing is changed
            _rlstTablesList.insert(tdStringList::value_type(strNameTable,true));
            break;
        }
        case eExpressionRequestSelector :
        case eExpressionTypeNothing :
        case eExpressionColumnAttributes :
        case eExpressionValueBind :
        case eExpressionOrderByASC :
        case eExpressionOrderByDESC :
        case eExpressionTypeValue :
        case eExpressionTypeValueQuoteString :
        case eExpressionTypeSQL :
        case eOperationOr :
        case eOperationAnd :
        case eOperationNot :
        case eOperationUpper :
        case eOperationUpperOrEqual :
        case eOperationLower :
        case eOperationLowerOrEqual :
        case eOperationEqual :
        case eOperationLike :
        case eOperationAs :
        case eOperationAsJoin :
        case eOperationDifferent :
        case eOperationAssignmentInsert :
        case eOperationAssignmentUpdate :
        case eOperationAndJoin :
        case eOperationIn :
        case eOperationCast :
        case eOperationIsNull :
        case eOperationIsNotNull :
        case eOperationMult :
        case eOperationDiv :
        case eOperationAdd :
        case eOperationSub :
        case eOperationAddComma :
        case eOperationDistinct :
        {   // Don't care for other values
            break;
        }
    }
    if (m_pExpression1 != NULL)
    {
        m_pExpression1->UpdateTablesList(_rlstTablesList);
    }
    if (m_pExpression2 != NULL)
    {
        m_pExpression2->UpdateTablesList(_rlstTablesList);
    }

    return _rlstTablesList;
}

const wxETKSQLite3Expression * wxETKSQLite3Expression::GetExpression1() const
{
    return m_pExpression1;
}

const wxETKSQLite3Expression * wxETKSQLite3Expression::GetExpression2() const
{
    return m_pExpression2;
}

bool wxETKSQLite3Expression::IsJoin() const
{
    switch(GetExpressionOrOperationType())
    {
        case eOperationAndJoin :
        case eOperationJoin :
        case eOperationInnerJoin :
        case eOperationLeftJoin :
        case eOperationRightJoin :
        case eOperationFullJoin :
        {
            return true; // Join
        }
        case eExpressionTypeNothing :
        case eExpressionColumnNameFull :
        case eExpressionColumnNameOnly :
        case eExpressionColumnAttributes :
        case eExpressionValueBind :
        case eExpressionRequestSelector :
        case eExpressionOrderByASC :
        case eExpressionOrderByDESC :
        case eExpressionTypeValue :
        case eExpressionTypeValueQuoteString :
        case eExpressionTypeSQL :
        case eOperationOr :
        case eOperationAnd :
        case eOperationNot :
        case eOperationUpper :
        case eOperationUpperOrEqual :
        case eOperationLower :
        case eOperationLowerOrEqual :
        case eOperationEqual :
        case eOperationLike :
        case eOperationAs :
        case eOperationAsSelect :
        case eOperationAsJoin : // Not used to detect join sql expression
        case eOperationDifferent :
        case eOperationCount :
        case eOperationMin :
        case eOperationMax :
        case eOperationSum :
        case eOperationAssignmentInsert :
        case eOperationAssignmentUpdate :
        case eOperationIn :
        case eOperationCast :
        case eOperationIsNull :
        case eOperationIsNotNull :
        case eOperationMult :
        case eOperationDiv :
        case eOperationAdd :
        case eOperationSub :
        case eOperationAddComma :
        case eOperationDistinct :
        {   // Other values are false
            break;
        }
    }

    return false;
}

bool wxETKSQLite3Expression::IsNull() const
{
    return GetExpressionOrOperationType() == eExpressionTypeNothing;
}

#ifdef __WXDEBUG__
wxString wxETKSQLite3Expression::GetStringContentDescription() const
{
    wxString strDescription;
    ConstructContentDescription(strDescription);
    return strDescription;
}

int wxETKSQLite3Expression::ConstructContentDescription(wxString &_strDescription) const
{
    wxString strThisDescription;

    int iLenThisDescription = ConstructContentThisDescription(strThisDescription);

    if (this == NULL)
    {
        _strDescription = strThisDescription;
    }
    else
    {
        // Left description
        wxString strLeftDescription;
        int iLenLeftDescription = GetExpression1()->ConstructContentDescription(strLeftDescription);

        // Right description
        wxString strRightDescription;
        int iLenRightDescription = GetExpression2()->ConstructContentDescription(strRightDescription);

        wxString strPadAllLeftLeft,strPadAllLeftRight;
        wxString strPadAllRightLeft,strPadAllRightRight;
        int iLenDescription = wxMax(iLenLeftDescription + iLenRightDescription + 3,iLenThisDescription);
        if (iLenLeftDescription + iLenRightDescription + 3 < iLenThisDescription)
        {
            strPadAllLeftLeft.Pad(((iLenThisDescription / 2) - (iLenLeftDescription    ))/2,wxT(' '));
            strPadAllLeftRight.Pad(((iLenThisDescription / 2) - (iLenLeftDescription + 1))/2,wxT(' '));

            strPadAllRightLeft.Pad(((iLenThisDescription / 2) - (iLenRightDescription    ))/2,wxT(' '));
            strPadAllRightRight.Pad(((iLenThisDescription / 2)- (iLenRightDescription + 1))/2,wxT(' '));
        }
        // First, pad strThisDescription with iLenDescription
        _strDescription = Pad(strThisDescription,iLenDescription);
        // Construct tree lines
        _strDescription += _T("\n") + wxString(_T(' '),(iLenDescription - 1) / 2)
                                      + _T('|')
                                      + wxString(_T(' '),iLenDescription / 2)
                         + _T("\n") + wxString(_T(' '),strPadAllLeftLeft.Len() + (iLenLeftDescription - 1) / 2)
                                      + wxString(_T('-'),2 + (iLenLeftDescription / 2) + strPadAllLeftRight.Len() + 3 + strPadAllRightLeft.Len() + (iLenRightDescription - 1) / 2)
                                      + wxString(_T(' '),iLenRightDescription / 2 + strPadAllRightRight.Len())
                         + _T("\n") + wxString(_T(' '),strPadAllLeftLeft.Len() + (iLenLeftDescription - 1) / 2)
                                    + _T('|')
                                    + wxString(_T(' '),(iLenLeftDescription / 2) + strPadAllLeftRight.Len() + 3 + strPadAllRightLeft.Len() + (iLenRightDescription - 1) / 2)
                                    + _T('|')
                                    + wxString(_T(' '),iLenRightDescription / 2 + strPadAllRightRight.Len());
        // Make fusion between 2 expressions
        wxStringTokenizer tkzLeft(strLeftDescription, _T("\n"));
        wxStringTokenizer tkzRight(strRightDescription, _T("\n"));
        while (tkzLeft.HasMoreTokens() || tkzRight.HasMoreTokens())
        {
            wxString strTokenLeft,strTokenRight;
            if (tkzLeft.HasMoreTokens())
            {
                strTokenLeft = tkzLeft.GetNextToken();
            }
            else
            {   // Depth of left expression is lower than right
                strTokenLeft.Pad(iLenLeftDescription,_T(' '));
            }
            if (tkzRight.HasMoreTokens())
            {
                strTokenRight = tkzRight.GetNextToken();
            }
            else
            {   // Depth of right expression is lower than left
                strTokenRight.Pad(iLenRightDescription,_T(' '));
            }
            _strDescription += _T('\n') + strPadAllLeftLeft + strTokenLeft + strPadAllLeftRight +wxString(_T(' '),3) + strPadAllRightLeft +strTokenRight + strPadAllRightRight;
        }
        iLenThisDescription = iLenDescription;
    }

    return iLenThisDescription;
}

int wxETKSQLite3Expression::ConstructContentThisDescription(wxString &_strDescription) const
{
    int iMaxLength;
    if (this == NULL) // I know it is ugly :-) Just as debug, simplify tree explorer
    {   // If accessed on one member on this class, the software crash (access violation)
        _strDescription = _T("(null) ");
        iMaxLength = 7;
    }
    else
    {
        wxString strVariantID    = wxString::Format(_T("[ID=%lu]"),m_ulCurrentId);
        wxString strDescription  = GetOperatorString();
        wxString strVariantType  = _T("(Type=") + GetVariantDebug().GetType() + _T(")");
        wxString strVariantValue = _T("\"") + GetVariantDebug().GetString() + _T("\"");
        strVariantValue.Replace(_T("\t"),_T("\\t"));
        strVariantValue.Replace(_T("\r"),_T("\\r"));
        strVariantValue.Replace(_T("\n"),_T("\\n"));

        iMaxLength = wxMax(strVariantID.Len(),strDescription.Len());
        iMaxLength = wxMax(iMaxLength,(int) strVariantType.Len());
        iMaxLength = wxMax(iMaxLength,(int) strVariantValue.Len());
        if (iMaxLength % 2 == 0)
        {
            ++iMaxLength;
        }
        strVariantID.Pad((iMaxLength - strVariantID.Len())/2,_T(' '),false);  // Pad left
        strVariantID.Pad(iMaxLength - strVariantID.Len(),_T(' '),true);   // Pad right
        strDescription.Pad((iMaxLength - strDescription.Len())/2,_T(' '),false);  // Pad left
        strDescription.Pad(iMaxLength - strDescription.Len(),_T(' '),true);   // Pad right
        strVariantType.Pad((iMaxLength - strVariantType.Len())/2,_T(' '),false);  // Pad left
        strVariantType.Pad(iMaxLength - strVariantType.Len(),_T(' '),true);   // Pad right
        strVariantValue.Pad((iMaxLength - strVariantValue.Len())/2,_T(' '),false); // Pad left
        strVariantValue.Pad(iMaxLength - strVariantValue.Len(),_T(' '),true);  // Pad right
        _strDescription = strVariantID + _T("\n") + strDescription + _T("\n") + strVariantType + _T("\n") + strVariantValue;
    }

    return iMaxLength;
}

wxString wxETKSQLite3Expression::GetOperatorString() const
{
    wxString strType;
    switch (GetExpressionOrOperationType())
    {
        case eExpressionTypeNothing :
        {
            strType = _T("eExpressionTypeNothing");
            break;
        }
        case eExpressionColumnNameFull :
        {
            strType = _T("eExpressionColumnNameFull");
            break;
        }
        case eExpressionColumnNameOnly :
        {
            strType = _T("eExpressionColumnNameOnly");
            break;
        }
        case eExpressionColumnAttributes :
        {
            strType = _T("eExpressionColumnAttributes");
            break;
        }
        case eExpressionValueBind :
        {
            strType = _T("eExpressionValueBind");
            break;
        }
        case eExpressionRequestSelector :
        {
            strType = _T("eExpressionRequestSelector");
            break;
        }
        case eExpressionOrderByASC :
        {
            strType = _T("eExpressionOrderByASC");
            break;
        }
        case eExpressionOrderByDESC :
        {
            strType = _T("eExpressionOrderByDESC");
            break;
        }
        case eExpressionTypeValue :
        {
            strType = _T("eExpressionTypeValue");
            break;
        }
        case eExpressionTypeValueQuoteString :
        {
            strType = _T("eExpressionTypeValueQuoteString");
            break;
        }
        case eExpressionTypeSQL :
        {
            strType = _T("eExpressionTypeSQL");
            break;
        }
        case eOperationOr :
        {
            strType = _T("eOperationOr");
            break;
        }
        case eOperationAnd :
        {
            strType = _T("eOperationAnd");
            break;
        }
        case eOperationNot :
        {
            strType = _T("eOperationNot");
            break;
        }
        case eOperationUpper :
        {
            strType = _T("eOperationUpper");
            break;
        }
        case eOperationUpperOrEqual :
        {
            strType = _T("eOperationUpperOrEqual");
            break;
        }
        case eOperationLower :
        {
            strType = _T("eOperationLower");
            break;
        }
        case eOperationLowerOrEqual :
        {
            strType = _T("eOperationLowerOrEqual");
            break;
        }
        case eOperationEqual :
        {
            strType = _T("eOperationEqual");
            break;
        }
        case eOperationLike :
        {
            strType = _T("eOperationLike");
            break;
        }
        case eOperationAs :
        {
            strType = _T("eOperationAs");
            break;
        }
        case eOperationAsSelect :
        {
            strType = _T("eOperationAsSelect");
            break;
        }
        case eOperationAsJoin :
        {
            strType = _T("eOperationAsJoin");
            break;
        }
        case eOperationDifferent :
        {
            strType = _T("eOperationDifferent");
            break;
        }
        case eOperationCount :
        {
            strType = _T("eOperationCount");
            break;
        }
        case eOperationMin :
        {
            strType = _T("eOperationMin");
            break;
        }
        case eOperationMax :
        {
            strType = _T("eOperationMax");
            break;
        }
        case eOperationSum :
        {
            strType = _T("eOperationSum");
            break;
        }
        case eOperationAssignmentInsert :
        {
            strType = _T("eOperationAssignmentInsert");
            break;
        }
        case eOperationAssignmentUpdate :
        {
            strType = _T("eOperationAssignmentUpdate");
            break;
        }
        case eOperationAndJoin :
        {
            strType = _T("eOperationAndJoin");
            break;
        }
        case eOperationJoin :
        {
            strType = _T("eOperationJoin");
            break;
        }
        case eOperationInnerJoin :
        {
            strType = _T("eOperationInnerJoin");
            break;
        }
        case eOperationLeftJoin :
        {
            strType = _T("eOperationLeftJoin");
            break;
        }
        case eOperationRightJoin :
        {
            strType = _T("eOperationRightJoin");
            break;
        }
        case eOperationFullJoin :
        {
            strType = _T("eOperationFullJoin");
            break;
        }
        case eOperationIn :
        {
            strType = _T("eOperationIn");
            break;
        }
        case eOperationCast :
        {
            strType = _T("eOperationCast");
            break;
        }
        case eOperationIsNull :
        {
            strType = _T("eOperationIsNull");
            break;
        }
        case eOperationIsNotNull :
        {
            strType = _T("eOperationIsNotNull");
            break;
        }
        case eOperationMult :
        {
            strType = _T("eOperationMult");
            break;
        }
        case eOperationDiv :
        {
            strType = _T("eOperationDiv");
            break;
        }
        case eOperationAdd :
        {
            strType = _T("eOperationAdd");
            break;
        }
        case eOperationSub :
        {
            strType = _T("eOperationSub");
            break;
        }
        case eOperationAddComma :
        {
            strType = _T("eOperationAddComma");
            break;
        }
        case eOperationDistinct :
        {
            strType = _T("eOperationDistinct");
            break;
        }
    }
    return strType;
}

wxString wxETKSQLite3Expression::Pad(const wxString _strString,int iLenString)
{
    wxString strPaddedString;
    wxStringTokenizer tkz(_strString, _T("\n")); //wxTOKEN_RET_EMPTY_ALL
    while (tkz.HasMoreTokens())
    {
        wxString strToken = tkz.GetNextToken();

        wxASSERT(((int) strToken.Len())<=iLenString); // With algorithm, must never happened !
        strToken.Pad((iLenString - strToken.Len()    )/2,_T(' '),false); // Pad left
        strToken.Pad(iLenString - strToken.Len(),_T(' '),true);  // Pad right
        strPaddedString += (strPaddedString.IsEmpty() ? _T("") : _T("\n")) + strToken;
    }

    return strPaddedString;
}

#endif // __WXDEBUG__

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                        wxETKSQLite3Criterion                                          //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

wxETKSQLite3Criterion::wxETKSQLite3Criterion(eRequestType _eRequestType)
    : m_bDistinct(false)
    , m_eRequestType(_eRequestType)
{
}

wxETKSQLite3Criterion::wxETKSQLite3Criterion(const wxETKSQLite3Expression& _rExpression)
{
    operator=(_rExpression);
}

wxETKSQLite3Criterion::wxETKSQLite3Criterion(const wxETKSQLite3Criterion& _rCriterion)
{
    operator=(_rCriterion);
}

wxETKSQLite3Criterion::wxETKSQLite3Criterion(const wxETKSQLite3RequestSelector& _rSelector)
{
    operator=(_rSelector);
}

wxETKSQLite3Criterion::~wxETKSQLite3Criterion()
{
}

wxETKSQLite3Criterion::eRequestType wxETKSQLite3Criterion::GetRequestType() const
{
    return m_eRequestType;
}

void wxETKSQLite3Criterion::SetRequestType(wxETKSQLite3Criterion::eRequestType _eRequestType)
{
    m_eRequestType = _eRequestType;
}

const wxETKSQLite3Criterion& wxETKSQLite3Criterion::operator=(const wxETKSQLite3Expression& _rExpresion)
{
    if (_rExpresion.IsJoin())
    {
        SetJoin(GetJoin() && _rExpresion);
    }
    else
    {
        wxETKSQLite3Expression::operator=(_rExpresion); // Call = on base class
    }
    SetDistinct(false); // Put distinct value to default
    SetOrderBy();       // Put order by value to default

    return *this;
}

const wxETKSQLite3Criterion& wxETKSQLite3Criterion::operator=(const wxETKSQLite3Criterion& _rCriterion)
{
    m_eRequestType = _rCriterion.m_eRequestType;
    SetDistinct(_rCriterion.GetDistinct()); // Copy distinct
    SetOrderBy(_rCriterion.GetOrderBy());   // Copy order
    SetJoin(_rCriterion.GetJoin());         // Copy joins criterion
    SetWhere(_rCriterion.GetWhere());       // Copy Where criterion
    SetFrom(_rCriterion.GetFrom());         // Copy From criterion

    wxETKSQLite3Expression::operator=(_rCriterion); // Call = on base class
    return *this;
}

const wxETKSQLite3Criterion& wxETKSQLite3Criterion::operator=(const wxETKSQLite3RequestSelector& _rSelector)
{
    wxETKSQLite3Expression::operator=(_rSelector);
    return *this;
}

wxETKSQLite3Criterion wxETKSQLite3Criterion::operator&&(const wxETKSQLite3Criterion & _rCriterion) const
{
    wxETKSQLite3Criterion critere(wxETKSQLite3Expression::operator&&(_rCriterion));

    critere.SetDistinct(GetDistinct());                 // Copy distinct
    critere.SetOrderBy(GetOrderBy());                   // Copy order
    critere.SetJoin(GetJoin() && _rCriterion.GetJoin());// Copy joins criterion

    return critere;
}

wxETKSQLite3Criterion wxETKSQLite3Criterion::operator&&(const wxETKSQLite3Expression& _rExpresion) const
{
    wxETKSQLite3Criterion critere(wxETKSQLite3Expression::operator&&(_rExpresion));

    critere.SetDistinct(GetDistinct()); // Copy distinct
    critere.SetOrderBy(GetOrderBy());   // Copy order
    critere.SetJoin(GetJoin());         // Copy joins criterion

    return critere;
}

void wxETKSQLite3Criterion::Clear()
{
    wxETKSQLite3Expression::Clear();
    m_exprOrderBy.Clear();
    m_exprJoin.Clear();
    m_exprWhere.Clear();
    m_exprFrom.Clear();
    m_bDistinct = false;
}


wxETKSQLite3Criterion & wxETKSQLite3Criterion::AddOrderByAscending(const wxETKSQLite3Column& _rColumn)
{
    m_exprOrderBy.Add(wxETKSQLite3Expression(_rColumn.GetFullName(),eExpressionOrderByASC));
    return *this;
}

wxETKSQLite3Criterion & wxETKSQLite3Criterion::AddOrderByDescending(const wxETKSQLite3Column& _rColumn)
{
    m_exprOrderBy.Add(wxETKSQLite3Expression(_rColumn.GetFullName(),eExpressionOrderByDESC));
    return *this;
}

void wxETKSQLite3Criterion::AddOrderbyList(const wxETKSQLite3Expression& _rExpresion)
{
    if (!_rExpresion.IsNull())
    {
        if (m_exprOrderBy.IsNull())
        {
            m_exprOrderBy = _rExpresion;
        }
        else
        {   // If both have expressions, it is because the order by was done on criterion and
            // on to the selector
            wxFAIL;
        }
    }
    // Else nothing to do
}

wxETKSQLite3Criterion & wxETKSQLite3Criterion::Distinct()
{
    SetDistinct(true);
    return *this;
}

wxETKSQLite3Criterion::tdStringList & wxETKSQLite3Criterion::UpdateTablesList(tdStringList &_rlstTablesList) const
{
    wxETKSQLite3Expression::UpdateTablesList(_rlstTablesList);
    GetWhere().UpdateTablesList(_rlstTablesList);
    GetJoin().UpdateTablesList(_rlstTablesList);

    return _rlstTablesList;
}

void wxETKSQLite3Criterion::SetDistinct(bool _bDistinct)
{
    m_bDistinct = _bDistinct;
}

bool wxETKSQLite3Criterion::GetDistinct() const
{
    return m_bDistinct;
}

const wxETKSQLite3Expression & wxETKSQLite3Criterion::GetOrderBy() const
{
    return m_exprOrderBy;
}

void wxETKSQLite3Criterion::SetOrderBy(const wxETKSQLite3Expression &_rExprOrderBy)
{
    m_exprOrderBy = _rExprOrderBy;
}

void wxETKSQLite3Criterion::SetJoin(const wxETKSQLite3Expression &_rExprJoin)
{
    m_exprJoin = _rExprJoin;
}

const wxETKSQLite3Expression & wxETKSQLite3Criterion::GetJoin() const
{
    return m_exprJoin;
}

void wxETKSQLite3Criterion::SetWhere(const wxETKSQLite3Expression &_rExprWhere)
{
    // Current where MUST be NULL !! Else it is initialize twice with where criterion
    // wxASSERT_MSG(m_exprWhere.IsNull(),wxT("WHERE is initialized more than once!"));
    // => Could change the where into the request, the wxASSERT_MSG is removed!
    wxASSERT_MSG(   GetRequestType() == eRequestTypeDelete
                 || GetRequestType() == eRequestTypeUpdate
                 || GetRequestType() == eRequestTypeSelect ,wxT("Bad type request for WHERE initialization!"));

    m_exprWhere = _rExprWhere;
}

const wxETKSQLite3Expression & wxETKSQLite3Criterion::GetWhere() const
{
    return m_exprWhere;
}

void wxETKSQLite3Criterion::SetFrom(const wxETKSQLite3Expression &_rExprFrom)
{
    // Current from MUST be NULL !! Else it is initialize twice with where criterion
    wxASSERT_MSG(m_exprFrom.IsNull(),wxT("WHERE is initialized more than once!"));
    wxASSERT_MSG(   GetRequestType() == eRequestTypeDelete
                 || GetRequestType() == eRequestTypeUpdate
                 || GetRequestType() == eRequestTypeSelect ,wxT("Bad type request for WHERE initialization!"));

    m_exprFrom = _rExprFrom;
}

const wxETKSQLite3Expression & wxETKSQLite3Criterion::GetFrom() const
{
    return m_exprFrom;
}

wxString wxETKSQLite3Criterion::GetSQL() const
{
    tdStringList lstTables;     // Table list for from or insert
    wxString strSQLRequest;

    if (m_exprFrom.IsNull())
    {
        UpdateTablesList(lstTables);
    }
    else
    {
        if (m_exprFrom.GetExpressionOrOperationType() == eExpressionTypeValueQuoteString)
        {   // Programmer has just add like "Table1,Table2,Table3"
            wxString str = m_exprFrom.GetVariant().GetString();
            wxStringTokenizer tkzTables(m_exprFrom.GetVariant().GetString(), _T(","));
            while (tkzTables.HasMoreTokens())
            {
                // Add element (if exist) and initialize the value to true, if the element already exists, nothing is modified
                lstTables.insert(tdStringList::value_type(tkzTables.GetNextToken(),true));
            }
        }
        else
        {
            m_exprFrom.UpdateTablesList(lstTables);
        }
    }

    switch (m_eRequestType)
    {
        case eRequestTypeUndefined:
        {
            // return wxEmptyString, let strSQLRequest empty
            break;
        }
        case eRequestTypeInsert:
        {
            wxASSERT(GetWhere().IsNull());  // Must be NULL on insert
            wxASSERT(GetJoin().IsNull());   // Must be NULL on insert
            #ifdef __WXDEBUG__
            if (lstTables.size() != 1)
            {
                wxString strErrorMessage(wxString::Format(wxT("Insert request must have only ONE table, it have %d table(s)!"),(int) lstTables.size()));
                int iIndexTable = 0;
                for (tdStringList::iterator it = lstTables.begin(); it != lstTables.end(); ++it)
                {
                    strErrorMessage += wxString::Format(wxT("\nTable[%d] = '%s'"),iIndexTable,((*it).first).wx_str());
                    ++iIndexTable;
                }
                wxFAIL_MSG(strErrorMessage);
            }
            #endif
            strSQLRequest.Printf(   _T("INSERT INTO %s %s"),
                                    FormatSetAsString(lstTables).wx_str(),
                                    FormatRequest().wx_str()
                                );
            break;
        }
        case eRequestTypeDelete:
        {
            strSQLRequest.Printf(   _T("DELETE FROM %s"),
                                    FormatSetAsString(UpdateTablesList(lstTables)).c_str()
                                );

            // Request contains WHERE ?
            if (!GetWhere().IsNull())
            {
                strSQLRequest += wxString::Format(_T(" WHERE (%s)"),GetWhere().FormatRequest().c_str());
            }
            break;
        }
        case eRequestTypeUpdate:
        {
            wxASSERT_MSG(lstTables.size() == 1,wxT("Update request must have only ONE table!"));

            strSQLRequest.Printf(   _T("UPDATE %s SET %s"),
                                    FormatSetAsString(lstTables).wx_str(),
                                    FormatRequest().wx_str()
                                );

            // Request contains WHERE ?
            if (!GetWhere().IsNull())
            {
                strSQLRequest += wxString::Format(_T(" WHERE (%s)"),GetWhere().FormatRequest().c_str());
            }
            break;
        }
        case eRequestTypeSelect:
        {
            strSQLRequest.Printf(   _T("SELECT %s%s FROM %s"),
                                    GetDistinct() ? _T("DISTINCT ") : _T(""),
                                    wxETKSQLite3Expression::FormatRequest().c_str(),
                                    FormatSetAsString(lstTables).c_str()
                                );

            // Request contains JOIN ?
            if (!GetJoin().IsNull())
            {
                strSQLRequest += GetJoin().FormatRequest();
            }

            // Request contains WHERE ?
            if (!GetWhere().IsNull())
            {
                strSQLRequest += wxString::Format(_T(" WHERE (%s)"),GetWhere().FormatRequest().c_str());
            }

            // Request contains ORDER BY ?
            if (CanHaveOrderBy())
            {   // Only if it is not an operator like MIN/MAX/COUNT/ etc...
                strSQLRequest += GetOrderByAsString(); // Add ORDER BY to others if defined
            }

            break;
        }
    }

    return strSQLRequest;
}

void wxETKSQLite3Criterion::BindTo(wxSQLite3Statement &_rstmt,int &_riIndex) const
{
    wxETKSQLite3Expression::BindTo(_rstmt,_riIndex,m_eRequestType == eRequestTypeInsert);
    GetWhere().BindTo(_rstmt,_riIndex,m_eRequestType == eRequestTypeInsert);
    GetJoin().BindTo(_rstmt,_riIndex,m_eRequestType == eRequestTypeInsert);
}

wxString wxETKSQLite3Criterion::GetOrderByAsString() const
{
    wxString strOrderByRequest;

    if (!GetOrderBy().IsNull())
    {
        strOrderByRequest = _T(" ORDER BY ") + m_exprOrderBy.FormatRequest();
    }

    return strOrderByRequest;
}

bool wxETKSQLite3Criterion::IsNull() const
{
    return !GetDistinct() && GetOrderBy().IsNull() && wxETKSQLite3Expression::IsNull();
}

bool wxETKSQLite3Criterion::IsValid() const
{
    return !wxETKSQLite3Expression::IsNull();
}

wxString wxETKSQLite3Criterion::FormatSetAsString(const tdStringList &_rlstDatas)
{   // List elements of set, separate from each others with comma (',')
    return std::for_each(_rlstDatas.begin(),_rlstDatas.end(),wxETKSQLite3StringConcat()).m_strValue;
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

EXPORT_IMPORT wxETKSQLite3Expression operator==(const wxETKSQLite3Value &_rValue,const wxETKSQLite3Column &_rColumn)
{
    return wxETKSQLite3Expression(_rValue) == _rColumn;
}

EXPORT_IMPORT wxETKSQLite3Expression operator>(const wxETKSQLite3Value &_rValue,const wxETKSQLite3Column &_rColumn)
{
    return wxETKSQLite3Expression(_rValue) > _rColumn;
}

EXPORT_IMPORT wxETKSQLite3Expression operator>=(const wxETKSQLite3Value &_rValue,const wxETKSQLite3Column &_rColumn)
{
    return wxETKSQLite3Expression(_rValue) >= _rColumn;
}

EXPORT_IMPORT wxETKSQLite3Expression operator<(const wxETKSQLite3Value &_rValue,const wxETKSQLite3Column &_rColumn)
{
    return wxETKSQLite3Expression(_rValue) < _rColumn;
}

EXPORT_IMPORT wxETKSQLite3Expression operator<=(const wxETKSQLite3Value &_rValue,const wxETKSQLite3Column &_rColumn)
{
    return wxETKSQLite3Expression(_rValue) <= _rColumn;
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

wxETKSQLite3Expression dbCount()
{
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationCount);
}

wxETKSQLite3Expression dbCount(const wxETKSQLite3Column& _rColumn)
{
    wxETKSQLite3Expression expression(_rColumn);
    expression.SetExpressionOrOperationType(wxETKSQLite3Expression::eOperationCount);
    return expression;
}

wxETKSQLite3Expression dbCount(const wxETKSQLite3Expression& _rExpression)
{
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationCount,_rExpression,wxETKSQLite3Expression());
}

wxETKSQLite3Expression dbDistinct(const wxETKSQLite3Column& _rColumn)
{
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationDistinct,wxETKSQLite3Expression(_rColumn),wxETKSQLite3Expression());
}

wxETKSQLite3Expression dbMax(const wxETKSQLite3Column& _rColumn)
{
    wxETKSQLite3Expression expression(_rColumn);
    expression.SetExpressionOrOperationType(wxETKSQLite3Expression::eOperationMax);
    return expression;
}

wxETKSQLite3Expression dbMin(const wxETKSQLite3Column& _rColumn)
{
    wxETKSQLite3Expression expression(_rColumn);
    expression.SetExpressionOrOperationType(wxETKSQLite3Expression::eOperationMin);
    return expression;
}

wxETKSQLite3Expression dbSum(const wxETKSQLite3Column& _rColumn)
{
    wxETKSQLite3Expression expression(_rColumn);
    expression.SetExpressionOrOperationType(wxETKSQLite3Expression::eOperationSum);
    return expression;
}

wxETKSQLite3Expression dbCast(const wxETKSQLite3Column& _rColumn,wxString _strCast)
{
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationCast,wxETKSQLite3Expression(wxETKSQLite3Value(_rColumn.GetFullName(),wxETKSQLite3Expression::eExpressionColumnNameFull)),wxETKSQLite3Expression(_strCast,wxETKSQLite3Expression::eExpressionTypeValue));
}

wxETKSQLite3Expression dbSubString(const wxETKSQLite3Expression &_rExpression,const wxETKSQLite3Expression &_rExpressionStart,const wxETKSQLite3Expression &_rExpressionLength)
{   // Little bit complex: format it directly into SQL cstring: we need 3 parameters for this operator and all expressions have only 2
    wxString strSql;
    strSql.Printf(_T("SUBSTR(%s,%s,%s)"),_rExpression.FormatRequest().c_str(),_rExpressionStart.FormatRequest().c_str(),_rExpressionLength.FormatRequest().c_str());
    return wxETKSQLite3Expression(strSql,wxETKSQLite3Expression::eExpressionTypeSQL);
}

wxETKSQLite3Expression dbAs(const wxETKSQLite3Expression& _rExpression,wxString _strAsName)
{
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationAs,_rExpression,wxETKSQLite3Value(_strAsName));
}

wxETKSQLite3Expression dbAs(const wxETKSQLite3Criterion& _rCriterion,wxString _strAsName)
{
    wxETKSQLite3Expression exprAdd;

    exprAdd.Add(_rCriterion);
    return wxETKSQLite3Expression(wxETKSQLite3Expression::eOperationAs,exprAdd,wxETKSQLite3Value(_strAsName));
}

wxETKSQLite3Expression dbSQL(wxString _strSQLRequest)
{
    return wxETKSQLite3Value(_strSQLRequest,wxETKSQLite3Expression::eExpressionTypeSQL);
}
