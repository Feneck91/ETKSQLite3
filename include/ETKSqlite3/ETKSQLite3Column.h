/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3Column.cpp
// Library:     ETKSQLite3
// Purpose:     Code for column managment
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/04
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_ETK_SQLITE3_COLUMN_H
#define INCLUDE_ETK_SQLITE3_COLUMN_H

#include "ETKSQLite3Types.h"
#include "ETKSQLite3SharedPtr.h"
#include "ETKSQLite3Criterion.h"

class ETKSQLite3ValueBindBase;
class ETKSQLite3ValueBind;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                      ETKSQLite3ColumnAttributes                                       //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Class used to manage the colum attributes of a table or request.
 *
 * It ccntains name and attributes pointer to be able to bind datas with database elements.
 *
 * @author Stéphane Château
 * @version Name : ETKSQLite3<br>
 *          Revision : <b>1.0</b>
 */
class EXPORT_IMPORT ETKSQLite3ColumnAttributes
{
private:
    typedef ETKSQLite3::shared_ptr<ETKSQLite3ValueBindBase> tdETKSQLite3ValueBindBasePtr;

    /**
     * Indicate if this column should be added on insert request.
     *
     * If not it is certainly because it is a primary key with autoincrement, when adding this element for insert
     * request, this field is set to NULL to be automatically computed by database.
     */
    bool                                m_bAddForInsertRequest;

    /**
     * Class used to bind datas.
     */
    tdETKSQLite3ValueBindBasePtr        m_pBindValue;

protected:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Default constructor.
     *
     * Use this constructor only to assign values, nothing is initialised.
     * Used into columns static constructor.
     */
    ETKSQLite3ColumnAttributes();

public:
    /**
     * Constructor.
     *
     * @param _rBindValue Column's value to bind.
     * @param _bAddForInsertRequest Indicate if this column should be added on insert request. If not it is
     *                              certainly because it is a primary key with autoincrement, when adding
     *                              this element for insert request, this field is set to NULL to be
     *                              automatically computed by database.
     */
    ETKSQLite3ColumnAttributes(const ETKSQLite3ValueBind & _rBindValue,bool _bAddForInsertRequest = true);

    /**
     * Copy constructor.
     *
     * @param _rColumnAttributes Column to copy into this.
     */
    ETKSQLite3ColumnAttributes(const ETKSQLite3ColumnAttributes &_rColumnAttributes);

    /**
     * Destructor.
     */
    virtual ~ETKSQLite3ColumnAttributes();
    //@}

    /**
     * Bind a data to the statement.
     *
     * @param _rstmt Statement to bind to.
     * @param _iIndex Index of item to bind.
     * @param _bForInsertRequest If is bind for an INSERT request. In this case, some field must be set to NULL value (for INSERT request, the primary key is computed).
     */
    void                                BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bForInsertRequest) const;

    /**
     * Bind datas from the structure.
     *
     * @param _rResultSet Reocrd set where binf data from.
     * @param _iIndex Index of column into _rResultSet recordset.
     */
    void                                BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;

    /**
     * Indicate if the column's field is null or not.
     *
     * @return true if the column's field is null, false else.
     */
    bool                                IsNull() const;

    /**
     * Set column's field to null or not null.
     *
     * Could be called on const instance. Protect other members.
     *
     * @param _bIsNull true to put this column's field to null, false to put it to not null, in this case the
     *                 bind value will be use the value of the binded data.
     */
    void                                SetNull(bool _bIsNull = true) const;

    /// @name Operators.
    //@{
    /**
     * Assignment operator.
     *
     * @param _rColumnAttributes Column to copy into this.
     * @return This.
     */
    const ETKSQLite3ColumnAttributes &  operator=(const ETKSQLite3ColumnAttributes &_rColumnAttributes);
    //@}

#ifdef __WXDEBUG__
    /**
     * Verify that this column attribute is correctly binded.
     *
     * It is used to verify that the internal binded is correctly initialized. Used into assert function
     * to verify that some functions are called only on binded data, not on static colum that doesn't contains
     * a value to bind,
     *
     * @return true if the internal value is binded, false else.
     */
    bool                                IsBindedValueExists() const { return m_pBindValue.IsNotNull(); }
