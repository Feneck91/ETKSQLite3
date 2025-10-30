/////////////////////////////////////////////////////////////////////////////
/**
 * @file ETKSQLite3Criterion.h
 * @brief Header file for SQL request creation.
 *
 * This file contains the headers for the ETKSQLite3Value,
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
#ifndef INCLUDE_ETK_SQLITE3_CRITERION_H
#define INCLUDE_ETK_SQLITE3_CRITERION_H

#include "ETKSQLite3Types.h"
#include <map>
#ifdef __WXDEBUG__
    #include <vector>
#endif

class ETKSQLite3Column;
class ETKSQLite3ColumnAttributes;
class ETKSQLite3ValueBind;
class ETKSQLite3RequestSelector;
class ETKSQLite3Criterion;
class ETKSQLite3Record;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                          ETKSQLite3Value                                              //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Classe that contains only one base element.
 *
 * This one contains one value (that could be empty) and an operator used to define values operations.
 *
 *
 * @author Stéphane Château
 * @version Nom : ETKSQLite3<br>
 *          Révision : <b>1.0</b>
 */
class EXPORT_IMPORT ETKSQLite3Value
{
public:
    /**
     * Value type or operator.
     *
     * <ul>
     *   <li> <b>eExpressionTypeNothing</b>: Empty.
     *   <li> <b>eExpressionColumnNameFull</b>: Field of type <b>TABLE.FIELD</b> (table column with table name).
     *   <li> <b>eExpressionColumnNameOnly</b>: Field of type <b>FIELD</b> only (table column name without table name).
     *   <li> <b>eExpressionColumnAttributes</b>: Contains the value of a column and how to bind it.
     *   <li> <b>eExpressionValueBind</b>: Contains the value and how to bind it.
     *   <li> <b>eExpressionRequestSelector</b>: Contains a selector type value (into the variant).
     *   <li> <b>eExpressionOrderByASC</b>: Ascending sort order.
     *   <li> <b>eExpressionOrderByDESC</b>: Descending sort order.
     *   <li> <b>eExpressionTypeValue</b>: Value.
     *   <li> <b>eExpressionTypeValueQuoteString</b>: Value or string type, is quoted by <b>'</b> char (example : 'xxx') opposite of eExpressionTypeValue.
     *   <li> <b>eExpressionTypeSQL</b>: String already formatted in SQL, put it like it.
     *   <li> <b>eOperationOr</b>: Logical OR between two expression, in this case the value is not used.
     *   <li> <b>eOperationAnd</b>: Logical AND between two expression, in this case the value is not used.
     *   <li> <b>eOperationNot</b>: Logical AND on one expression.
     *   <li> <b>eOperationUpper</b>: Upper strict.
     *   <li> <b>eOperationUpperOrEqual</b>:  Upper or egal.
     *   <li> <b>eOperationLower</b>: Lesser strict.
     *   <li> <b>eOperationLowerOrEqual</b>: Lesser or egal.
     *   <li> <b>eOperationEqual</b>: Equality between two expressions, in this case the value is not used.
     *   <li> <b>eOperationLike</b>: LIKE  between two expressions, in this case the value is not used.
     *   <li> <b>eOperationAs</b>: AS between two expressions, in this case the value is not used.
     *   <li> <b>eOperationAsSelect</b>: AS into selector (use << or Add), in this case only the keyword AS is generated else not.
     *   <li> <b>eOperationAsSelectFrom</b>: AS into selector (use << dbAs(const ETKSQLite3Record &) with record.
     *   <li> <b>eOperationAsJoin</b>: AS  between two expressions used after JOIN keyword, the value is the table name of original table.
     *   <li> <b>eOperationDifferent</b>: DIFFERENCE between two expressions, in this case the value is not used.
     *   <li> <b>eOperationCount</b>: Used into request operator to count the record number.
     *   <li> <b>eOperationMin</b>: Used into request operator to get the min record.
     *   <li> <b>eOperationMax</b>: Used into request operator to get the max record.
     *   <li> <b>eOperationSum</b>: Used into request operator to get the sum record.
     *   <li> <b>eOperationAssignmentInsert</b>: To assign value to a column, used into insert request.
     *   <li> <b>eOperationAssignmentUpdate</b>: To assign value to a column, used into update request.
     *   <li> <b>eOperationAndJoin</b>: Used when more than once join are used (into && when the expression this is already a join).
     *   <li> <b>eOperationJoin</b>: Join between tables (colonne1 == colonne2).
     *   <li> <b>eOperationInnerJoin</b>: Inner Join between tables (colonne1 == colonne2).
     *   <li> <b>eOperationLeftJoin</b>: Left join between tables (colonne1 == colonne2).
     *   <li> <b>eOperationRightJoin</b>: Right join between tables (colonne1 == colonne2). <b>Currently not supported by sqlite3</b>.
     *   <li> <b>eOperationFullJoin</b>: Right join between tables (colonne1 == colonne2). <b>Currently not supported by sqlite3</b>.
     *   <li> <b>eOperationIn</b>: IN request type.
     *   <li> <b>eOperationCast</b>: Cast request type.
     *   <li> <b>eOperationIsNull</b>: IS NULL request type.
     *   <li> <b>eOperationIsNotNull</b>: IS NOT NULL request type.
     *   <li> <b>eOperationMult</b>: Multiplication.
     *   <li> <b>eOperationDiv</b>: Division.
     *   <li> <b>eOperationAdd</b>: Addition.
     *   <li> <b>eOperationSub</b>: Subtraction.
     *   <li> <b>eOperationAddComma</b>: Add element one after other, separate by comma.
     *   <li> <b>eOperationDistinct</b>: Distinct (can be used for count of columns).
     * </ul>
     */
    enum eExpressionOrOperationType
    {
        eExpressionTypeNothing,
        eExpressionColumnNameFull,
        eExpressionColumnNameOnly,
        eExpressionColumnAttributes,
        eExpressionValueBind,
        eExpressionRequestSelector,
        eExpressionOrderByASC,
        eExpressionOrderByDESC,
        eExpressionTypeValue,
        eExpressionTypeValueQuoteString,
        eExpressionTypeSQL,
        eOperationOr,
        eOperationAnd,
        eOperationNot,
        eOperationUpper,
        eOperationUpperOrEqual,
        eOperationLower,
        eOperationLowerOrEqual,
        eOperationEqual,
        eOperationExists,
        eOperationLike,
        eOperationAs,
        eOperationAsSelect,
        eOperationAsSelectFrom,
        eOperationAsJoin,
        eOperationDifferent,
        eOperationCount,
        eOperationMin,
        eOperationMax,
        eOperationSum,
        eOperationAssignmentInsert,
        eOperationAssignmentUpdate,
        eOperationAndJoin,
        eOperationJoin,
        eOperationInnerJoin,
        eOperationLeftJoin,
        eOperationRightJoin,
        eOperationFullJoin,
        eOperationIn,
        eOperationCast,
        eOperationIsNull,
        eOperationIsNotNull,
        eOperationMult,
        eOperationDiv,
        eOperationAdd,
        eOperationSub,
        eOperationAddComma,
        eOperationDistinct
    };

private:
    /**
     * Value.
     *
     * This value could be ignored, depending of kind operator.
     */
    etkVariant                          m_Value;

