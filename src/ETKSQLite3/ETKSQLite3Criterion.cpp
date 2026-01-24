/////////////////////////////////////////////////////////////////////////////
/**
 * @file ETKSQLite3Criterion.cpp
 * @brief Implementation file for SQL request creation.
 *
 * This file contains the implementation for the ETKSQLite3Value,
 * ETKSQLite3Expression, and ETKSQLite3Criterion classes, which are used to
 * define the criteria to be used in queries to be formatted in SQL.
 *
 * @author Stéphane Château
 * @date Created: 2011/08/04
 * @date Modified: 2025/08/29
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#include "ETKSQLite3Criterion.h"
#include "ETKSQLite3Column.h"
#include "ETKSQLite3ResultSet.h"
#include "ETKSQLite3VariantDatas.h"
#include "ETKSQLite3RequestSelector.h"

#include <algorithm>
#include <wx/tokenzr.h>

// Classe used to concat the for_each of all elements, separates with with comma.
struct EXPORT_IMPORT ETKSQLite3StringConcat
{
    etkString  m_strValue;

    etkString  operator()(const std::pair<etkString const, bool> &_Value)
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
//                                          ETKSQLite3Value                                              //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
ETKSQLite3Value::ETKSQLite3Value(eExpressionOrOperationType _expressionType)
    : m_ExpressionType(_expressionType)
{
}

// Copy constructor
ETKSQLite3Value::ETKSQLite3Value(const ETKSQLite3Value &_rOther)
{
    operator=(_rOther);
}

ETKSQLite3Value::ETKSQLite3Value(const ETKSQLite3ColumnAttributes &_rColumnAttributes)
    : m_Value(new ETKSQLite3VariantDataColumnAttributes(_rColumnAttributes))
    , m_ExpressionType(eExpressionColumnAttributes)
{
}

ETKSQLite3Value::ETKSQLite3Value(const ETKSQLite3ValueBind &_rValueBind)
    : m_Value(new ETKSQLite3VariantDataValueBind(_rValueBind))
    , m_ExpressionType(eExpressionValueBind)
{
}

ETKSQLite3Value::ETKSQLite3Value(const ETKSQLite3RequestSelector& _rSelector)
    : m_Value(new ETKSQLite3VariantDataRequestSelector(_rSelector))
    , m_ExpressionType(eExpressionRequestSelector)
{
}

// Value constructor (as variant)
ETKSQLite3Value::ETKSQLite3Value(const etkVariant &_rValue)
    : m_Value(_rValue)
    , m_ExpressionType(eExpressionTypeValue)
{
}

ETKSQLite3Value::ETKSQLite3Value(const struct tm &_rtmValue)
    : m_Value(etkDateTime(_rtmValue).MakeUTC())
    , m_ExpressionType(eExpressionTypeValue)
{
}

ETKSQLite3Value::ETKSQLite3Value(const etkDateTime &_rdtValue)
    : m_Value(etkDateTime(_rdtValue).MakeUTC())
    , m_ExpressionType(eExpressionTypeValue)
{
}

ETKSQLite3Value::ETKSQLite3Value(long _lValue)
    : m_Value(_lValue)
    , m_ExpressionType(eExpressionTypeValue)
{
}

ETKSQLite3Value::ETKSQLite3Value(int _iValue)
    : m_Value((long) _iValue)
    , m_ExpressionType(eExpressionTypeValue)
{
}

ETKSQLite3Value::ETKSQLite3Value(double _dValue)
    : m_Value(_dValue)
    , m_ExpressionType(eExpressionTypeValue)
{
}

ETKSQLite3Value::ETKSQLite3Value(const etkInt64 &_ri64Value)
    : m_Value(new ETKSQLite3VariantDataLongLong(_ri64Value))
    , m_ExpressionType(eExpressionTypeValue)
{
}

ETKSQLite3Value::ETKSQLite3Value(const wxChar *_pszValue, eExpressionOrOperationType _expressionType)
    : m_Value(etkString(_pszValue))
    , m_ExpressionType(_expressionType)
{
}

ETKSQLite3Value::ETKSQLite3Value(etkString _strValue, eExpressionOrOperationType _expressionType)
    : m_Value(_strValue)
    , m_ExpressionType(_expressionType)
{
}

ETKSQLite3Value::~ETKSQLite3Value()
{
    Clear();
}

const ETKSQLite3Value & ETKSQLite3Value::operator=(const ETKSQLite3Value &_rExpression)
{
    m_Value = _rExpression.m_Value;
    SetExpressionOrOperationType(_rExpression.GetExpressionOrOperationType());

    return *this;
}

const etkVariant & ETKSQLite3Value::GetVariant() const
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
const etkVariant & ETKSQLite3Value::GetVariantDebug() const
{
    return m_Value;
}
#endif

etkString ETKSQLite3Value::GetColumnName() const
{
    wxASSERT(   GetExpressionOrOperationType() == eExpressionColumnNameFull
             || GetExpressionOrOperationType() == eExpressionColumnNameOnly
             || GetExpressionOrOperationType() == eOperationCount);

    etkString strNameColumn(GetVariant().GetString());
    int iFindIndex = strNameColumn.Find(_T('.'));
    if (iFindIndex != wxNOT_FOUND)
    {   // Find column name
        strNameColumn = strNameColumn.Right(strNameColumn.length() - iFindIndex - 1);
    }

    return strNameColumn;
}

etkString ETKSQLite3Value::GetTableName() const
{
    wxASSERT(   GetExpressionOrOperationType() == eExpressionColumnNameFull
             || GetExpressionOrOperationType() == eExpressionColumnNameOnly
             || GetExpressionOrOperationType() == eOperationAsJoin); // AsJoin operation ask for table name too

    etkString strNameColumn(GetVariant().GetString());
    int iFindIndex = strNameColumn.Find(_T('.'));
    if (iFindIndex != wxNOT_FOUND)
    {   // Trouver le nom de la table
        strNameColumn = strNameColumn.Left(iFindIndex);
    }
    else
    {
        wxFAIL_MSG(etkString::Format(wxT("Table name (%s) of type %d is not a Column type"), strNameColumn.wx_str(), GetExpressionOrOperationType())); // Strange, the table name doesn't exist!
        strNameColumn.clear();
    }

    return strNameColumn;
}

void ETKSQLite3Value::BindTo(wxSQLite3Statement &_rstmt, int &_riIndex, bool _bForInsertRequest) const
{
    if (GetExpressionOrOperationType() == eExpressionColumnAttributes)
    {
        ETKSQLite3VariantDataColumnAttributes *pColumnAttributes = dynamic_cast<ETKSQLite3VariantDataColumnAttributes *>(GetVariant().GetData());
        wxASSERT(pColumnAttributes != nullptr); // Should never arrives
        if (pColumnAttributes != nullptr)
        {
            pColumnAttributes->BindTo(_rstmt, _riIndex++, _bForInsertRequest);
        }
    }
    else if (GetExpressionOrOperationType() == eExpressionValueBind)
    {
        ETKSQLite3VariantDataValueBind *pValueBind = dynamic_cast<ETKSQLite3VariantDataValueBind *>(GetVariant().GetData());
        wxASSERT(pValueBind != nullptr); // Should never arrives
        if (pValueBind != nullptr)
        {
            pValueBind->BindTo(_rstmt, _riIndex++, true, _bForInsertRequest);
        }
    }
    else if (GetExpressionOrOperationType() == eExpressionRequestSelector)
    {
        ETKSQLite3VariantDataRequestSelector *pRequestSelector = dynamic_cast<ETKSQLite3VariantDataRequestSelector *>(GetVariant().GetData());
        wxASSERT(pRequestSelector != nullptr); // Should never arrives
        if (pRequestSelector != nullptr)
        {
            pRequestSelector->GetCriterionRequest().BindTo(_rstmt, _riIndex);
        }
    }
}

etkString ETKSQLite3Value::GetAsString() const
{
    etkString strCriterionAsString;

    switch (GetExpressionOrOperationType())
    {
        case eExpressionTypeNothing :
        {
            break;
        }
        case eExpressionOrderByASC :
        {
            strCriterionAsString.Printf(_T("%s ASC"), m_Value.GetString().c_str());
            break;
        }
        case eExpressionOrderByDESC :
        {
            strCriterionAsString.Printf(_T("%s DESC"), m_Value.GetString().c_str());
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
                strCriterionAsString.Printf(_T("%f"), m_Value.GetDouble());
                strCriterionAsString.Replace(_T(","), _T("."));
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
            strCriterionAsString.Printf(_T("(%s)"), m_Value.GetString().c_str());
            break;
        }
        case eExpressionTypeValueQuoteString :
        {
            strCriterionAsString.Printf(_T("'%s'"), QuoteString(m_Value.GetString()).c_str());
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
        case eOperationExists :
        case eOperationLike :
        case eOperationAs :
        case eOperationAsSelect :
        case eOperationAsSelectFrom :
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

bool ETKSQLite3Value::IsValueEmpty() const
{
    return m_Value.IsNull();
}

ETKSQLite3Value::eExpressionOrOperationType ETKSQLite3Value::GetExpressionOrOperationType() const
{
    return m_ExpressionType;
}

void ETKSQLite3Value::SetExpressionOrOperationType(ETKSQLite3Value::eExpressionOrOperationType _expressionType)
{
    m_ExpressionType = _expressionType;
}

void ETKSQLite3Value::Clear()
{
    m_Value.Clear();
    SetExpressionOrOperationType(eExpressionTypeNothing);
}

bool ETKSQLite3Value::IsAndOrOR() const
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
        case eOperationExists :
        case eOperationLike :
        case eOperationAs :
        case eOperationAsSelect :
        case eOperationAsSelectFrom :
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

etkString ETKSQLite3Value::QuoteString(etkString _strValue) const
{
    _strValue.Replace(_T("'"), _T("''"));
    return _strValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                       ETKSQLite3Expression                                            //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __WXDEBUG__
unsigned long ETKSQLite3Expression::s_ulCurrentCounterId = 0;
#endif

//----------------------------
// Constructors / Destructor

// Default expression
ETKSQLite3Expression::ETKSQLite3Expression(eExpressionOrOperationType _expressionType)
    : ETKSQLite3Value(_expressionType)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

// Copy constructor
ETKSQLite3Expression::ETKSQLite3Expression(const ETKSQLite3Expression &_rExpression)
    : m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
    operator=(_rExpression);
}

// Two expressions
ETKSQLite3Expression::ETKSQLite3Expression(eExpressionOrOperationType _expressionType, const ETKSQLite3Expression &_rExpression1, const ETKSQLite3Expression &_rExpression2)
    : ETKSQLite3Value(_expressionType)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
    Assign2Expressions(_rExpression1, _rExpression2);
}

ETKSQLite3Expression::ETKSQLite3Expression(const ETKSQLite3Value &_rValueAndExpressionType, const ETKSQLite3Expression &_rExpression1, const ETKSQLite3Expression &_rExpression2)
    : ETKSQLite3Value(_rValueAndExpressionType)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
    Assign2Expressions(_rExpression1, _rExpression2);
}

// With column
ETKSQLite3Expression::ETKSQLite3Expression(const ETKSQLite3Column &_rColumn, eExpressionOrOperationType _expressionType)
    : ETKSQLite3Value(_rColumn.GetFullName(), _expressionType)  // Let full name is it can be needed but the AsString function will return only column name
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
    wxASSERT(GetExpressionOrOperationType() == eExpressionColumnNameFull || GetExpressionOrOperationType() == eExpressionColumnNameOnly);
}

// Binding column with attributes
ETKSQLite3Expression::ETKSQLite3Expression(const ETKSQLite3ColumnAttributes &_rColumnAttributes)
    : ETKSQLite3Value(_rColumnAttributes)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

// Binding column with attributes
ETKSQLite3Expression::ETKSQLite3Expression(const ETKSQLite3ValueBind &_rValueBind)
    : ETKSQLite3Value(_rValueBind)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

// With ETKSQLite3Value value
ETKSQLite3Expression::ETKSQLite3Expression(const ETKSQLite3Value &_rValue)
    : ETKSQLite3Value(_rValue)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

ETKSQLite3Expression::ETKSQLite3Expression(const etkVariant &_rValue)
    : ETKSQLite3Value(_rValue)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

ETKSQLite3Expression::ETKSQLite3Expression(const struct tm &_rtmValue)
    : ETKSQLite3Value(_rtmValue)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

ETKSQLite3Expression::ETKSQLite3Expression(const etkDateTime &_rdtValue)
    : ETKSQLite3Value(_rdtValue)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

ETKSQLite3Expression::ETKSQLite3Expression(long _lValue)
    : ETKSQLite3Value(_lValue)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

ETKSQLite3Expression::ETKSQLite3Expression(int _iValue)
    : ETKSQLite3Value(_iValue)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

ETKSQLite3Expression::ETKSQLite3Expression(double _dValue)
    : ETKSQLite3Value(_dValue)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

ETKSQLite3Expression::ETKSQLite3Expression(const etkInt64 &_ri64Value)
    : ETKSQLite3Value(_ri64Value)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

ETKSQLite3Expression::ETKSQLite3Expression(const wxChar *_pszValue, eExpressionOrOperationType _expressionType)
    : ETKSQLite3Value(_pszValue, _expressionType)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

ETKSQLite3Expression::ETKSQLite3Expression(etkString _value, eExpressionOrOperationType _expressionType)
    : ETKSQLite3Value(_value, _expressionType)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

ETKSQLite3Expression::ETKSQLite3Expression(const ETKSQLite3RequestSelector& _rSelector)
    : ETKSQLite3Value(_rSelector)
    , m_pExpression1(nullptr)
    , m_pExpression2(nullptr)
#ifdef __WXDEBUG__
    , m_ulCurrentId(++s_ulCurrentCounterId)
#endif
{
}

// Destructor
ETKSQLite3Expression::~ETKSQLite3Expression()
{
    Clear();
}

// Two expressions, used ONLY into 2 constructors, private function
void ETKSQLite3Expression::Assign2Expressions(const ETKSQLite3Expression &_rExpression1, const ETKSQLite3Expression &_rExpression2)
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
                m_pExpression1 = new ETKSQLite3Expression(_rExpression1);
                m_pExpression2 = new ETKSQLite3Expression(_rExpression2);
            }
        }
    }
    else
    {
        if (_rExpression1.GetExpressionOrOperationType() != eExpressionTypeNothing)
        {
            m_pExpression1 = new ETKSQLite3Expression(_rExpression1);
        }
        if (_rExpression2.GetExpressionOrOperationType() != eExpressionTypeNothing)
        {
            m_pExpression2 = new ETKSQLite3Expression(_rExpression2);
        }

        wxASSERT_MSG(m_pExpression1 != nullptr || m_pExpression2 == nullptr, wxT("Expression 1 is nullptr and Expression 2 is not, it is not allowed! Check your source code!"));
    }
}

// Constructors / Destructor
//----------------------------


//----------------------------
// Operators

// Assign
const ETKSQLite3Expression &ETKSQLite3Expression::operator=(const ETKSQLite3Expression &_rExpression)
{
    Clear();
    ETKSQLite3Value::operator=(_rExpression);

    if (_rExpression.m_pExpression1 != nullptr)
    {
        m_pExpression1 = new ETKSQLite3Expression(*_rExpression.m_pExpression1);
    }

    if (_rExpression.m_pExpression2 != nullptr)
    {
        m_pExpression2 = new ETKSQLite3Expression(*_rExpression.m_pExpression2);
    }

    return *this;
}

// Assign with request selector
const ETKSQLite3Expression & ETKSQLite3Expression::operator=(const ETKSQLite3RequestSelector& _rSelector)
{
    Clear();
    ETKSQLite3Value::operator=(_rSelector);
    return *this;
}

// Logical OR operator.
ETKSQLite3Expression ETKSQLite3Expression::operator||(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(eOperationOr, *this, _rExpression);
}

// Logical AND operator.
ETKSQLite3Expression ETKSQLite3Expression::operator&&(const ETKSQLite3Expression &_rExpression) const
{
    if (_rExpression.IsJoin())
    {
        wxASSERT_MSG(GetExpressionOrOperationType()==eExpressionTypeNothing || IsJoin(), wxT("This is JOIN, expression must be Nothing or JOIN, check your source code!"));
        return ETKSQLite3Expression(eOperationAndJoin, *this, _rExpression);
    }
    return ETKSQLite3Expression(eOperationAnd, *this, _rExpression);
}

// Logical == operator.
ETKSQLite3Expression ETKSQLite3Expression::operator==(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(eOperationEqual, *this, _rExpression);
}

// Logical == operator on column.
ETKSQLite3Expression ETKSQLite3Expression::operator==(const ETKSQLite3Column &_rColumn) const
{   // Call same operator but with expression that contains the full column name.
    return operator==(ETKSQLite3Expression(_rColumn.GetFullName(), eExpressionTypeValue));
}

// Logical > operator.
ETKSQLite3Expression ETKSQLite3Expression::operator>(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(eOperationUpper, *this, _rExpression);
}

ETKSQLite3Expression ETKSQLite3Expression::operator>(const ETKSQLite3Column &_rColumn) const
{   // Call same operator but with expression that contains the full column name.
    return operator>(ETKSQLite3Expression(_rColumn));
}

// Logical >= operator.
ETKSQLite3Expression ETKSQLite3Expression::operator>=(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(eOperationUpperOrEqual, *this, _rExpression);
}

ETKSQLite3Expression ETKSQLite3Expression::operator>=(const ETKSQLite3Column &_rColumn) const
{   // Call same operator but with expression that contains the full column name.
    return operator>=(ETKSQLite3Expression(_rColumn));
}

// Logical < operator.
ETKSQLite3Expression ETKSQLite3Expression::operator<(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(eOperationLower, *this, _rExpression);
}

ETKSQLite3Expression ETKSQLite3Expression::operator<(const ETKSQLite3Column &_rColumn) const
{   // Call same operator but with expression that contains the full column name.
    return operator<(ETKSQLite3Expression(_rColumn));
}

// Logical <= operator.
ETKSQLite3Expression ETKSQLite3Expression::operator<=(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(eOperationLowerOrEqual, *this, _rExpression);
}

ETKSQLite3Expression ETKSQLite3Expression::operator<=(const ETKSQLite3Column &_rColumn) const
{   // Call same operator but with expression that contains the full column name.
    return operator<=(ETKSQLite3Expression(_rColumn.GetFullName(), eExpressionTypeValue));
}

ETKSQLite3Expression ETKSQLite3Expression::operator!() const
{
    return ETKSQLite3Expression(eOperationNot, *this, ETKSQLite3Expression());
}

ETKSQLite3Expression ETKSQLite3Expression::Like(const ETKSQLite3Expression &_rExpression) const
{
    return ETKSQLite3Expression(eOperationLike, *this, _rExpression);
}

ETKSQLite3Expression ETKSQLite3Expression::In(const ETKSQLite3Expression &_rExpression) const
{
    // Must be column type
    wxASSERT(GetExpressionOrOperationType() == eExpressionColumnNameFull);
    // Often formatted directly into SQL or selector, If not case, code should be analyzed and be upgraded, making new tests!
    wxASSERT(_rExpression.GetExpressionOrOperationType() == eExpressionTypeSQL || _rExpression.GetExpressionOrOperationType() == eExpressionRequestSelector);
    if (!_rExpression.IsNull())
    {   // If In is not empty
        return ETKSQLite3Expression(ETKSQLite3Expression::eOperationIn, *this, _rExpression);
    }

    return ETKSQLite3Expression(); // If the 'in' is empty, return empty expression
}

ETKSQLite3Expression ETKSQLite3Expression::Join(const ETKSQLite3Expression &_rExpressionIn) const
{
    if (!_rExpressionIn.IsNull())
    {   // If In is not empty
        ETKSQLite3Expression exprThis(*this);
        if (GetExpressionOrOperationType() == eOperationAs)
        {   // 'This' is an alias, change type to be correctly generate SQL: JOIN Table Alias ON (expression)
            exprThis.SetExpressionOrOperationType(ETKSQLite3Value::eOperationAsJoin);
        }
        // else generate SQL: JOIN Table ON Table.Column = Table2.Column2
        return ETKSQLite3Expression(ETKSQLite3Expression::eOperationJoin, exprThis, _rExpressionIn);
    }

    return ETKSQLite3Expression(); // If the 'inner join' is empty, return empty expression
}

ETKSQLite3Expression ETKSQLite3Expression::InnerJoin(const ETKSQLite3Expression &_rExpressionIn) const
{
    if (!_rExpressionIn.IsNull())
    {   // If In is not empty
        ETKSQLite3Expression exprThis(*this);
        if (GetExpressionOrOperationType() == eOperationAs)
        {   // 'This' is an alias, change type to be correctly generate SQL: INNER JOIN Table Alias ON (expression)
            exprThis.SetExpressionOrOperationType(ETKSQLite3Value::eOperationAsJoin);
        }
        // else generate SQL: INNER JOIN Table ON Table.Column = Table2.Column2
        return ETKSQLite3Expression(ETKSQLite3Expression::eOperationInnerJoin, exprThis, _rExpressionIn);
    }

    return ETKSQLite3Expression(); // If the 'inner join' is empty, return empty expression
}

ETKSQLite3Expression ETKSQLite3Expression::LeftJoin(const ETKSQLite3Expression &_rExpressionIn) const
{
    if (!_rExpressionIn.IsNull())
    {   // If In is not empty
        ETKSQLite3Expression exprThis(*this);
        if (GetExpressionOrOperationType() == eOperationAs)
        {   // 'This' is an alias, change type to be correctly generate SQL: LEFT JOIN Table Alias ON (expression)
            exprThis.SetExpressionOrOperationType(ETKSQLite3Value::eOperationAsJoin);
        }
        // else generate SQL: LEFT JOIN Table ON Table.Column = Table2.Column2
        return ETKSQLite3Expression(ETKSQLite3Expression::eOperationLeftJoin, exprThis, _rExpressionIn);
    }

    return ETKSQLite3Expression(); // If the 'inner join' is empty, return empty expression
}

// Operators
//----------------------------

void ETKSQLite3Expression::Clear()
{
    if (m_pExpression1 != nullptr)
    {
        delete m_pExpression1;
        m_pExpression1 = nullptr;
    }
    if (m_pExpression2 != nullptr)
    {
        delete m_pExpression2;
        m_pExpression2 = nullptr;
    }

    ETKSQLite3Value::Clear();
}

bool ETKSQLite3Expression::CanHaveOrderBy() const
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
        case eOperationAsSelectFrom :
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

const ETKSQLite3Expression & ETKSQLite3Expression::Add(const ETKSQLite3Expression &_rExpression)
{
    // Used to change operation type on AS depending if it is used on selector or somewhere else
    eExpressionOrOperationType expressionToAddType = (_rExpression.GetExpressionOrOperationType() == eOperationAs)
                                                        ? eOperationAsSelect
                                                        : _rExpression.GetExpressionOrOperationType();

    switch (GetExpressionOrOperationType())
    {
        case eOperationAsJoin :
        {
            wxASSERT_MSG(false, wxT("Cannot add value to AsJoin operation"));
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
            if (m_pExpression1 == nullptr)
            {
                m_pExpression1 = new ETKSQLite3Expression(_rExpression);
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
            wxASSERT(m_pExpression1 != nullptr);
            wxASSERT(m_pExpression2 != nullptr);
            wxASSERT(_rExpression.m_pExpression1->GetExpressionOrOperationType() == eExpressionColumnNameOnly);
            m_pExpression1->Add(*_rExpression.m_pExpression1);
            m_pExpression2->Add(*_rExpression.m_pExpression2);
            break;
        }
        case eOperationAssignmentUpdate :
        {
            wxASSERT(m_pExpression1 != nullptr);
            wxASSERT(m_pExpression2 != nullptr);
            m_pExpression2->Add(_rExpression);
            break;
        }
        case eOperationAs :
        case eOperationAsSelect :
        case eOperationAsSelectFrom :
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
            if (m_pExpression1 == nullptr)
            {
                m_pExpression1 = new ETKSQLite3Expression(*this);
                m_pExpression2 = new ETKSQLite3Expression(_rExpression);
                m_pExpression2->SetExpressionOrOperationType(expressionToAddType); // To change eOperationAs to eOperationAsSelect if needed
                SetExpressionOrOperationType(expressionType);
            }
            else
            {   // Expression 1 already exists
                ETKSQLite3Expression * exprTemp = new ETKSQLite3Expression(*this);
                delete m_pExpression1;
                m_pExpression1 = exprTemp;

                if (m_pExpression2 != nullptr)
                {
                    delete m_pExpression2;
                }

                m_pExpression2 = new ETKSQLite3Expression(_rExpression);
                m_pExpression2->SetExpressionOrOperationType(expressionToAddType); // To change eOperationAs to eOperationAsSelect if needed
                SetExpressionOrOperationType(expressionType);
            }
            break;
        }
    }
    return *this;
}

const ETKSQLite3Expression & ETKSQLite3Expression::Add(const ETKSQLite3Criterion &_rCriterion)
{
    wxASSERT_MSG(_rCriterion.GetRequestType() == ETKSQLite3Criterion::eRequestTypeSelect, wxT("Adding criterion to expression MUST be a select criterion!"));
    if (_rCriterion.GetRequestType() == ETKSQLite3Criterion::eRequestTypeSelect)
    {
        return Add(ETKSQLite3Expression(ETKSQLite3RequestSelector(_rCriterion)));
    }
    return *this;
}

ETKSQLite3Expression & ETKSQLite3Expression::operator<<(const ETKSQLite3Expression &_rExpression)
{
    Add(_rExpression);
    return *this;
}


#include <stack>

etkString ETKSQLite3Expression::GetAsString() const
{
    return GetAsStringIterative();
}

etkString ETKSQLite3Expression::GetAsStringIterative() const
{
    struct Frame
    {
        const ETKSQLite3Expression* m_pExpression;
        bool                        m_bIsVisited;
    };

    etkString strResult;
    std::stack<Frame> stackFrame;
    std::stack<etkString> stackString;

    stackFrame.push({ this, false });

    while (!stackFrame.empty())
    {
        Frame frame = stackFrame.top();
        stackFrame.pop();

        const ETKSQLite3Expression * pExpression = frame.m_pExpression;
        if (pExpression != nullptr)
        {
            if (frame.m_bIsVisited)
            {
                // Get SQL String already generated
                etkString left, right;
                if (pExpression->m_pExpression2 != nullptr)
                {
                    right = stackString.top();
                    stackString.pop();
                }
                if (pExpression->m_pExpression1 != nullptr)
                {
                    left = stackString.top();
                    stackString.pop();
                }

                etkString strCurrentSQLNode;
                InternalAsStringForNode(strCurrentSQLNode, pExpression, pExpression->m_pExpression1, left, pExpression->m_pExpression2, right);
                stackString.push(strCurrentSQLNode);
            }
            else
            {
                // First stack the current node
                stackFrame.push({ pExpression, true });

                // Then children, begin with right to have left called before
                if (pExpression->m_pExpression2 != nullptr)
                {
                    stackFrame.push({ pExpression->m_pExpression2, false });
                }
                if (pExpression->m_pExpression1 != nullptr)
                {
                    stackFrame.push({ pExpression->m_pExpression1, false });
                }
            }
        }

        if (!stackString.empty())
        {
            strResult = stackString.top();
        }
    }

    return strResult;
}

void ETKSQLite3Expression::InternalAsStringForNode(etkString & _rstrOperatorAsString, const ETKSQLite3Expression *_pExpression, const ETKSQLite3Expression *_pExpression1, const etkString & _rstrExpression1, const ETKSQLite3Expression *_pExpression2, const etkString & _rstrExpression2)
{
    switch (_pExpression->GetExpressionOrOperationType())
    {
        case eExpressionTypeNothing :
        {   // Nothing to do
            break;
        }
        case eExpressionOrderByASC :
        case eExpressionOrderByDESC :
        {
            _rstrOperatorAsString = _pExpression->ETKSQLite3Value::GetAsString();
            if (!_rstrExpression1.IsEmpty())
            {   // Add all Orderby
                _rstrOperatorAsString += _T(",") + _rstrExpression1;
            }
            break;
        }
        case eOperationAddComma :
        {
            _rstrOperatorAsString.Printf(_T("%s,%s"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationOr :
        {
            _rstrOperatorAsString = InternalGetAsStringForOperator(_T("OR"), _pExpression1, _rstrExpression1, _pExpression2, _rstrExpression2);
            break;
        }
        case eOperationAnd :
        {
            _rstrOperatorAsString = InternalGetAsStringForOperator(_T("AND"), _pExpression1, _rstrExpression1, _pExpression2, _rstrExpression2);
            break;
        }
        case eOperationUpper :
        {
            _rstrOperatorAsString.Printf(_T("%s > %s"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationUpperOrEqual :
        {
            _rstrOperatorAsString.Printf(_T("%s >= %s"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationLower :
        {
            _rstrOperatorAsString.Printf(_T("%s < %s"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationLowerOrEqual :
        {
            _rstrOperatorAsString.Printf(_T("%s <= %s"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationEqual :
        {
            _rstrOperatorAsString.Printf(_T("%s = %s"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationExists :
        {
            wxASSERT(!_rstrExpression1.IsEmpty());
            wxASSERT(_rstrExpression2.IsEmpty());
            _rstrOperatorAsString.Printf(_T("EXISTS (%s)"), _rstrExpression1.c_str());
            break;
        }
        case eOperationLike :
        {
            _rstrOperatorAsString.Printf(_T("%s LIKE %s"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationAs :
        {   // The name hase changed, used only this one into expression
            _rstrOperatorAsString.Printf(_T("%s"), _rstrExpression1.c_str());
            break;
        }
        case eOperationAsSelect :
        {   // Into select, generate AS into expression
            if (_rstrExpression1.Len() > 2 && _rstrExpression1[0] == _T('(') && _rstrExpression1[_rstrExpression1.Len() - 1] == _T(')'))
            {   // Don't put too much ()
                _rstrOperatorAsString.Printf(_T("%s AS %s"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            }
            else
            {
                _rstrOperatorAsString.Printf(_T("(%s) AS %s"),_rstrExpression1.c_str(), _rstrExpression2.c_str());
            }
            break;
        }
        case eOperationAsSelectFrom :
        {
            // m_pExpression2 contains AS but here, it is on SELECT, the AS is goes to FROM xx AS <asname>
            _rstrOperatorAsString = _rstrExpression1;
            break;
        }
        case eOperationAsJoin :
        {   // Generate after the join keyword, when using alias, must generate 2 tables names
            _rstrOperatorAsString.Printf(_T("%s %s"), _pExpression->GetTableName().c_str(), _pExpression1->GetTableName().c_str());
            break;
        }
        case eOperationDifferent :
        {
            _rstrOperatorAsString.Printf(_T("%s <> %s"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationNot :
        {
            _rstrOperatorAsString.Printf(_T("NOT (%s)"), _rstrExpression1.c_str());
            break;
        }
        case eOperationMult :
        {
            _rstrOperatorAsString.Printf(_T("(%s * %s)"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationDiv :
        {
            _rstrOperatorAsString.Printf(_T("(%s / %s)"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationAdd :
        {
            _rstrOperatorAsString.Printf(_T("(%s + %s)"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationSub :
        {
            _rstrOperatorAsString.Printf(_T("(%s - %s)"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eExpressionTypeValue :
        case eExpressionTypeValueQuoteString :
        case eExpressionTypeSQL : // Must contains one value type expression (with column name) + One SQL formatted expression
        case eExpressionRequestSelector :
        case eExpressionColumnNameFull :
        {
            _rstrOperatorAsString = _pExpression->ETKSQLite3Value::GetAsString();
            break;
        }
        case eExpressionColumnNameOnly :
        {
            _rstrOperatorAsString = _pExpression->ETKSQLite3Value::GetColumnName();
            break;
        }
        case eOperationJoin :
        {   // Must contains two expression of column name type
            wxASSERT(!_rstrExpression1.IsEmpty() && _pExpression1->GetExpressionOrOperationType() == eExpressionColumnNameFull);
            wxASSERT(!_rstrExpression2.IsEmpty() && _pExpression2->GetExpressionOrOperationType() == eExpressionColumnNameFull);

            _rstrOperatorAsString.Printf(_T(" JOIN %s ON %s = %s"), _pExpression2->GetTableName().c_str(), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationInnerJoin :
        case eOperationLeftJoin :
        case eOperationRightJoin :
        case eOperationFullJoin :
        {   // Must contains two expression of column name type
            etkString strJoinOperator;
            switch (_pExpression->GetExpressionOrOperationType())
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

            wxASSERT(!_rstrExpression1.IsEmpty());
            wxASSERT(!_rstrExpression2.IsEmpty());
            if (   _pExpression1->GetExpressionOrOperationType() == eExpressionColumnNameFull
                && _pExpression2->GetExpressionOrOperationType() == eExpressionColumnNameFull
               )
            {   // Take table name
                _rstrOperatorAsString.Printf(_T(" %s %s ON %s = %s"), strJoinOperator.c_str(), _pExpression2->GetTableName().c_str(), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            }
            else
            {
                _rstrOperatorAsString.Printf(_T(" %s %s ON %s"), strJoinOperator.c_str(), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            }
            break;
        }
        case eOperationIn:
        {   // Must contains one expression of column name type + one SQL formatted expression
            wxASSERT(!_rstrExpression1.IsEmpty());
            wxASSERT(!_rstrExpression2.IsEmpty());
            wxASSERT(_pExpression1->GetExpressionOrOperationType() == eExpressionColumnNameFull);
            wxASSERT(_pExpression2->GetExpressionOrOperationType() == eExpressionTypeSQL);
            _rstrOperatorAsString.Printf(_T(" %s IN (%s)"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationCount :
        {
            if (!_rstrExpression1.IsEmpty())
            {
                if (_pExpression1->GetExpressionOrOperationType() == eExpressionColumnNameFull)
                {
                    _rstrOperatorAsString.Printf(_T("COUNT (%s)"), _pExpression1->GetColumnName().c_str());
                }
                else
                {
                    _rstrOperatorAsString.Printf(_T("COUNT (%s)"), _rstrExpression1.c_str());
                }
            }
            else
            {
                etkString strTmpString = _pExpression->ETKSQLite3Value::GetAsString();
                if (strTmpString.length() == 0 || _pExpression->ETKSQLite3Value::GetColumnName()==_T("*"))
                {   // If nothing into count parameters, take (*)
                    _rstrOperatorAsString = _T("COUNT (*)");
                }
                else
                {
                    _rstrOperatorAsString.Printf(_T("COUNT (%s)"), strTmpString.c_str());
                }
            }
            break;
        }
        case eOperationMin:
        {
            etkString strTmpString = _pExpression->ETKSQLite3Value::GetAsString();
            if (strTmpString.length() == 0)
            {   // If nothing into min parameters, take (*)
                _rstrOperatorAsString = _T("MIN (*)");
            }
            else
            {
                _rstrOperatorAsString.Printf(_T("MIN (%s)"), strTmpString.c_str());
            }
            break;
        }
        case eOperationMax:
        {
            etkString strTmpString = _pExpression->ETKSQLite3Value::GetAsString();
            if (strTmpString.length() == 0)
            {   // If nothing into max parameters, take (*)
                _rstrOperatorAsString = _T("MAX (*)");
            }
            else
            {
                _rstrOperatorAsString.Printf(_T("MAX (%s)"), strTmpString.c_str());
            }
            break;
        }
        case eOperationSum:
        {
            etkString strTmpString = _pExpression->ETKSQLite3Value::GetAsString();
            if (strTmpString.length() == 0)
            {   // If nothing into sum parameters, take (*)
                _rstrOperatorAsString = _T("SUM (*)");
            }
            else
            {
                _rstrOperatorAsString.Printf(_T("SUM (%s)"), strTmpString.c_str());
            }
            break;
        }
        case eOperationCast :
        {
            _rstrOperatorAsString.Printf(_T("CAST(%s AS %s)"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationIsNull :
        {
            _rstrOperatorAsString.Printf(_T("(%s IS nullptr)"), _rstrExpression1.c_str());
            break;
        }
        case eOperationIsNotNull :
        {
            _rstrOperatorAsString.Printf(_T("(%s IS NOT nullptr)"), _rstrExpression1.c_str());
            break;
        }
        case eOperationDistinct :
        {
            _rstrOperatorAsString.Printf(_T("DISTINCT %s"), _rstrExpression1.c_str());
            break;
        }
        case eExpressionColumnAttributes :
        case eExpressionValueBind :
        {
            _rstrOperatorAsString = _pExpression->ETKSQLite3Value::GetAsString();
            break;
        }
        case eOperationAssignmentInsert :
        {   // Must contains one expression of column name type + one value or binded data
            wxASSERT(!_rstrExpression1.IsEmpty());
            wxASSERT(!_rstrExpression2.IsEmpty());
            _rstrOperatorAsString.Printf(_T("(%s) VALUES (%s)"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationAssignmentUpdate :
        {   // Must contains one expression of column name type + one value or binded data
            wxASSERT(!_rstrExpression1.IsEmpty());
            wxASSERT(!_rstrExpression2.IsEmpty());
            // Cannot easily add the SET keyword, don't put it
            _rstrOperatorAsString.Printf(_T("%s=%s"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
        case eOperationAndJoin :
        {   // Must contains 2 expressions of JOIN type
            wxASSERT(!_rstrExpression1.IsEmpty() && _pExpression1->IsJoin());
            wxASSERT(!_rstrExpression2.IsEmpty() && _pExpression2->IsJoin());
            // Cannot easily add the SET keyword, don't put it
            _rstrOperatorAsString.Printf(_T("%s%s"), _rstrExpression1.c_str(), _rstrExpression2.c_str());
            break;
        }
    }
}

etkString ETKSQLite3Expression::InternalGetAsStringForOperator(etkString _strOperator, const ETKSQLite3Expression *_pExpression1, const etkString & _rstrExpression1, const ETKSQLite3Expression *_pExpression2, const etkString & _rstrExpression2)
{
    etkString strReturn;

    if (_pExpression1 == nullptr || _pExpression1->GetExpressionOrOperationType() == eExpressionTypeNothing)
    {
        if (_pExpression2 != nullptr && _pExpression2->GetExpressionOrOperationType() != eExpressionTypeNothing)
        {   // Expression 1 is empty and expresssion 2 is empty
            strReturn = _rstrExpression2;
        }
    }
    else if (_pExpression2 == nullptr || _pExpression2->GetExpressionOrOperationType() == eExpressionTypeNothing)
    {   // Expression is not empty ans 2 is empty
        strReturn = _rstrExpression1;
    }
    else
    {   // Both Expression 1 and 2 are not empty
        strReturn.Printf(_T("(%s %s %s)"), _rstrExpression1.c_str(), _strOperator.c_str(), _rstrExpression2.c_str());
    }

    return strReturn;
}

void ETKSQLite3Expression::BindTo(wxSQLite3Statement &_rstmt, int &_riIndex, bool _bForInsertRequest) const
{
    ETKSQLite3Value::BindTo(_rstmt, _riIndex, _bForInsertRequest);

    if (m_pExpression1 != nullptr)
    {
        m_pExpression1->BindTo(_rstmt, _riIndex, _bForInsertRequest);
    }
    if (m_pExpression2 != nullptr)
    {
        m_pExpression2->BindTo(_rstmt, _riIndex, _bForInsertRequest);
    }
}

etkString ETKSQLite3Expression::FormatRequest() const
{
    return GetAsString();
}

ETKSQLite3Expression::tdStringList & ETKSQLite3Expression::UpdateTablesList(tdStringList &_rlstTablesList) const
{
    switch (GetExpressionOrOperationType())
    {
        case eOperationMin :
        case eOperationMax :
        case eOperationSum :
        case eOperationCount :
        {   // Count elements: if the count/min/max/sum is made on column, put the table name into table lists
            if (!ETKSQLite3Value::IsValueEmpty())
            {   // If there is a value for the count/min/max/sum it is because it was made with column class: It should have TABLE.COLUMN
                // Must find the table name and add it (if not already exist) int _rlstTablesList
                // Be careful: call the function from ETKSQLite3Value class and not the derived class (not use derived virtual methode)
                etkString strValueString = ETKSQLite3Value::GetAsString();
                int iFoundPoint = strValueString.Find(_T('.'));
                if (iFoundPoint != wxNOT_FOUND)
                {   // Find table name
                    // Add element (if exist) and initialize the value to true, if the element already exists, nothing is modified
                    _rlstTablesList.insert(tdStringList::value_type(strValueString.Left(iFoundPoint), true));
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
            etkString strNameTable2 = m_pExpression2->GetAsString();
            int iFoundPoint = strNameTable2.Find(_T('.'));
            if (iFoundPoint != wxNOT_FOUND)
            {
                strNameTable2 = strNameTable2.Left(iFoundPoint);
                // Do not use the insert function but [] operator, it create the element if not exists and return element.
                if (_rlstTablesList.find(strNameTable2) == _rlstTablesList.end())
                {   // Only if doesn't already exists!
                    _rlstTablesList[strNameTable2] = false; // It is now into the table list with false attribute (not added to FROM)
                }
            }
            return _rlstTablesList; // Do not call UpdateTablesList on m_pExpression1 and m_pExpression2 else
                                    // this function will add it and I just removed it
        }
        case eExpressionColumnNameFull :
        case eExpressionColumnNameOnly :
        {   // Add all columns table
            // Take columns names
            // Add element (if exists) and init it with true value, if the element exits, nothing is changed
            _rlstTablesList.insert(tdStringList::value_type(ETKSQLite3Value::GetTableName(), true));
            break;
        }
        case eOperationAsSelectFrom:
        {
            if (m_pExpression2 != nullptr && m_pExpression2->GetExpressionOrOperationType() == eOperationAs)
            {
                _rlstTablesList.insert(tdStringList::value_type(etkString::Format(wxT("%s AS %s"),m_pExpression2->m_pExpression1->GetAsString().c_str(), m_pExpression2->m_pExpression2->GetAsString().c_str()), true));
            }
            return _rlstTablesList;
        }
        case eOperationAs :
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
    if (m_pExpression1 != nullptr)
    {
        m_pExpression1->UpdateTablesList(_rlstTablesList);
    }
    if (m_pExpression2 != nullptr)
    {
        m_pExpression2->UpdateTablesList(_rlstTablesList);
    }

    return _rlstTablesList;
}

const ETKSQLite3Expression * ETKSQLite3Expression::GetExpression1() const
{
    return m_pExpression1;
}

const ETKSQLite3Expression * ETKSQLite3Expression::GetExpression2() const
{
    return m_pExpression2;
}

bool ETKSQLite3Expression::IsJoin() const
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

bool ETKSQLite3Expression::IsNull() const
{
    return GetExpressionOrOperationType() == eExpressionTypeNothing;
}

#ifdef __WXDEBUG__
etkString ETKSQLite3Expression::GetStringContentDescription() const
{
    etkString strDescription;
    ConstructContentDescription(strDescription);
    return strDescription;
}

int ETKSQLite3Expression::ConstructContentDescription(etkString &_strDescription) const
{
    etkString strThisDescription;

    int iLenThisDescription = ConstructContentThisDescription(strThisDescription);

    if (this == nullptr)
    {
        _strDescription = strThisDescription;
    }
    else
    {
        // Left description
        etkString strLeftDescription;
        int iLenLeftDescription = GetExpression1()->ConstructContentDescription(strLeftDescription);

        // Right description
        etkString strRightDescription;
        int iLenRightDescription = GetExpression2()->ConstructContentDescription(strRightDescription);

        etkString strPadAllLeftLeft, strPadAllLeftRight;
        etkString strPadAllRightLeft, strPadAllRightRight;
        int iLenDescription = wxMax(iLenLeftDescription + iLenRightDescription + 3, iLenThisDescription);
        if (iLenLeftDescription + iLenRightDescription + 3 < iLenThisDescription)
        {
            strPadAllLeftLeft.Pad(((iLenThisDescription / 2) - (iLenLeftDescription    ))/2, wxT(' '));
            strPadAllLeftRight.Pad(((iLenThisDescription / 2) - (iLenLeftDescription + 1))/2, wxT(' '));

            strPadAllRightLeft.Pad(((iLenThisDescription / 2) - (iLenRightDescription    ))/2, wxT(' '));
            strPadAllRightRight.Pad(((iLenThisDescription / 2)- (iLenRightDescription + 1))/2, wxT(' '));
        }
        // First, pad strThisDescription with iLenDescription
        _strDescription = Pad(strThisDescription, iLenDescription);
        // Construct tree lines
        _strDescription += _T("\n") + etkString(_T(' '), (iLenDescription - 1) / 2)
                                      + _T('|')
                                      + etkString(_T(' '), iLenDescription / 2)
                         + _T("\n") + etkString(_T(' '), strPadAllLeftLeft.Len() + (iLenLeftDescription - 1) / 2)
                                      + etkString(_T('-'), 2 + (iLenLeftDescription / 2) + strPadAllLeftRight.Len() + 3 + strPadAllRightLeft.Len() + (iLenRightDescription - 1) / 2)
                                      + etkString(_T(' '), iLenRightDescription / 2 + strPadAllRightRight.Len())
                         + _T("\n") + etkString(_T(' '), strPadAllLeftLeft.Len() + (iLenLeftDescription - 1) / 2)
                                    + _T('|')
                                    + etkString(_T(' '), (iLenLeftDescription / 2) + strPadAllLeftRight.Len() + 3 + strPadAllRightLeft.Len() + (iLenRightDescription - 1) / 2)
                                    + _T('|')
                                    + etkString(_T(' '), iLenRightDescription / 2 + strPadAllRightRight.Len());
        // Make fusion between 2 expressions
        wxStringTokenizer tkzLeft(strLeftDescription, _T("\n"));
        wxStringTokenizer tkzRight(strRightDescription, _T("\n"));
        while (tkzLeft.HasMoreTokens() || tkzRight.HasMoreTokens())
        {
            etkString strTokenLeft, strTokenRight;
            if (tkzLeft.HasMoreTokens())
            {
                strTokenLeft = tkzLeft.GetNextToken();
            }
            else
            {   // Depth of left expression is lower than right
                strTokenLeft.Pad(iLenLeftDescription, _T(' '));
            }
            if (tkzRight.HasMoreTokens())
            {
                strTokenRight = tkzRight.GetNextToken();
            }
            else
            {   // Depth of right expression is lower than left
                strTokenRight.Pad(iLenRightDescription, _T(' '));
            }
            _strDescription += etkString(_T('\n'), 1) + strPadAllLeftLeft + strTokenLeft + strPadAllLeftRight + etkString(_T(' '), 3) + strPadAllRightLeft + strTokenRight + strPadAllRightRight;
        }
        iLenThisDescription = iLenDescription;
    }

    return iLenThisDescription;
}

int ETKSQLite3Expression::ConstructContentThisDescription(etkString &_strDescription) const
{
    int iMaxLength;
    if (this == nullptr) // I know it is ugly :-) Just as debug, simplify tree explorer
    {   // If accessed on one member on this class, the software crash (access violation)
        _strDescription = _T("(null) ");
        iMaxLength = 7;
    }
    else
    {
        etkString strVariantID    = etkString::Format(_T("[ID=%lu]"), m_ulCurrentId);
        etkString strDescription  = GetOperatorString();
        etkString strVariantType  = _T("(Type=") + GetVariantDebug().GetType() + _T(")");
        etkString strVariantValue = _T("\"") + GetVariantDebug().GetString() + _T("\"");
        strVariantValue.Replace(_T("\t"), _T("\\t"));
        strVariantValue.Replace(_T("\r"), _T("\\r"));
        strVariantValue.Replace(_T("\n"), _T("\\n"));

        iMaxLength = wxMax(strVariantID.Len(), strDescription.Len());
        iMaxLength = wxMax(iMaxLength, (int) strVariantType.Len());
        iMaxLength = wxMax(iMaxLength, (int) strVariantValue.Len());
        if (iMaxLength % 2 == 0)
        {
            ++iMaxLength;
        }
        strVariantID.Pad((iMaxLength - strVariantID.Len())/2, _T(' '), false);  // Pad left
        strVariantID.Pad(iMaxLength - strVariantID.Len(), _T(' '), true);   // Pad right
        strDescription.Pad((iMaxLength - strDescription.Len())/2, _T(' '), false);  // Pad left
        strDescription.Pad(iMaxLength - strDescription.Len(), _T(' '), true);   // Pad right
        strVariantType.Pad((iMaxLength - strVariantType.Len())/2, _T(' '), false);  // Pad left
        strVariantType.Pad(iMaxLength - strVariantType.Len(), _T(' '), true);   // Pad right
        strVariantValue.Pad((iMaxLength - strVariantValue.Len())/2, _T(' '), false); // Pad left
        strVariantValue.Pad(iMaxLength - strVariantValue.Len(), _T(' '), true);  // Pad right
        _strDescription = strVariantID + _T("\n") + strDescription + _T("\n") + strVariantType + _T("\n") + strVariantValue;
    }

    return iMaxLength;
}

etkString ETKSQLite3Expression::GetOperatorString() const
{
    etkString strType;
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
        case eOperationExists :
        {
            strType = _T("eOperationExists");
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
        case eOperationAsSelectFrom :
        {
            strType = _T("eOperationAsSelectFrom");
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
        default:
        {
            wxASSERT(false); // Code missing
        }
    }
    return strType;
}

etkString ETKSQLite3Expression::Pad(const etkString _strString, int iLenString)
{
    etkString strPaddedString;
    wxStringTokenizer tkz(_strString, _T("\n")); //wxTOKEN_RET_EMPTY_ALL
    while (tkz.HasMoreTokens())
    {
        etkString strToken = tkz.GetNextToken();

        wxASSERT(((int) strToken.Len())<=iLenString); // With algorithm, must never happened !
        strToken.Pad((iLenString - strToken.Len()    )/2, _T(' '), false); // Pad left
        strToken.Pad(iLenString - strToken.Len(), _T(' '), true);  // Pad right
        strPaddedString += (strPaddedString.IsEmpty() ? _T("") : _T("\n")) + strToken;
    }

    return strPaddedString;
}

#endif // __WXDEBUG__

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                         ETKSQLite3Criterion                                           //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

ETKSQLite3Criterion::ETKSQLite3Criterion(eRequestType _eRequestType)
    : m_bDistinct(false)
    , m_iLimit(0)
    , m_eRequestType(_eRequestType)
{
}

ETKSQLite3Criterion::ETKSQLite3Criterion(const ETKSQLite3Expression& _rExpression)
{
    operator=(_rExpression);
}

ETKSQLite3Criterion::ETKSQLite3Criterion(const ETKSQLite3Criterion& _rCriterion)
{
    operator=(_rCriterion);
}

ETKSQLite3Criterion::ETKSQLite3Criterion(const ETKSQLite3RequestSelector& _rSelector)
{
    operator=(_rSelector);
}

ETKSQLite3Criterion::~ETKSQLite3Criterion()
{
}

ETKSQLite3Criterion::eRequestType ETKSQLite3Criterion::GetRequestType() const
{
    return m_eRequestType;
}

void ETKSQLite3Criterion::SetRequestType(ETKSQLite3Criterion::eRequestType _eRequestType)
{
    m_eRequestType = _eRequestType;
}

const ETKSQLite3Criterion& ETKSQLite3Criterion::operator=(const ETKSQLite3Expression& _rExpresion)
{
    if (_rExpresion.IsJoin())
    {
        SetJoin(GetJoin() && _rExpresion);
    }
    else
    {
        ETKSQLite3Expression::operator=(_rExpresion); // Call = on base class
    }
    SetDistinct(false); // Put distinct value to default
    SetOrderBy();       // Put order by value to default

    return *this;
}

const ETKSQLite3Criterion& ETKSQLite3Criterion::operator=(const ETKSQLite3Criterion& _rCriterion)
{
    m_eRequestType = _rCriterion.m_eRequestType;
    SetDistinct(_rCriterion.GetDistinct()); // Copy distinct
    SetOrderBy(_rCriterion.GetOrderBy());   // Copy order
    SetJoin(_rCriterion.GetJoin());         // Copy joins criterion
    SetWhere(_rCriterion.GetWhere());       // Copy Where criterion
    SetFrom(_rCriterion.GetFrom());         // Copy From criterion
    SetLimit(_rCriterion.GetLimit());       // Copy Limit criterion
    SetGroupBy(_rCriterion.GetGroupBy());   // Copy Group By criterion

    ETKSQLite3Expression::operator=(_rCriterion); // Call = on base class
    return *this;
}

const ETKSQLite3Criterion& ETKSQLite3Criterion::operator=(const ETKSQLite3RequestSelector& _rSelector)
{
    ETKSQLite3Expression::operator=(_rSelector);
    return *this;
}

ETKSQLite3Criterion ETKSQLite3Criterion::operator&&(const ETKSQLite3Criterion & _rCriterion) const
{
    ETKSQLite3Criterion critere(ETKSQLite3Expression::operator&&(_rCriterion));

    critere.SetDistinct(GetDistinct());                 // Copy distinct
    critere.SetOrderBy(GetOrderBy());                   // Copy order
    critere.SetJoin(GetJoin() && _rCriterion.GetJoin());// Copy joins criterion

    return critere;
}

ETKSQLite3Criterion ETKSQLite3Criterion::operator&&(const ETKSQLite3Expression& _rExpresion) const
{
    ETKSQLite3Criterion critere(ETKSQLite3Expression::operator&&(_rExpresion));

    critere.SetDistinct(GetDistinct()); // Copy distinct
    critere.SetOrderBy(GetOrderBy());   // Copy order
    critere.SetJoin(GetJoin());         // Copy joins criterion

    return critere;
}

void ETKSQLite3Criterion::Clear()
{
    ETKSQLite3Expression::Clear();
    m_exprOrderBy.Clear();
    m_exprJoin.Clear();
    m_exprWhere.Clear();
    m_exprFrom.Clear();
    m_exprGroupBy.Clear();
    m_bDistinct = false;
}


ETKSQLite3Criterion & ETKSQLite3Criterion::AddOrderByAscending(const ETKSQLite3Column& _rColumn)
{
    m_exprOrderBy.Add(ETKSQLite3Expression(_rColumn.GetFullName(), eExpressionOrderByASC));
    return *this;
}

ETKSQLite3Criterion & ETKSQLite3Criterion::AddOrderByDescending(const ETKSQLite3Column& _rColumn)
{
    m_exprOrderBy.Add(ETKSQLite3Expression(_rColumn.GetFullName(), eExpressionOrderByDESC));
    return *this;
}

void ETKSQLite3Criterion::AddOrderbyList(const ETKSQLite3Expression& _rExpresion)
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

ETKSQLite3Criterion & ETKSQLite3Criterion::Distinct()
{
    SetDistinct(true);
    return *this;
}

ETKSQLite3Criterion::tdStringList & ETKSQLite3Criterion::UpdateTablesList(tdStringList &_rlstTablesList) const
{
    ETKSQLite3Expression::UpdateTablesList(_rlstTablesList);
    tdStringList lstTablesList;
    GetWhere().UpdateTablesList(lstTablesList);
    GetJoin().UpdateTablesList(lstTablesList);
    for (tdStringList::const_iterator it = lstTablesList.begin();it != lstTablesList.end(); ++it)
    {
        if (_rlstTablesList.find(it->first) == _rlstTablesList.end())
        {
            _rlstTablesList[it->first] = false; // Not added to FROM (false)
        }
    }

    return _rlstTablesList;
}

void ETKSQLite3Criterion::SetDistinct(bool _bDistinct)
{
    m_bDistinct = _bDistinct;
}

bool ETKSQLite3Criterion::GetDistinct() const
{
    return m_bDistinct;
}

void ETKSQLite3Criterion::SetLimit(int _iLimit)
{
    m_iLimit = _iLimit;
}

int ETKSQLite3Criterion::GetLimit() const
{
    return m_iLimit;
}

void ETKSQLite3Criterion::SetOrderBy(const ETKSQLite3Expression &_rExprOrderBy)
{
    m_exprOrderBy = _rExprOrderBy;
}

const ETKSQLite3Expression & ETKSQLite3Criterion::GetOrderBy() const
{
    return m_exprOrderBy;
}

void ETKSQLite3Criterion::SetJoin(const ETKSQLite3Expression &_rExprJoin)
{
    m_exprJoin = _rExprJoin;
}

const ETKSQLite3Expression & ETKSQLite3Criterion::GetJoin() const
{
    return m_exprJoin;
}

ETKSQLite3Expression & ETKSQLite3Criterion::AddGroupBy(const ETKSQLite3Expression &_rGroupBy)
{
    m_exprGroupBy.Add(_rGroupBy);
    return m_exprGroupBy;
}

void ETKSQLite3Criterion::SetGroupBy(const ETKSQLite3Expression &_rExprGroupBy)
{
    m_exprGroupBy = _rExprGroupBy;
}

const ETKSQLite3Expression & ETKSQLite3Criterion::GetGroupBy() const
{
    return m_exprGroupBy;
}

void ETKSQLite3Criterion::SetWhere(const ETKSQLite3Expression &_rExprWhere)
{
    // Current where MUST be nullptr !! Else it is initialize twice with where criterion
    // wxASSERT_MSG(m_exprWhere.IsNull(), wxT("WHERE is initialized more than once!"));
    // => Could change the where into the request, the wxASSERT_MSG is removed!
    wxASSERT_MSG(   GetRequestType() == eRequestTypeDelete
                 || GetRequestType() == eRequestTypeUpdate
                 || GetRequestType() == eRequestTypeSelect , wxT("Bad type request for WHERE initialization!"));

    m_exprWhere = _rExprWhere;
}

const ETKSQLite3Expression & ETKSQLite3Criterion::GetWhere() const
{
    return m_exprWhere;
}

void ETKSQLite3Criterion::SetFrom(const ETKSQLite3Expression &_rExprFrom)
{
    // Current from MUST be nullptr !! Else it is initialize twice with where criterion
    wxASSERT_MSG(m_exprFrom.IsNull(), wxT("WHERE is initialized more than once!"));
    wxASSERT_MSG(   GetRequestType() == eRequestTypeDelete
                 || GetRequestType() == eRequestTypeUpdate
                 || GetRequestType() == eRequestTypeSelect , wxT("Bad type request for WHERE initialization!"));

    m_exprFrom = _rExprFrom;
}

const ETKSQLite3Expression & ETKSQLite3Criterion::GetFrom() const
{
    return m_exprFrom;
}

etkString ETKSQLite3Criterion::GetSQL() const
{
    tdStringList lstTables;     // Table list for from or insert
    etkString strSQLRequest;

    if (m_exprFrom.IsNull())
    {
        UpdateTablesList(lstTables);
    }
    else
    {
        if (m_exprFrom.GetExpressionOrOperationType() == eExpressionTypeValueQuoteString)
        {   // Programmer has just add like "Table1, Table2, Table3"
            etkString str = m_exprFrom.GetVariant().GetString();
            wxStringTokenizer tkzTables(m_exprFrom.GetVariant().GetString(), _T(","));
            while (tkzTables.HasMoreTokens())
            {
                // Add element (if exist) and initialize the value to true, if the element already exists, nothing is modified
                lstTables.insert(tdStringList::value_type(tkzTables.GetNextToken(), true));
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
            wxASSERT(GetWhere().IsNull());  // Must be nullptr on insert
            wxASSERT(GetJoin().IsNull());   // Must be nullptr on insert
            #ifdef __WXDEBUG__
            if (lstTables.size() != 1)
            {
                etkString strErrorMessage(etkString::Format(wxT("Insert request must have only ONE table, it have %d table(s)!"), (int) lstTables.size()));
                int iIndexTable = 0;
                for (tdStringList::iterator it = lstTables.begin(); it != lstTables.end(); ++it)
                {
                    strErrorMessage += etkString::Format(wxT("\nTable[%d] = '%s'"), iIndexTable, ((*it).first).wx_str());
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
                strSQLRequest += etkString::Format(_T(" WHERE (%s)"), GetWhere().FormatRequest().c_str());
            }
            break;
        }
        case eRequestTypeUpdate:
        {
            wxASSERT_MSG(lstTables.size() == 1, wxT("Update request must have only ONE table!"));

            strSQLRequest.Printf(   _T("UPDATE %s SET %s"),
                                    FormatSetAsString(lstTables).wx_str(),
                                    FormatRequest().wx_str()
                                );

            // Request contains WHERE ?
            if (!GetWhere().IsNull())
            {
                strSQLRequest += etkString::Format(_T(" WHERE (%s)"), GetWhere().FormatRequest().c_str());
            }
            break;
        }
        case eRequestTypeSelect:
        {
            strSQLRequest.Printf(   _T("SELECT %s%s FROM %s"),
                                    GetDistinct() ? _T("DISTINCT ") : _T(""),
                                    ETKSQLite3Expression::FormatRequest().c_str(),
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
                strSQLRequest += etkString::Format(_T(" WHERE (%s)"), GetWhere().FormatRequest().c_str());
            }

            if (!GetGroupBy().IsNull())
            {
                strSQLRequest += etkString::Format(_T(" GROUP BY %s"), GetGroupBy().FormatRequest().c_str());
            }

            // Request contains ORDER BY ?
            if (CanHaveOrderBy())
            {   // Only if it is not an operator like MIN/MAX/COUNT/ etc...
                strSQLRequest += GetOrderByAsString(); // Add ORDER BY to others if defined
            }

            if (GetLimit() != 0)
            {
                strSQLRequest += GetLimitAsString(); // Add LIMIT to others if defined
            }

            break;
        }
    }

    return strSQLRequest;
}

void ETKSQLite3Criterion::BindTo(wxSQLite3Statement &_rstmt, int &_riIndex) const
{
    ETKSQLite3Expression::BindTo(_rstmt, _riIndex, m_eRequestType == eRequestTypeInsert);
    GetWhere().BindTo(_rstmt, _riIndex, m_eRequestType == eRequestTypeInsert);
    GetJoin().BindTo(_rstmt, _riIndex, m_eRequestType == eRequestTypeInsert);
}

etkString ETKSQLite3Criterion::GetOrderByAsString() const
{
    etkString strOrderByRequest;

    if (!GetOrderBy().IsNull())
    {
        strOrderByRequest = _T(" ORDER BY ") + m_exprOrderBy.FormatRequest();
    }

    return strOrderByRequest;
}

etkString ETKSQLite3Criterion::GetLimitAsString() const
{
    etkString strLimitRequest;

    if (GetLimit() != 0)
    {
        strLimitRequest = etkString::Format(_T(" LIMIT %d"), GetLimit());
    }

    return strLimitRequest;
}

bool ETKSQLite3Criterion::IsNull() const
{
    return !GetDistinct() && GetOrderBy().IsNull() && ETKSQLite3Expression::IsNull();
}

bool ETKSQLite3Criterion::IsValid() const
{
    return !ETKSQLite3Expression::IsNull();
}

etkString ETKSQLite3Criterion::FormatSetAsString(const tdStringList &_rlstDatas)
{   // List elements of set, separate from each others with comma (',')
    return std::for_each(_rlstDatas.begin(), _rlstDatas.end(), ETKSQLite3StringConcat()).m_strValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                   ETKSQLite3Expression::operator                                      //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT_IMPORT ETKSQLite3Expression operator==(const ETKSQLite3Value &_rValue, const ETKSQLite3Column &_rColumn)
{
    return ETKSQLite3Expression(_rValue) == _rColumn;
}

EXPORT_IMPORT ETKSQLite3Expression operator>(const ETKSQLite3Value &_rValue, const ETKSQLite3Column &_rColumn)
{
    return ETKSQLite3Expression(_rValue) > _rColumn;
}

EXPORT_IMPORT ETKSQLite3Expression operator>=(const ETKSQLite3Value &_rValue, const ETKSQLite3Column &_rColumn)
{
    return ETKSQLite3Expression(_rValue) >= _rColumn;
}

EXPORT_IMPORT ETKSQLite3Expression operator<(const ETKSQLite3Value &_rValue, const ETKSQLite3Column &_rColumn)
{
    return ETKSQLite3Expression(_rValue) < _rColumn;
}

EXPORT_IMPORT ETKSQLite3Expression operator<=(const ETKSQLite3Value &_rValue, const ETKSQLite3Column &_rColumn)
{
    return ETKSQLite3Expression(_rValue) <= _rColumn;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                          Functions                                                    //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

ETKSQLite3Expression dbCount()
{
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationCount);
}

ETKSQLite3Expression dbExists(const ETKSQLite3RequestSelector& _rSelector)
{
    return dbExists(_rSelector.GetCriterionRequest());
}

ETKSQLite3Expression dbExists(const ETKSQLite3Criterion& _rCriterion)
{
    return dbExists(ETKSQLite3Expression(dbSQL(_rCriterion.GetSQL())));
}

ETKSQLite3Expression dbExists(const ETKSQLite3Expression& _rExpression)
{
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationExists, _rExpression, nullptr);
}

ETKSQLite3Expression dbCount(const ETKSQLite3Column& _rColumn)
{
    ETKSQLite3Expression expression(_rColumn);
    expression.SetExpressionOrOperationType(ETKSQLite3Expression::eOperationCount);
    return expression;
}

ETKSQLite3Expression dbCount(const ETKSQLite3Expression& _rExpression)
{
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationCount, _rExpression, ETKSQLite3Expression());
}

ETKSQLite3Expression dbDistinct(const ETKSQLite3Column& _rColumn)
{
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationDistinct, ETKSQLite3Expression(_rColumn), ETKSQLite3Expression());
}

ETKSQLite3Expression dbMax(const ETKSQLite3Column& _rColumn)
{
    ETKSQLite3Expression expression(_rColumn);
    expression.SetExpressionOrOperationType(ETKSQLite3Expression::eOperationMax);
    return expression;
}

ETKSQLite3Expression dbMin(const ETKSQLite3Column& _rColumn)
{
    ETKSQLite3Expression expression(_rColumn);
    expression.SetExpressionOrOperationType(ETKSQLite3Expression::eOperationMin);
    return expression;
}

ETKSQLite3Expression dbSum(const ETKSQLite3Column& _rColumn)
{
    ETKSQLite3Expression expression(_rColumn);
    expression.SetExpressionOrOperationType(ETKSQLite3Expression::eOperationSum);
    return expression;
}

ETKSQLite3Expression dbCast(const ETKSQLite3Column& _rColumn, etkString _strCast)
{
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationCast, ETKSQLite3Expression(ETKSQLite3Value(_rColumn.GetFullName(), ETKSQLite3Expression::eExpressionColumnNameFull)), ETKSQLite3Expression(_strCast, ETKSQLite3Expression::eExpressionTypeValue));
}

ETKSQLite3Expression dbSubString(const ETKSQLite3Expression &_rExpression, const ETKSQLite3Expression &_rExpressionStart, const ETKSQLite3Expression &_rExpressionLength)
{   // Little bit complex: format it directly into SQL cstring: we need 3 parameters for this operator and all expressions have only 2
    etkString strSql;
    strSql.Printf(_T("SUBSTR(%s,%s,%s)"), _rExpression.FormatRequest().c_str(), _rExpressionStart.FormatRequest().c_str(), _rExpressionLength.FormatRequest().c_str());
    return ETKSQLite3Expression(strSql, ETKSQLite3Expression::eExpressionTypeSQL);
}

ETKSQLite3Expression dbAs(const ETKSQLite3Expression& _rExpression, etkString _strAsName)
{
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationAs, _rExpression, ETKSQLite3Value(_strAsName, ETKSQLite3Expression::eExpressionTypeValue));
}

ETKSQLite3Expression dbAs(const ETKSQLite3Criterion& _rCriterion, etkString _strAsName)
{
    ETKSQLite3Expression exprAdd;

    exprAdd.Add(_rCriterion);
    return ETKSQLite3Expression(ETKSQLite3Expression::eOperationAsSelect, exprAdd, ETKSQLite3Value(_strAsName, ETKSQLite3Expression::eExpressionTypeValue));
}

ETKSQLite3Expression dbAs(const ETKSQLite3Record &_rRecord, etkString _strAsName)
{
    return _rRecord.As(_strAsName);
}

ETKSQLite3Expression dbSQL(etkString _strSQLRequest)
{
    return ETKSQLite3Value(_strSQLRequest, ETKSQLite3Expression::eExpressionTypeSQL);
}

ETKSQLite3Column dbAsColumn(const ETKSQLite3Column& _rColumn, etkString _strAs)
{
    return ETKSQLite3Column(_strAs, _rColumn.GetColumnName());
}