#endif

    friend class ETKSQLite3Column;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                            ETKSQLite3Column                                         //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Class used to manage the colum of a table or request.
 *
 * It contains all needed attributes: table and columns names.
 *
 * @author Stéphane Château
 * @version Name : ETKSQLite3<br>
 *          Revision : <b>1.0</b>
 */
class EXPORT_IMPORT ETKSQLite3Column
{
private:
    /**
     * Table name.
     */
    wxString                            m_strTableName;

    /**
     * Column name.
     */
    wxString                            m_strColumnName;

    /**
     * Contains all needed columns attributes.
     */
    ETKSQLite3ColumnAttributes          m_ColumnAttributes;

    /**
     * Column index.
     *
     * Be careful, Used ONLY on ETKSQLite3ResultSet to bind column directly on result index.
     * Is -1 if not used.
     */
    int                                 m_iColumnIndex;
public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Constructor.
     *
     * Used to construct columns instance with index, used <b>ONLY</b> on resultset to make binding with a column index without
     * using table and column name. Used when using COUNT and don't make a binding with COUNT (TABLE.COLUMN) but directly with the
     * index of column.
     *
     * @param _iIndexColumn Column index to bind to (0 based index).
     */
    explicit ETKSQLite3Column(int _iIndexColumn);

    /**
     * Constructor.
     *
     * Used to construct static colums instance: only table and column names are record.
     *
     * @param _strTableName Table's name.
     * @param _strColumnName Column's name.
     */
    ETKSQLite3Column(wxString _strTableName,wxString _strColumnName);

    /**
     * Constructor.
     *
     * @param _rColumn Column that record table and column names.
     * @param _rColumnAttributes Contains all colums needed attributes to make bindings.
     */
    ETKSQLite3Column(const ETKSQLite3Column &_rColumn,const ETKSQLite3ColumnAttributes &_rColumnAttributes);

    /**
     * Copy constructor.
     *
     * @param _rColumn Column to copy into this.
     */
    ETKSQLite3Column(const ETKSQLite3Column &_rColumn);

    /**
     * Destructor.
     */
    virtual ~ETKSQLite3Column();
    //@}

    /**
     * Get the column index (bind).
     *
     * Be careful, Used ONLY on ETKSQLite3ResultSet to bind column directly on result index.
     *
     * @return The index column, -1 if not used.
     */
    int                                 GetColumnIndex() const;

    /**
     * Ask if it contains columns index (used for the resultset binding only).
     *
     * Be careful, Used ONLY on ETKSQLite3ResultSet to bind column directly on result index.
     *
     * @return true if the column column exists, false else.
     */
    bool                                HasColumnIndex() const;

    /**
     * Get the table's name.
     *
     * @see GetColumnName,GetFullName
     */
    wxString                            GetTableName() const;

    /**
     * Set the table name.
     *
     * Change the table name.
     *
     * @param _strTableName New table name.
     */
    void                                SetTableName(wxString _strTableName);

    /**
     * Get the column's name without table name.
     *
     * @see GetTableName,GetFullName
     */
    wxString                            GetColumnName() const;

    /**
     * Get the column name with the table name.
     *
     * @return A string formatted as \"<b>TABLE_NAME.COLUMN_NAME</b>\".
     * @see GetTableName,GetColumnName
     */
    wxString                            GetFullName() const;