    /**
     *  Expression type, explain how to use (or not) the value and the child expressions.
     *
     */
    eExpressionOrOperationType          m_ExpressionType;

public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Default constructor.
     *
     * This default constructor constuct an empty value with only a type expression.
     *
     * @param _expressionType Expression type, by default is eExpressionTypeNothing.
     */
    ETKSQLite3Value(eExpressionOrOperationType _expressionType = eExpressionTypeNothing);

    /**
     * Copy constructor.
     *
     * @param _rOther Element to copy into this.
     */
    ETKSQLite3Value(const ETKSQLite3Value &_rOther);

    /**
     * Create a value with colum attribute.
     *
     * The column attribute doesn't contains any column name but contains column data and how to bind it.
     *
     * @param _rColumnAttributes Contains some column attribute and the binded data.
     */
    ETKSQLite3Value(const ETKSQLite3ColumnAttributes &_rColumnAttributes);

    /**
     * Create a binding value.
     *
     * This binding value contains data and how to bind it.
     *
     * @param _rValueBind Contains value data and class that record how to bind it.
     */
    ETKSQLite3Value(const ETKSQLite3ValueBind &_rValueBind);

    /**
     * Create a value with selector expression.
     *
     * Use, for example, to make IN (so, with selection request into the In criterion).
     *
     * @param _rSelector Selector to record into this.
     */
    ETKSQLite3Value(const ETKSQLite3RequestSelector& _rSelector);

    /**
     * Create value only.
     *
     * Value is always recorded into variant.
     *
     * @param _rValue Variant value to record.
     */
    ETKSQLite3Value(const etkVariant &_rValue);

    /**
     * Create value only.
     *
     * Value is always recorded into variant.
     *
     * @param _rtmValue Date/Time into struct tm. Internal converted in UTC date/time.
     */
    ETKSQLite3Value(const struct tm &_rtmValue);

    /**
     * Create value only.
     *
     * Value is always recorded into variant.
     *
     * @param _rdtValue Date/Time wxWidgets date/time class. Internal converted in UTC date/time.
     */
    ETKSQLite3Value(const etkDateTime &_rdtValue);

    /**
     * Create value only.
     *
     * Value is always recorded into variant.
     *
     * @param _lValue long value.
     */
    ETKSQLite3Value(long _lValue);

    /**
     * Create value only.
     *
     * Value is always recorded into variant.
     *
     * @param _iValue int value.
     */
    ETKSQLite3Value(int _iValue);

    /**
     * Create value only.
     *
     * Value is always recorded into variant.
     *
     * @param _dValue double value.
     */
    ETKSQLite3Value(double _dValue);

    /**
     * Create value only.
     *
     * Value is always recorded into variant.
     *
     * @param _ri64Value long long value (64 bits).
     */
    ETKSQLite3Value(const etkInt64 &_ri64Value);

    /**
     * Create value only.
     *
     * Value is always recorded into variant.
     *
     * @param _pszValue string value.
     * @param _expressionType Expression type, by default use quote string but it can be other type. No check is done
     *                        to check validity expression type.
     */
    ETKSQLite3Value(const wxChar *_pszValue, eExpressionOrOperationType _expressionType = eExpressionTypeValueQuoteString);  // Or eExpressionTypeValue for column name (for example)

    /**
     * Create value only.
     *
     * Value is always recorded into variant.
     *
     * @param _strValue string value.
     * @param _expressionType Expression type, by default use quote string but it can be other type. No check is done
     *                        to check validity expression type.
     */
    ETKSQLite3Value(etkString _strValue, eExpressionOrOperationType _expressionType = eExpressionTypeValueQuoteString);  // Or eExpressionTypeValue for column name (for example)

    /**
     * Destructor.
     */
    virtual ~ETKSQLite3Value();
    //@}

    /// @name Operators.
    //@{
    /**
     * Assignment operator.
     *
     * @param _rExpression {const ETKSQLite3Value &} - Value to copy into this.
     * @return Reference to this.
     */
    const ETKSQLite3Value &             operator=(const ETKSQLite3Value &_rExpression);
    //@}

    /**
     * Get the value as string.
     *
     * @return The expression as string.
     */
    virtual etkString                   GetAsString() const;

    /**
     * Get the column name without the table name.
     *
     * Used only on eExpressionColumnNameFull or  eExpressionColumnNameOnly expression type.
     *
     * @return The column name as string, without table name.
     */
    etkString                           GetColumnName() const;

    /**
     * Get the table name without the column name.
     *
     * Used only on eExpressionColumnName or eExpressionColumnNameOnly expression type.
     *
     * @return The table name as string, without column name.
     */
    etkString                           GetTableName() const;

    /**
     * Get the variant.
     *
     * Be careful, use only on values else an assert is raised.
     *
     * @return Get the value variant.
     */
    const etkVariant &                  GetVariant() const;

#ifdef __WXDEBUG__
    /**
     * Get the variant.
     *
     * Debug version, use to log all expressions tree, don't raise assert.
     * Called by GetOperatorString function.
     * This function doesn't exist un release mode, <b>only avalaible in debug mode</b>.<br>
     *
     * @return Get the value variant.
     * @see GetOperatorString
     */
    const etkVariant &                  GetVariantDebug() const;
#endif

    /**
     * Test is value is empty.
     *
     * @return true of the value is empty, false else.
     */
    bool                                IsValueEmpty() const;

    /**
     * Get the data type or operation type.
     *
     * @return The expression type.
     */
    eExpressionOrOperationType          GetExpressionOrOperationType() const;

    /**
     * Init the data or operation (expression) type.
     *
     * @param _expressionType Expression type to set.
     */
    void                                SetExpressionOrOperationType(eExpressionOrOperationType _expressionType);

    /**
     * Clear this content.
     *
     * After calling this function, this instance can be reuse.
     */
    void                                Clear();
protected:
    /**
     * Bind a column data of a statement.
     *
     * @param _rstmt Statement to bind to.
     * @param _riIndex Index of item to bind. If a bind is done, increment index.
     * @param _bForInsertRequest If true if it's bind for an insert request (for INSERT request, the primary key is computed and should be null).
     */
    virtual void                        BindTo(wxSQLite3Statement &_rstmt, int &_riIndex, bool _bForInsertRequest) const;

    /**
     * Used to know if the operation type is AND or OR.
     *
     * The AND could be eOperationAnd or eOperationAndJoin.
     * Is used to don't let create tree with empty elements.
     *
     * @return true is operation is AND or OR type.
     */
    bool                                IsAndOrOR() const;

    /**
     * String quote.
     *
     * In SQL request, all string are quoted ('). If into the string, a <b>'</b> char is present, this
     * one must be repeat twice else request syntax become invalid and exception is throw when request
     * is executed.
     *
     * @param _strValue String to quote.
     * @return Quoted string, can be same as entry one is no <b>'</b> is present.
     */
    etkString                           QuoteString(etkString _strValue) const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                       ETKSQLite3Expression                                            //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Each expression is composed with 0, 1 or 2 expressions depending of expression type.
 *
 * This expression could be a value, an unary operetor or an operator with 2 arguments.
 * Used to record 2 expressions that could be empty.
 *
 * @see ETKSQLite3Value
 *
 * @author Stéphane Château
 * @version Nom : ETKSQLite3<br>
 *          Révision : <b>1.0</b>
 */
class EXPORT_IMPORT ETKSQLite3Expression : public ETKSQLite3Value
{
public:
    /**
     * Type of list used to record table list.
     */
    typedef std::map<etkString, bool>   tdStringList;

protected:
    /**
     * Expression 1.
     *
     * Left expression when it is an expression of OR / AND / etc type that contains two expressions.
     * Unique expression when the operator contains only one element (like count, max, min, etc).
     */
    ETKSQLite3Expression *              m_pExpression1;

    /**
     * Expression 2.
     *
     * Right expression only used when it is an expression of OR / AND / etc type that contains two expressions.
     */
    ETKSQLite3Expression *              m_pExpression2;

#ifdef __WXDEBUG__
    /**
     * Unique counter identifier.
     *
     * Used in debug mode to identify each expression by a unique identifier.
     * Is incremented each time an expression is created.
     */
    static unsigned long                s_ulCurrentCounterId;

public:
    /**
     * Unique identifier.
     *
     * Used in debug mode to identify each expression by a unique identifier.
     */
    unsigned long                       m_ulCurrentId;
#endif
public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Default constructor.
     *
     * This default constructor constuct an empty value with only a type expression.
     *
     * @param _expressionType Expression type, by default is eExpressionTypeNothing.
     */
    ETKSQLite3Expression(eExpressionOrOperationType _expressionType = eExpressionTypeNothing);

    /**
     * Copy constructor.
     *
     * @param _rExpression Element to copy into this.
     */
    ETKSQLite3Expression(const ETKSQLite3Expression &_rExpression);

    /**
     * Construct an operation type.
     *
     * This constructor constuct an expression with an operation and two expressions but with no value.
     * Both expression could be empty or not. If expression 2 is not empty, expression 1 must not be empty!
     *
     * @param _expressionType Operation type.
     * @param _rExpression1 Expression 1 (left expression).
     * @param _rExpression2 Expression 2 (right expression).
     */
    ETKSQLite3Expression(eExpressionOrOperationType _expressionType, const ETKSQLite3Expression &_rExpression1, const ETKSQLite3Expression &_rExpression2);

    /**
     * Construct an operation type.
     *
     * This constructor constuct an expression with two expressions and a operation + value.
     * Both expression could be empty or not. If expression 2 is not empty, expression 1 must not be empty!
     *
     * @param _rValueAndExpressionType Contains value and operation type.
     * @param _rExpression1 Expression 1 (left expression).
     * @param _rExpression2 Expression 2 (right expression).
     */
    ETKSQLite3Expression(const ETKSQLite3Value &_rValueAndExpressionType, const ETKSQLite3Expression &_rExpression1, const ETKSQLite3Expression &_rExpression2);

    /**
     * Create an expression with a column (name).
     *
     * Only record \"<b>TABLE_NAME.COLUMN_NAME</b>\" with type equal to eExpressionColumnName.
     * This constructor don't make any bind datas.
     *
     * @param _rColumn Contains columns name and table name.
     * @param _expressionType Operation type, could be only eExpressionColumnNameFull or eExpressionColumnNameOnly.
     */
    ETKSQLite3Expression(const ETKSQLite3Column &_rColumn, eExpressionOrOperationType _expressionType=eExpressionColumnNameFull);

    /**
     * Create an expression with colum attribute.
     *
     * Used to bind values with '?'. It is the class <i>wxSQLite3Statement</i> that will make the binding after
     * has been initialized with correct binding indexex and types.
     *
     * @param _rColumnAttributes Contains some column attribute and the binded data.
     */
    ETKSQLite3Expression(const ETKSQLite3ColumnAttributes &_rColumnAttributes);

    /**
     * Create an expression with binding value.
     *
     * This binding value contains data and how to bind it.
     *
     * @param _rValueBind Contains value data and class that record how to bind it.
     */
    ETKSQLite3Expression(const ETKSQLite3ValueBind &_rValueBind);

    /**
     * Create an expression with a value.
     *
     * @param _rValue Contains value and type, calling copy constructor of this base class.
     */
     ETKSQLite3Expression(const ETKSQLite3Value &_rValue);

    /**
     * Create an expression with a value.
     *
     * @param _rValue Contains value, type is eExpressionTypeValue.
     */
    ETKSQLite3Expression(const etkVariant &_rValue);

    /**
     * Create an expression with a value.
     *
     * @param _rtmValue Date/Time into struct tm. Internal converted in UTC date/time. Type is eExpressionTypeValue.
     */
    ETKSQLite3Expression(const struct tm &_rtmValue);

    /**
     * Create an expression with a value.
     *
     * @param _rdtValue Date/Time wxWidgets date/time class. Internal converted in UTC date/time. Type is eExpressionTypeValue.
     */
    ETKSQLite3Expression(const etkDateTime &_rdtValue);

    /**
     * Create an expression with a value.
     *
     * @param _lValue long value. Type is eExpressionTypeValue.
     */
    ETKSQLite3Expression(long _lValue);

    /**
     * Create an expression with a value.
     *
     * @param _iValue int value. Type is eExpressionTypeValue.
     */
    ETKSQLite3Expression(int _iValue);

    /**
     * Create an expression with a value.
     *
     * @param _dValue double value. Type is eExpressionTypeValue.
     */
    ETKSQLite3Expression(double _dValue);

    /**
     * Create an expression with a value.
     *
     * @param _ri64Value long long value (64 bits).
     */
    ETKSQLite3Expression(const etkInt64 &_ri64Value);