    /**
     * Bind a data to the statement.
     *
     * @param _rstmt Statement to bind to.
     * @param _iIndex Index of item to bind.
     * @param _bForInsertRequest If is bind for an INSERT request. In this case, some field must be set to NULL value (for INSERT request, the primary key is computed).
     */
    void                                BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bForInsertRequest) const;

    /**
     * Bind datas from the structure.
     *
     * @param _rResultSet Reocrd set where binf data from.
     * @param _iIndex Index of column into _rResultSet recordset.
     */
    void                                BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;

    /**
     * Get column attributes.
     *
     * It used to bind column value to SQL request. It replace the value by '<b>?</b>' and bind it to create
     * the correct request. This operator cannot be called on static column attribute, it must be called on
     * ETKSQLite3Record column non static instance (so binded on real value).
     *
     * @return The column attribute.
     */
    const ETKSQLite3ColumnAttributes &GetColumnAttributes() const;

    /**
     * Get column attributes.
     *
     * @see GetColumnAttributes.
     *
     * @return The column attribute.
     */
    const ETKSQLite3ColumnAttributes &operator*() const;

    /**
     * Get column attributes.
     *
     * @see GetColumnAttributes.
     *
     * @return The column attribute. The returns value is never NULL.
     */
    const ETKSQLite3ColumnAttributes *operator->() const;

#ifdef __WXDEBUG__
    /**
     * Ask if a bind value exists or not.
     *
     * Only columns of ETKSQLite3Record (automatically generated) contains value binding, static columns of
     * ETKSQLite3Record does not contains the value binding.
     *
     * @return true if the column has value binding, false else (static columns for example).
     */
    bool                                IsBindedValueExists() const;