    /**
     * Create an expression with a value.
     *
     * @param _pszValue string value.
     * @param _expressionType Expression type, by default use quote string but it can be other type. No check is done
     *                        to check expression type validity.
     */
    ETKSQLite3Expression(const wxChar *_pszValue, eExpressionOrOperationType _expressionType = eExpressionTypeValueQuoteString);

    /**
     * Create an expression with a value.
     *
     * @param _strValue string value.
     * @param _expressionType Expression type, by default use quote string but it can be other type. No check is done
     *                        to check expression type validity.
     */
    ETKSQLite3Expression(etkString _strValue, eExpressionOrOperationType _expressionType = eExpressionTypeValueQuoteString);

    /**
     * Constructor created from selection request.
     *
     * The content of selector is record as this into variant data.
     *
     * @param _rSelector Selector to copy into this.
     *                   Is used to make IN request tyoe (also with selector request into the In).
     */
    ETKSQLite3Expression(const ETKSQLite3RequestSelector& _rSelector);

    /**
     * Destructor.
     */
    virtual ~ETKSQLite3Expression();
    //@}

    /** @name Operators.
     *
     * Are used to make test on expression to generate new one.
     */
    //@{
    /**
     * Assignment operator.
     *
     * Be careful it is not an assigment operator as 'SQL' one, it is the C++ assignment operator. This function
     * clear all content and remplace content by new expression one.<br>
     * This function is also called by copy constructor.
     *
     * @param _rExpression Expression to copy into this.
     * @return contant reference to this.
     */
    const ETKSQLite3Expression &        operator=(const ETKSQLite3Expression &_rExpression);

    /**
     * Assignment operator from a selector.
     *
     * Use, for example, to make IN (so, with selection request into the In criterion).
     *
     * @param _rSelector Selector to copy into this.
     * @return Reference to this that contains assigned expression.
     */
    const ETKSQLite3Expression &        operator=(const ETKSQLite3RequestSelector& _rSelector);