#endif

    /// @name Operators.
    //@{
    /**
     * Assign operator.
     *
     * Called by copy constructor.
     *
     * @param _rColumn Column to copy into this.
     * @return this.
     */
    const ETKSQLite3Column &          operator=(const ETKSQLite3Column &_rColumn);
    //@}

    /** @name Expression construction operators.
     *
     * These operators are used to create SQL expression to execute database requests.
     */
    //@{
    /**
     * Test equality with expression.
     *
     * Used to generate equality operator into SQL expression.
     *
     * @param _rExpression Expression from wich the operator is applyed.
     * @return A new formatted expression.
     */
    ETKSQLite3Expression              operator==(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Test equality between column (database join).
     *
     * Used to generate equality operator between two columns (join) into SQL expression.
     *
     * @param _rColumn Column from wich the operator is applyed.
     * @return A new formatted expression.
     */
    ETKSQLite3Expression              operator==(const ETKSQLite3Column &_rColumn) const;

    /**
     * Test equality between column (database join).
     *
     * Used to generate equality operator between two columns (join) into SQL expression.
     * <br>The JOIN keyword return rows when there is at least one match in both tables.<br>
     * Example:<br>
     * <pre>
     *     ETKSQLite3RequestSelector selector = GetDatabase().GetSelector();
     *     selector << CRecordTable1::COLUMN_FIELD_Name
     *              << CRecordTable1::COLUMN_FIELD_FirstName
     *              << CRecordTable2::COLUMN_FIELD_Address
     *              << CRecordTable1::COLUMN_FIELD_id;
     *     selector.Join(CRecordTable1::COLUMN_FIELD_idAddr.Join(CRecordTable2::COLUMN_FIELD_id));
     * </pre>
     *
     * @param _rColumn Column from wich the operator is applyed.
     * @return A new formatted expression.
     */
    ETKSQLite3Expression              Join(const ETKSQLite3Column &_rColumn) const;

    /**
     * Test equality between column (database left join).
     *
     * Used to generate equality operator between two columns (left join) into SQL expression.
     * <br>Return all rows from the left table, even if there are no matches in the right table.<br>
     * Example:<br>
     * <pre>
     *     ETKSQLite3RequestSelector selector = GetDatabase().GetSelector();
     *     selector << CRecordTable1::COLUMN_FIELD_Name
     *              << CRecordTable1::COLUMN_FIELD_FirstName
     *              << CRecordTable2::COLUMN_FIELD_Address
     *              << CRecordTable1::COLUMN_FIELD_id;
     *     selector.Join(CRecordTable1::COLUMN_FIELD_idAddr.LeftJoin(CRecordTable2::COLUMN_FIELD_id));
     * </pre>
     *
     * @param _rColumn Column from wich the operator is applyed.
     * @return A new formatted expression.
     */
    ETKSQLite3Expression              LeftJoin(const ETKSQLite3Column &_rColumn) const;

    /**
     * Test equality between column (database inner join).
     *
     * Used to generate INNER JOIN operator between two columns into SQL expression.
     * <br>Return all rows when there is at least one match in both tables.<br>
     * Example:<br>
     * <pre>
     *     ETKSQLite3RequestSelector selector = GetDatabase().GetSelector();
     *     selector << CRecordTable1::COLUMN_FIELD_Name
     *              << CRecordTable1::COLUMN_FIELD_FirstName
     *              << CRecordTable2::COLUMN_FIELD_Address
     *              << CRecordTable1::COLUMN_FIELD_id;
     *     selector.Join(CRecordTable1::COLUMN_FIELD_idAddr.InnerJoin(CRecordTable2::COLUMN_FIELD_id));
     * </pre>
     *
     * @param _rColumn Column from wich the operator is applyed.
     * @return A new formatted expression.
     */
    ETKSQLite3Expression              InnerJoin(const ETKSQLite3Column &_rColumn) const;

//    RIGHT JOIN and FULL JOIN are not supported by sqlite3
//    **
//     * Test equality between column (database right join).
//     *
//     * Used to generate equality operator between two columns (right join) into SQL expression.
//     * <br>Return all rows from the right table, even if there are no matches in the left table.
//     *
//     * @param _rColumn Column from wich the operator is applyed.
//     * @return A new formatted expression.
//     *
//    ETKSQLite3Expression              RightJoin(const ETKSQLite3Column &_rColumn) const;
//
//    **
//     * Test equality between column (database right join).
//     *
//     * Used to generate equality operator between two columns (right join) into SQL expression.
//     * <br>Return rows when there is a match in one of the tables.
//     *
//     * @param _rColumn Column from wich the operator is applyed.
//     * @return A new formatted expression.
//     *
//    ETKSQLite3Expression              FullJoin(const ETKSQLite3Column &_rColumn) const;

    /**
     * <b>IN</b> operator with selector.
     *
     * Used to generate <b>IN</b> operator with selector into SQL expression.
     *
     * @param _rSelector Selector used to format the select into Where.
     * @return A new formatted expression.
     */
    ETKSQLite3Expression              In(const ETKSQLite3RequestSelector &_rSelector) const;

    /**
     * <b>IN</b> operator with expression.
     *
     * Used to generate <b>IN</b> operator into SQL expression.
     *
     * @param _rExpression Expression, must contains a values list.
     * @return A new formatted expression.
     */
    ETKSQLite3Expression              In(const ETKSQLite3Expression &_rExpression) const;

    /**
     * <b>LIKE</b> operator with expression.
     *
     * Used to generate <b>LIKE</b> operator into SQL expression.
     *
     * @param _rExpression Expression from wich the operator is applyed.
     * @return A new formatted expression.
     */
    ETKSQLite3Expression              Like(const ETKSQLite3Expression &_rExpression) const;

    /**
     * <b>AS</b> operator with expression.
     *
     * Used to generate <b>AS</b> operator into SQL expression:
     * <i>_strTableName</i>.<i>column name</i> AS <i>_strAsName</i>.<br>
     * If <i>_strAsName</i> is empty, the value is remplaced by the <i>_strTableName</i> + <i>column name</i>.<br>
     *
     * @see ChangeAs,ETKSQLite3Record::As
     * @param _strTableName Table name of the AS value.
     * @param _strAsName Name of the AS value variable, if empty, this value is computed as <i>_strTableName</i> + <i>column name</i>.
     * @return A new formatted expression.
     */
    ETKSQLite3Expression              As(wxString _strTableName,wxString _strAsName = wxEmptyString) const;

    /**
     * Modify the column name to be used as ResultSet record <b>ONLY</b>!
     *
     * Used to bind data into record when the result set is generate with an SQL expression
     * that use the <b>AS</b> expression from the record.<br>
     * Unlike the \ref ETKSQLite3Column::As "As" function, this function doesn't change the table name,
     * only change the columns names.<br>
     * <b>DO NOT CALL THIS FUNCTION ON STATIC COLUMN WITHOUT ATTRIBUTES</b>.
     *
     * @see As,ETKSQLite3Record::ChangeAs
     * @param _strAsName Name of the AS value variable, if empty, this value is computed as <i>_strTableName</i> + <i>column name</i>.
     * @param _strTableName The AS table name.
     */
    void                                ChangeAs(wxString _strTableName,wxString _strAsName = wxEmptyString);

    /**
     * Compute the column / table name for the \ref ETKSQLite3Column::ChangeAs "ChangeAs" function.
     *
     * Could be used to retrieve column attributes from a record that some columns are rebind using As SQL instruction.
     *
     * @return A copy of a column that have correct table name / column name.
     */
    ETKSQLite3Column                  ComputeColumnChangeAs(wxString _strTableName,wxString _strAsName = wxEmptyString) const;

    /**
     * <b>IS NULL</b> operator.
     *
     * Used to generate <b>IS NULL</b> operator on column.
     *
     * @return A new formatted expression.
     */
    ETKSQLite3Expression              IsNull() const;

    /**
     * <b>IS NOT NULL</b> operator.
     *
     * Used to generate <b>IS NOT NULL</b> operator on column.
     *
     * @return A new formatted expression.
     */
    ETKSQLite3Expression              IsNotNull() const;

    /**
     * Logical <b>!=</b> operator.
     *
     * Used to make logical operator into request between column and expression.
     *
     * @param _rExpression Expression used to make logical operation with this column.
     * @return A new constructed expression with this operator.
     */
    ETKSQLite3Expression              operator!=(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Logical <b>\></b> operator.
     *
     * Used to make logical operator into request between column and expression.
     *
     * @param _rExpression Expression used to make logical operation with this column.
     * @return A new constructed expression with this operator.
     */
    ETKSQLite3Expression              operator>(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Logical <b>\>=</b> operator.
     *
     * Used to make logical operator into request between column and expression.
     *
     * @param _rExpression Expression used to make logical operation with this column.
     * @return A new constructed expression with this operator.
     */
    ETKSQLite3Expression              operator>=(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Logical <b>\<</b> operator.
     *
     * Used to make logical operator into request between column and expression.
     *
     * @param _rExpression Expression used to make logical operation with this column.
     * @return A new constructed expression with this operator.
     */
    ETKSQLite3Expression              operator<(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Logical <b>\<=</b> operator.
     *
     * Used to make logical operator into request between column and expression.
     *
     * @param _rExpression Expression used to make logical operation with this column.
     * @return A new constructed expression with this operator.
     */
    ETKSQLite3Expression              operator<=(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Artihmetic <b>\*</b> operator (multiplication).
     *
     * Used to make artihmetic operator into request between column and expression.
     *
     * @param _rExpression Expression used to make logical operation with this column.
     * @return A new constructed expression with this operator.
     */
    ETKSQLite3Expression              operator*(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Artihmetic <b>\/</b> operator (division).
     *
     * Used to make artihmetic operator into request between column and expression.
     *
     * @param _rExpression Expression used to make logical operation with this column.
     * @return A new constructed expression with this operator.
     */
    ETKSQLite3Expression              operator/(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Artihmetic <b>\+</b> operator (sum).
     *
     * Used to make artihmetic operator into request between column and expression.
     *
     * @param _rExpression Expression used to make logical operation with this column.
     * @return A new constructed expression with this operator.
     */
    ETKSQLite3Expression              operator+(const ETKSQLite3Expression &_rExpression) const;

    /**
     * Artihmetic <b>\-</b> operator (substraction).
     *
     * Used to make artihmetic operator into request between column and expression.
     *
     * @param _rExpression Expression used to make logical operation with this column.
     * @return A new constructed expression with this operator.
     */
    ETKSQLite3Expression              operator-(const ETKSQLite3Expression &_rExpression) const;
    //@}
};

#endif // INCLUDE_ETK_SQLITE3_COLUMN_H