    /**
     * Logical <b>OR</b> operator.
     *
     * Used to make logical operator into request between two expressions.
     *
     * @param _rExpression Expression used to make logical operation with this.
     * @return A new constructed expression with this + operator + expression.
     */
    ETKSQLite3Expression                operator||(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Logical <b>AND</b> operator.
     *
     * Used to make logical operator into request between two expressions.
     *
     * @param _rExpression Expression used to make logical operation with this.
     * @return A new constructed expression with this + operator + expression.
     */
    ETKSQLite3Expression                operator&&(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Logical <b>==</b> operator.
     *
     * Used to make logical operator into request between two expressions.
     *
     * @param _rExpression Expression used to make logical operation with this.
     * @return A new constructed expression with this + operator + expression.
     */
    ETKSQLite3Expression                operator==(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Logical <b>==</b> operator with column.
     *
     * Used to make logical operator into request between two expressions.
     * The full name of the column is recorded.
     *
     * @param _rColumn Column class that contains column and table name. Names are used to make logical operation with this.
     * @return A new constructed expression with this + operator + expression.
     */
    ETKSQLite3Expression                operator==(const ETKSQLite3Column &_rColumn) const;

    /**
     * Logical <b>\></b> operator.
     *
     * Used to make logical operator into request between two expressions.
     *
     * @param _rExpression Expression used to make logical operation with this.
     * @return A new constructed expression with this + operator + expression.
     */
    ETKSQLite3Expression                operator>(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Logical <b>\></b> operator with column.
     *
     * Used to make logical operator into request between two expressions.
     * The full name of the column is recorded.
     *
     * @param _rColumn Column class that contains column and table name. Names are used to make logical operation with this.
     * @return A new constructed expression with this + operator + expression.
     */
    ETKSQLite3Expression                operator>(const ETKSQLite3Column &_rColumn) const;

    /**
     * Logical <b>\>=</b> operator.
     *
     * Used to make logical operator into request between two expressions.
     *
     * @param _rExpression Expression used to make logical operation with this.
     * @return A new constructed expression with this + operator + expression.
     */
    ETKSQLite3Expression                operator>=(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Logical <b>\>=</b> operator with column.
     *
     * Used to make logical operator into request between two expressions.
     * The full name of the column is recorded.
     *
     * @param _rColumn Column class that contains column and table name. Names are used to make logical operation with this.
     * @return A new constructed expression with this + operator + expression.
     */
    ETKSQLite3Expression                operator>=(const ETKSQLite3Column &_rColumn) const;

    /**
     * Logical <b>\<</b> operator.
     *
     * Used to make logical operator into request between two expressions.
     *
     * @param _rExpression Expression used to make logical operation with this.
     * @return A new constructed expression with this + operator + expression.
     */
    ETKSQLite3Expression                operator<(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Logical <b>\<</b> operator with column.
     *
     * Used to make logical operator into request between two expressions.
     * The full name of the column is recorded.
     *
     * @param _rColumn Column class that contains column and table name. Names are used to make logical operation with this.
     * @return A new constructed expression with this + operator + expression.
     */
    ETKSQLite3Expression                operator<(const ETKSQLite3Column &_rColumn) const;

    /**
     * Logical <b>\<</b> operator.
     *
     * Used to make logical operator into request between two expressions.
     *
     * @param _rExpression Expression used to make logical operation with this.
     * @return A new constructed expression with this + operator + expression.
     */
    ETKSQLite3Expression                operator<=(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Logical <b>\<=</b> operator with column.
     *
     * Used to make logical operator into request between two expressions.
     * The full name of the column is recorded.
     *
     * @param _rColumn Column class that contains column and table name. Names are used to make logical operation with this.
     * @return A new constructed expression with this + operator + expression.
     */
    ETKSQLite3Expression                operator<=(const ETKSQLite3Column &_rColumn) const;

    /**
     * Add expression to other.
     *
     * Used to concatenate expressions with '<b>, </b>'. Used for operations Order By that are separate with comma,
     * or assignment for inserters and updaters.
     *
     * @param _rExpression Expression to add to this.
     * @return A new constructed expression with this new expression.
     */
    const ETKSQLite3Expression &        Add(const ETKSQLite3Expression &_rExpression);

    /**
     * Add criterion.
     *
     * Used to add select into a select. The criterion <b>must</b> be select criterion.
     *
     * @param _rCriterion Criterion to add to this.
     * @return A new constructed expression with this new criterion.
     */
    const ETKSQLite3Expression &        Add(const ETKSQLite3Criterion &_rCriterion);

    /**
     * Add expression to other.
     *
     * Used to concatenate expressions with '<b>, </b>'. Used for operations Order By that are separate with comma,
     * join (separate with nothing) or assignment for inserters and updaters.
     *
     * @param _rExpression Expression to add to this.
     * @return A new constructed expression with this new expression.
     */
    ETKSQLite3Expression &              operator<<(const ETKSQLite3Expression &_rExpression);

    /**
     * Logical <b>\!</b> operator.
     *
     * Used to make logical not operator into request on this expression only (unary operator).
     *
     * @return A new constructed expression with this operator.
     */
    ETKSQLite3Expression                operator!() const;

    /**
     * SQL Like operator.
     *
     * Used to make Like operator into request.
     *
     * @return A new constructed expression with this + operator + expression.
     */
    ETKSQLite3Expression                Like(const ETKSQLite3Expression &_rExpression) const;

    /**
     * SQL In operator.
     *
     * Used to make In operator into request.
     *
     * @return A new constructed expression with this + operator + expression.
     */
     ETKSQLite3Expression               In(const ETKSQLite3Expression &_rExpression) const;

    /**
     * SQL JOIN operator.
     *
     * Used to make JOIN operator into request.
     *
     * @param _rExpressionIn Join expression to set into IN expression.
     * @return A new constructed expression with this + operator + expression.
     */
     ETKSQLite3Expression               Join(const ETKSQLite3Expression &_rExpressionIn) const;

    /**
     * SQL INNER JOIN operator.
     *
     * Used to make INNER JOIN operator into request.
     *
     * @param _rExpressionIn Inner join expression to set into IN expression.
     * @return A new constructed expression with this + operator + expression.
     */
     ETKSQLite3Expression               InnerJoin(const ETKSQLite3Expression &_rExpressionIn) const;

    /**
     * SQL LEFT JOIN operator.
     *
     * Used to make LEFT JOIN operator into request.
     *
     * @param _rExpressionIn Left join expression to set into IN expression.
     * @return A new constructed expression with this + operator + expression.
     */
     ETKSQLite3Expression               LeftJoin(const ETKSQLite3Expression &_rExpressionIn) const;
    //@}

    /**
     * Format elements into string char to format the request.
     *
     * @return The request as string format.
     */
    etkString                           FormatRequest() const;

    /**
     * Used to know if expression is empty or not.
     *
     * Is empty if m_ExpressionType is equal to eExpressionTypeNothing.
     *
     * @return true if expression is empty, false else.
     */
    virtual bool                        IsNull() const;

    /**
     * Used to know if the expression is join type.
     *
     * The join expression types are <b>eOperationInnerJoin</b>, <b>eOperationAndJoin</b>, <b>eOperationJoin</b>, <b>eOperationLeftJoin</b>
     , <b>eOperationRightJoin</b>, <b>eOperationFullJoin</b>.
     *
     * @return true if it is a join type expression, false else.
     */
    bool                                IsJoin() const;

    /**
     * Update (if needed) the table list.
     *
     * Is used in specific case (like a column select where the column is not added on the same table as the select one)?
     * or for <b>LEFT OUTER JOIN</b> that should remove a table name from the <b>FROM</b> selector.
     * Add or remove tables depending of request type.
     *
     * @param _rlstTablesList List to update.
     * @return _rlstTablesList Used to calling methods on cascade.
     */
    virtual tdStringList &              UpdateTablesList(tdStringList &_rlstTablesList) const;

    /**
     * Get expression 1.
     *
     * Be careful, the expression can be NULL.
     *
     * @return The left expression 1.
     */
    const ETKSQLite3Expression *        GetExpression1() const;

    /**
     * Get expression 2.
     *
     * Be careful, the expression can be NULL.
     *
     * @return The left expression 2.
     */
    const ETKSQLite3Expression *        GetExpression2() const;

    /**
     * Get the expression as string.
     *
     * Call GetAsStringIterative() to don't use recursion that can make a stack overflow.
     *
     * @return Espression computed as string.
     */
    virtual etkString                    GetAsString() const;

    /**
     * Clear all expression and delete them to prevent memory leaks.
     */
    void                                Clear();

protected:
    /**
     * Bind a column data of a statement.
     *
     * @param _rstmt Statement to bind to.
     * @param _riIndex Index of item to bind. If a bind is done, increment index.
     * @param _bForInsertRequest If true if it's bind for an insert request (for INSERT request, the primary key is computed and should be NULL).
     */
    virtual void                        BindTo(wxSQLite3Statement &_rstmt, int &_riIndex, bool _bForInsertRequest) const;

    /**
     * Test if this kind of operator could have an ORDER BY statement.
     *
     * An ORDER BY criterion is not working on MIN / MAX / COUNT / SUM, etc.
     *
     * @return true if the request is allowed to have an ORDER BY statement, false else.
     */
    bool                                CanHaveOrderBy() const;

private:
    /**
     * Construct an operation type.
     *
     * Is used into constructors only to constuct an expression with two expressions and a operation + value.
     * Both expression could be empty or not. If expression 2 is not empty, expression 1 must not be empty!
     *
     * @param _rExpression1 Expression 1 (left expression).
     * @param _rExpression2 Expression 2 (right expression).
     */
    void                                Assign2Expressions(const ETKSQLite3Expression &_rExpression1, const ETKSQLite3Expression &_rExpression2);

    /**
     * Get the expression as string.
     *
     * Is called by GetAsString() to remove recursion that can make a stack overflow.
     *
     * @return Espression computed as string.
     */
    etkString                           GetAsStringIterative() const;

    /**
     * Get the expression as string for the node.
     *
     * Is called by GetAsStringIterative() for each node to compute.
     *
     * @param _rstrOperatorAsString String where to put the result.
     * @param _pExpression Pointer on expression.
     * @param _pExpression1 Pointer on expression 1.
     * @param _rstrExpression1 String expression already computed for expression 1.
     * @param _pExpression2 Pointer on expression 2.
     * @param _rstrExpression2 String expression already computed for expression 2.
     * @return The expression for the node _pExpression.
     */
    static void                         InternalAsStringForNode(etkString & _rstrOperatorAsString, const ETKSQLite3Expression *_pExpression, const ETKSQLite3Expression *_pExpression1, const etkString & _rstrExpression1, const ETKSQLite3Expression *_pExpression2, const etkString & _rstrExpression2);

    /**
     * Format AND or OR expression.
     *
     * Is used to omit operator if one the both expression is empty.
     *
     * @param strOperator Operator (AND or OR).
     * @param _pExpression1 Pointer on expression 1.
     * @param _rstrExpression1 String expression already computed for expression 1.
     * @param _pExpression2 Pointer on expression 2.
     * @param _rstrExpression2 String expression already computed for expression 2.
     * @return Formatted string.
     */
    static etkString                    InternalGetAsStringForOperator(etkString _strOperator, const ETKSQLite3Expression *_pExpression1, const etkString & _rstrExpression1, const ETKSQLite3Expression *_pExpression2, const etkString & _rstrExpression2);

    friend class ETKSQLite3Criterion;

#ifdef __WXDEBUG__
public:
    /**
     * Get all expression tree as string.
     *
     * Is very useful function to see what kind of expression was created, or why the generated SQL is not
     * correct and to see all content of the generated expression.<br>
     * <b>Avalaible only in debug mode</b>.
     *
     * @return All content of tree expression as string text.
     */
    etkString                            GetStringContentDescription() const;
private:

    /**
     * Construct the description tree of one node and all node it contains.
     *
     * It is a recursive function. All tree node are merge each other on the same line, one line contains
     * more than one node pad with space to be align. The result string is on several lines separate by carriage return.<br>
     * <b>Avalaible only in debug mode</b>.
     *
     * @return _strDescription Receive all Tree node description, all line have same length.
     * @return The length of one line.
     */
    int                                 ConstructContentDescription(etkString &_strDescription) const;

    /**
     * Construct the description of one node without child and lines.
     *
     * It is not a recursive function. One expression description is degined on more one line.
     * All description must have always the same number of line. The description node describe:
     * <ul>
     *  <li>[ID=<i>id_number</i>]</li>
     *  <li><i>operator_type</i></li>
     *  <li>Type=<i>variant_type</i></li>
     *  <li><i>variant_value</i></li>
     * </ul>
     * One line contains more than one node pad with space to be align.
     * The result string is on several lines separate by carriage return.<br>
     * <b>Avalaible only in debug mode</b>. Could be called on NULL instance (in this case, no
     * members are accessed and the description indicate null value.
     *
     * @return _strDescription Receive one tree node description, all line have same length.
     * @return The length of one line.
     */
    int                                 ConstructContentThisDescription(etkString &_strDescription) const;

    /**
     * Get the current operator type as string.
     *
     * Used to convert an enum to string.
     * Call GetVariantDebug function.
     * <b>Avalaible only in debug mode</b>.
     *
     * @return The operator as string.
     */
    etkString                            GetOperatorString() const;

    /**
     * Pad all string to specific size.
     *
     * Decode all lines of the string and pad all line with space to be centered with the specific size.
     * <b>Avalaible only in debug mode</b>.
     *
     * @param _strString String to pad.
     * @param iLenString length to pad.
     * @return The padded string.
     */
    static etkString                     Pad(const etkString _strString, int iLenString);
#endif // __WXDEBUG__
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                        ETKSQLite3Criterion                                          //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * SQL criterion class.
 *
 * It is the class that contains all kind of expressions (where, column selection, order by, join, etc) and make
 * the SQL format request that is used directly with database request.
 *
 * @see ETKSQLite3Expression
 *
 * @author Stéphane Château
 * @version Nom : ETKSQLite3<br>
 *          Révision : <b>1.0</b>
 */
class EXPORT_IMPORT ETKSQLite3Criterion : public ETKSQLite3Expression
{
public:
    /**
     * Request type.
     *
     * <ul>
     *   <li> <b>eRequestTypeUndefined</b>: Not already defined.
     *   <li> <b>eRequestTypeInsert</b>: Insert request (INSERT).
     *   <li> <b>eRequestTypeDelete</b>: Delete request (DELETE).
     *   <li> <b>eRequestTypeUpdate</b>: Update request (UPDATE).
     *   <li> <b>eRequestTypeSelect</b>: Delete request (SELECT).
     * </ul>
     */
    enum eRequestType
    {
        eRequestTypeUndefined,
        eRequestTypeInsert,
        eRequestTypeDelete,
        eRequestTypeUpdate,
        eRequestTypeSelect
    };
private:
    /**
     * Expression used into the order by request.
     *
     * In SQL request it can be empty or could contains:
     * <ul>
     *   <li> ORDER BY <i>column_name</i> ASC
     *   <li> ORDER BY <i>nom_colonne</i> DESC
     * </ul>
     */
    ETKSQLite3Expression                m_exprOrderBy;

    /**
     * Used to record Join request.
     */
    ETKSQLite3Expression                m_exprJoin;

    /**
     * Used to record Where expression.
     *
     * This expression is used to construct the where of the request, it is
     * not used for all request type (not used for INSERT).
     */
    ETKSQLite3Expression                m_exprWhere;

    /**
     * Used to record From expression.
     */
    ETKSQLite3Expression                m_exprFrom;

    /**
     * Used to record GROUP BY expression.
     */
    ETKSQLite3Expression                m_exprGroupBy;

    /**
     * Selection flag of DISTINCT type request.
     *
     * Used, when a request is post to suppress twice columns in database response.
     * Is false by default.
     */
    bool                                m_bDistinct;

    /**
     * If not 0, use to add LIMIT to request.
     */
    int                                 m_iLimit;

    /**
     * Request type.
     */
    eRequestType                        m_eRequestType;

public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Default constructor.
     *
     * Set criterion to empty.
     *
     * @param _eRequestType Type of request.
     */
    ETKSQLite3Criterion(eRequestType _eRequestType);

    /**
     * Constructor created from an expression.
     *
     * @param _rExpression Expression to copy into this.
     */
    ETKSQLite3Criterion(const ETKSQLite3Expression& _rExpression);

    /**
     * Copy contructor.
     *
     * @param _rCriterion Criterion to copy into this.
     */
    ETKSQLite3Criterion(const ETKSQLite3Criterion& _rCriterion);

    /**
     * Constructor created from a selection request expression.
     *
     * Use, for example, to make IN (so, with selection request into the In criterion).
     *
     * @param _rSelector Selector to copy into this.
     */
    ETKSQLite3Criterion(const ETKSQLite3RequestSelector& _rSelector);

    /**
     * Destructor.
     *
     */
    virtual ~ETKSQLite3Criterion();
    //@}

    /**
     * Get the request type.
     */
    eRequestType                        GetRequestType() const;

    /**
     * Set the request type.
     */
    void                                SetRequestType(eRequestType _eRequestType);

    /** @name Operators.
     *
     */
    //@{
    /**
     * Assignment operator from an expression.
     *
     * @param _rExpresion Expression to copy into this.
     * @return Reference to this that contains assigned expression.
     */
    const ETKSQLite3Criterion&          operator=(const ETKSQLite3Expression& _rExpresion);

    /**
     * Assignment operator from a criterion.
     *
     * @param _rCriterion Criterion to copy into this.
     * @return Reference to this that contains assigned expression.
     */
    const ETKSQLite3Criterion&          operator=(const ETKSQLite3Criterion& _rCriterion);

    /**
     * Assignment operator from a selector.
     *
     * Use, for example, to make IN (so, with selection request into the In criterion).
     *
     * @param _rSelector Selector to copy into this.
     * @return Reference to this that contains assigned expression.
     */
    const ETKSQLite3Criterion&          operator=(const ETKSQLite3RequestSelector& _rSelector);

    /**
     * AND operator from an expression.
     *
     * @param _rExpresion Expression to make operation with this.
     * @return Reference to this that contains new created expression.
     */
    ETKSQLite3Criterion                 operator&&(const ETKSQLite3Expression& _rExpresion) const;

    /**
     * AND operator from a criterion.
     *
     * @param _rCriterion criterion to make operation with this.
     * @return Reference to this that contains new created expression.
     */
    ETKSQLite3Criterion                 operator&&(const ETKSQLite3Criterion & _rCriterion) const;
    //@}

// Public methods
    /**
     * Clear all expressions and internal informations.
     *
     * After calling this method, the instance of this class can be reuse.
     * Be careful, the request type is unchanged.
     */
    void                                Clear();

    /**
     * Apply ascendant sort order.
     *
     * Add this order to other ORDER BY already present.
     *
     * @param _rColumn Column on wich the sort should be apply.
     * @return A new criterion with this order by.
     */
    ETKSQLite3Criterion &               AddOrderByAscending(const ETKSQLite3Column& _rColumn);

    /**
     * Apply descendant sort order.
     *
     * Add this order to other ORDER BY already present.
     *
     * @param _rColumn Column on wich the sort should be apply.
     * @return A new criterion with this order by.
     */
    ETKSQLite3Criterion &               AddOrderByDescending(const ETKSQLite3Column& _rColumn);

    /**
     * Add a list of order by.
     *
     * BE CAREFUL : Is used only with ETKSQLite3RequestSelector class.
     * The order must be applied only on ETKSQLite3RequestSelector or ETKSQLite3Criterion classes but never
     * on both, else the source code will be modified to support priority order (probably the selector first).
     *
     * @param _rExpresion {const ETKSQLite3Expression &} - Order list as an expression.
     */
    void                                AddOrderbyList(const ETKSQLite3Expression& _rExpresion);

    /**
     * Set Distinct request to true and get the request.
     *
     * Is equal to <i>SetDistinct(true)</i> but return this that be able to cascade more operations.
     *
     * @return Reference to this that contains new created expression.
     * @see SetDistinct
     */
    ETKSQLite3Criterion &               Distinct();

    /**
     * Used to know if the expression is null or not.
     *
     * Is nul if expression type is eExpressionTypeNothing and if distinct value is default one (false), and
     * if order expression is null too.
     *
     * @return true if the expression is null, false else.
     */
    virtual bool                        IsNull() const;

    /**
     * Used to know if the expression is valid or not.
     *
     * Is valid if expression type is not eExpressionTypeNothing (distrinct and order are not taken
     * into account).
     *
     * @return true if the expression is valid, false else.
     */
    bool                                IsValid() const;

    /**
     * Convert a string list into a string separate by comma.
     *
     * Insert '<b>, </b>' between all field of this list.
     *
     * @param _rlstDatas Liste of strings.
     * @return A formatted string with '<b>, </b>' between all field.
     */
    static etkString                    FormatSetAsString(const tdStringList &_rlstDatas);

    /**
     * Apply (oo not) the distinc mode on a request.
     *
     * @param _bDistinct Distinct parameter.
     */
    void                                SetDistinct(bool _bDistinct);

    /**
     * Get the distict mode of this criterion.
     *
     * @return true if the distinct mode is set, false else.
     */
    bool                                GetDistinct() const;

    /**
     * Add limit.
     *
     * @param _iLimit Number of limit rows.
     */
    void                                SetLimit(int _iLimit);

    /**
     * Get limit.
     *
     * @return Number of limit rows, 0 if not used.
     */
    int                                 GetLimit() const;

    /**
     * Set the Where expression.
     *
     * This expression is used to construct the Where of the request, it is
     * not used for all request type (not used for INSERT).
     *
     * @param _rExprWhere The Where expression.
     */
    void                                SetWhere(const ETKSQLite3Expression &_rExprWhere);

    /**
     * Get the Where expression.
     *
     * @return The Where expression.
     */
    const ETKSQLite3Expression &        GetWhere() const;

    /**
     * Set the From expression.
     *
     * <b>Be careful, this expression is computed automatically</b> if empty. Use it only to remove automatically computation.<br>
     * Example:<br>
     *  TABLE Customer { ID, Name }<br>
     *  TABLE Purchase { ID_Customer , ID_Item }<br><br>
     *
     * To know how many purchase a customer has done, you can make this request:<br>
     *  <i>SELECT TCustomer.*, (SELECT count(*) from TPurchase WHERE (TCustomer.id = TPurchase.idCustomer)) FROM TCustomer</i>.<br><br>
     *
     * If you let the automatic mode do its works, it will generated a SQL request as:<br>
     *  <i>SELECT TCustomer.*, (SELECT count(*) from <b>TCustomer</b>, TPurchase WHERE (TCustomer.id = TPurchase.idCustomer)) FROM TCustomer</i>.<br><br>
     *
     * This automatic generation will set one more table name into the from (TCustomer) that not give the correct SQL request: the result will not give the attemp result.<br>
     * In normal mode, don't set this expression, use it only for specific cases that need it.
     *
     * @see GetFrom
     * @param _rExprWhere The From expression. It can be a list of columns (only tables name will be taken into account) or directly a
     *        string of table seperate with comma, like "Table1, Table2, Table3".
     */
    void                                SetFrom(const ETKSQLite3Expression &_rExprFrom);

    /**
     * Get the From expression.
     *
     * @see SetFrom
     * @return The From expression.
     */
    const ETKSQLite3Expression &        GetFrom() const;

    /**
     * Set the Join expression.
     *
     * This expression is used to construct the Join of the request, it is
     * not used for all request type (only for SELECT).
     *
     * @see GetJoin
     * @param _rExprJoin The Join expression.
     */
    void                                SetJoin(const ETKSQLite3Expression &_rExprJoin = ETKSQLite3Expression());

    /**
     * Get the Join expression.
     *
     * @see SetJoin
     * @return The Join expression.
     */
    const ETKSQLite3Expression &        GetJoin() const;

    /**
     * Add a group by.
     *
     * @see GetGroupBy, SetGroupBy
     * @param _rGroupBy Group By expression.
     * @return An expression that can be used to call Add again.
     */
    ETKSQLite3Expression &              AddGroupBy(const ETKSQLite3Expression &_rGroupBy);

    /**
     * Set the Group By expression.
     *
     * @see AddGroupBy, GetGroupBy
     * @param _rExprGroupBy Group By expression.
     */
    void                                SetGroupBy(const ETKSQLite3Expression &_rExprGroupBy = ETKSQLite3Expression());

    /**
     * Get the Group By expression.
     *
     * @see AddGroupBy, SetGroupBy
     * @return The Group By expression.
     */
    const ETKSQLite3Expression &        GetGroupBy() const;

    /**
     * Construct SQL request with internal informations.
     */
    etkString                           GetSQL() const;

    /**
     * Bind a column data of a statement.
     *
     * @param _rstmt Statement to bind to.
     * @param _riIndex Index of item to bind. If a bind is done, increment index.
     */
    void                                BindTo(wxSQLite3Statement &_rstmt, int &_riIndex) const;

protected:
    /**
     * Update (if needed) the table list.
     *
     * Is used in specific case (like a column select where the column is not added on the same table as the select one)?
     * or for <b>LEFT OUTER JOIN</b> that should remove a table name from the <b>FROM</b> selector.
     * Add or remove tables depending of request type on internal expression and in join expression too.<br>
     * Update table from join expression (GetJoin()) and where expression (GetWhere()) only. Inserter call directly
     * base class UpdateTablesList() function.
     *
     * @param _rlstTablesList List to update.
     * @return _rlstTablesList Used to calling methods on cascade.
     */
    virtual tdStringList &              UpdateTablesList(tdStringList &_rlstTablesList) const;

    /**
     * Set The order by.
     *
     * @param _rExprOrderBy Expression used for order by.
     */
    void                                SetOrderBy(const ETKSQLite3Expression &_rExprOrderBy = ETKSQLite3Expression());

    /**
     * Get The order by.
     *
     * @return The Expression used for order by.
     */
    const ETKSQLite3Expression &        GetOrderBy() const;

    /**
     * Format an order by for sql request.
     *
     * @return The ORDER BY SQL string.
     */
    etkString                           GetOrderByAsString() const;

    /**
     * Format a limit for sql request.
     *
     * @return The LIMIT(x) SQL string.
     */
    etkString                           GetLimitAsString() const;
};

EXPORT_IMPORT ETKSQLite3Expression      operator==(const ETKSQLite3Value &_rValue, const ETKSQLite3Column &_rColumn);
EXPORT_IMPORT ETKSQLite3Expression      operator>(const ETKSQLite3Value &_rValue, const ETKSQLite3Column &_rColumn);
EXPORT_IMPORT ETKSQLite3Expression      operator>=(const ETKSQLite3Value &_rValue, const ETKSQLite3Column &_rColumn);
EXPORT_IMPORT ETKSQLite3Expression      operator<(const ETKSQLite3Value &_rValue, const ETKSQLite3Column &_rColumn);
EXPORT_IMPORT ETKSQLite3Expression      operator<=(const ETKSQLite3Value &_rValue, const ETKSQLite3Column &_rColumn);

EXPORT_IMPORT ETKSQLite3Expression      dbCount();
EXPORT_IMPORT ETKSQLite3Expression      dbCount(const ETKSQLite3Column& _rColumn);
EXPORT_IMPORT ETKSQLite3Expression      dbExists(const ETKSQLite3RequestSelector& _rSelector);
EXPORT_IMPORT ETKSQLite3Expression      dbExists(const ETKSQLite3Criterion& _rCriterion);
EXPORT_IMPORT ETKSQLite3Expression      dbExists(const ETKSQLite3Expression& _rExpression);
EXPORT_IMPORT ETKSQLite3Expression      dbDistinct(const ETKSQLite3Column& _rColumn);
EXPORT_IMPORT ETKSQLite3Expression      dbCount(const ETKSQLite3Expression& _rExpression);
EXPORT_IMPORT ETKSQLite3Expression      dbMax(const ETKSQLite3Column& _rColumn);
EXPORT_IMPORT ETKSQLite3Expression      dbMin(const ETKSQLite3Column& _rColumn);
EXPORT_IMPORT ETKSQLite3Expression      dbSum(const ETKSQLite3Column& _rColumn);
EXPORT_IMPORT ETKSQLite3Expression      dbCast(const ETKSQLite3Column& _rColumn, etkString _strCast);
EXPORT_IMPORT ETKSQLite3Expression      dbSubString(const ETKSQLite3Expression &_rExpression, const ETKSQLite3Expression &_rExpressionStart, const ETKSQLite3Expression &_rExpressionLength);
EXPORT_IMPORT ETKSQLite3Expression      dbAs(const ETKSQLite3Expression& _rExpression, etkString _strAsName);
EXPORT_IMPORT ETKSQLite3Expression      dbAs(const ETKSQLite3Criterion& _rCriterion, etkString _strAsName);
EXPORT_IMPORT ETKSQLite3Expression      dbAs(const ETKSQLite3Record &_rRecord, etkString _strAsName);
/**
 * Make a column that can be used in set order of selector.
 *
 * @param _rColumn Column on wich the as should be apply.
 * @param _strAs Will replace table name like: 'AsColumn'.'ColumnName'.
 * @return A new column that can be used into AddOrderByAscending / AddOrderByDescending selector.
 */
EXPORT_IMPORT ETKSQLite3Column          dbAsColumn(const ETKSQLite3Column& _rColumn, etkString _strAs);

// Functor to extract key from key-value
template<typename Pair> struct ETKSQLite3PairFirstExtractor
{
    typename Pair::first_type operator()(const Pair& pair) const
    {
        return pair.first;
    }
};

// Functor to extract value from key-value
template<typename Pair> struct ETKSQLite3PairSecondExtractor
{
    typename Pair::first_type operator()(const Pair& pair) const
    {
        return pair.second;
    }
};

template<typename Iterator, typename Extractor>
              ETKSQLite3Expression      dbList(Iterator _itBegin, Iterator _itEnd, Extractor _extractor)
                                        {
                                            ETKSQLite3Expression expr;
                                            while (_itBegin != _itEnd)
                                            {
                                                expr.Add(_extractor(*_itBegin++)); // Add value pointed by iterator
                                            }
                                            return expr;
                                        }

template<typename Iterator>
              ETKSQLite3Expression      dbList(Iterator _itBegin, Iterator _itEnd)
                                        {
                                            ETKSQLite3Expression expr;
                                            while (_itBegin != _itEnd)
                                            {
                                                expr.Add(*_itBegin++); // Add value pointed by iterator
                                            }
                                            return expr;
                                        }
// Directly format a sql request
EXPORT_IMPORT ETKSQLite3Expression      dbSQL(etkString _strSQLRequest);

#endif // INCLUDE_ETK_SQLITE3_CRITERION_H
