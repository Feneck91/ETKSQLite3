/////////////////////////////////////////////////////////////////////////////
/**
 * @file ETKSQLite3RequestSelector.h
 * @brief Header file for selector request managment.
 *
 * Code for database request selector class.
 *
 * @author Stéphane Château
 * @date Created: 2011/08/09
 * @date Modified: 2025/09/01
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_ETK_SQLITE3_REQUEST_SELECTOR_H
#define INCLUDE_ETK_SQLITE3_REQUEST_SELECTOR_H

#include "ETKSQLite3Types.h"
#include "ETKSQLite3Request.h"
#include "ETKSQLite3ResultSet.h"
#include "ETKSQLite3Record.h"
#include <list>

class ETKSQLite3Record;

/**
 * Class used to make select request.
 *
 * Is used to select rows into table. Unlike selection request, this class update column field
 * directly with binding without convert into text (it is <i>wxSQLite3Statement</i> that makes
 * the last string conversion).
 *
 * @see ETKSQLite3Request
 * @author Stéphane Château
 * @version Nom : ETKSQLite3<br>
 *          Révision : <b>1.0</b>
 */
class EXPORT_IMPORT ETKSQLite3RequestSelector : public ETKSQLite3Request
{
protected:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Constructor.
     *
     * Allowed only for derived classes. Only the database is allowed to create instance of this class.
     *
     * @param _rDatabase Reference on database.
     */
    ETKSQLite3RequestSelector(ETKSQLite3Database & _rDatabase);

    /**
     * Constructor.
     *
     * Allowed only for ETKSQLite3Expression to make select into select.
     * Use friend to allow ETKSQLite3Expression to call this constructor.
     * In this case, the database is not set.
     *
     */
    ETKSQLite3RequestSelector(const ETKSQLite3Criterion &_rCriterion);

public:
    /**
     * Copy constructor.
     *
     * @param _rOther Element to copy to this.
     */
    ETKSQLite3RequestSelector(const ETKSQLite3RequestSelector & _rOther);

    /** Destructeur.
    *
    */
    virtual ~ETKSQLite3RequestSelector();
    //@}

    /** @name Operators.
     *
     * Are used to add fields or record to this request.
     */
    //@{
    /**
     * All all the content of a record (or table).
     *
     * Add all colums by calling \<\< operator with all columns parameters.
     *
     * @param _rRecord Record to select.
     * @return A reference to this used to cascade other operators (like \<\< operators).
     */
    ETKSQLite3RequestSelector &         operator<<(const ETKSQLite3Record &_rRecord);

    /**
     * Add a column into of the table into the selector.
     *
     * Select the list of columns to get into the SQL request.
     *
     * @param _rColumn Column to add.
     * @return A reference to this used to cascade other operators (like \<\< operators).
     */
    ETKSQLite3RequestSelector &         operator<<(const ETKSQLite3Column &_rColumn);

    /**
     * Add a criterion into the select.
     *
     * Contains all criterion to put between SELECT and FROM request.
     * This criterion could be count, min, max, etc... Others criterions make errors.
     * All are put near each other, separate with comma.
     *
     * @param _rCriterion Criterion to add.
     * @return A reference to this used to cascade other operators (like \<\< operators).
     */
    ETKSQLite3RequestSelector &         operator<<(const ETKSQLite3Criterion &_rCriterion);

    /**
     * Add an expresion into the select.
     *
     * Contains all criterion to put between SELECT and FROM request.
     * This criterion could be count, min, max, etc... Others criterions make errors.
     * All are put near each other, separate with comma.
     *
     * @param _rExpression Expression to add.
     * @return A reference to this used to cascade other operators (like \<\< operators).
     */
    ETKSQLite3RequestSelector &         operator<<(const ETKSQLite3Expression &_rExpression);
    //@}

    /** @name Order By.
     *
     * Are used to add order by to this request.
     */
    //@{
    /**
     * Apply ascendant sort order.
     *
     * Add this order to other ORDER BY already present.
     *
     * @param _rColumn Column on wich the sort should be apply.
     * @return A reference to this used to cascade other operators (like \<\< operators).
     */
    ETKSQLite3RequestSelector &         AddOrderByAscending(const ETKSQLite3Column& _rColumn);

    /**
     * Apply descendant sort order.
     *
     * Add this order to other ORDER BY already present.
     *
     * @param _rColumn Column on wich the sort should be apply.
     * @return A reference to this used to cascade other operators (like \<\< operators).
     */
    ETKSQLite3RequestSelector &         AddOrderByDescending(const ETKSQLite3Column& _rColumn);
    //@}

    /**
     * Add limit.
     *
     * @param _iLimit Number of limit rows.
     * @return A reference to this used to cascade other operators (like \<\< operators).
     */
    ETKSQLite3RequestSelector &         SetLimit(int _iLimit);

    /**
     * Execute the request.
     *
     * The return class contains all rows. You could cast it into template ETKSQLite3ResultSet class.
     * to bind automatically all columns with datas.
     *
     * @return A resultset that contains all rows.
     */
    virtual wxSQLite3ResultSet          ExecuteQuery();

private:
    /**
     * This function is used to forbid access to execute function of the base class.
     *
     * Nothing is done into this function, an exception is raised.
     *
     * @see ExecuteWithTransaction,ExecuteWithoutTransaction,
     * @param _bLogError If true, log database error.
     * @param _bDisplayMsgBox If true, display the error to the user.
     * @param _bWithTransaction true to execute with transaction, false to execute without transaction.
     * @return The number of modified rows, -1 is error occurs. In this case, you can call ETKSQLite3Database::GetLastErrorMessage()
     *         to have error description.
     * @throw wxSQLite3Exception If the function failed, a wxSQLite3Exception is raided so calling this function
     * must be protected by try / catch statement.
     */
    virtual int                         Execute(bool _bLogError, bool _bDisplayMsgBox, bool _bWithTransaction) override;

    /**
     * Allow ETKSQLite3Database class to create ETKSQLite3RequestInserter instance.
     */
    friend class ETKSQLite3Database;

    /**
     * Allow ETKSQLite3Expression class to create ETKSQLite3RequestInserter instance without database.
     */
    friend class ETKSQLite3Expression;
};

#endif // INCLUDE_ETK_SQLITE3_REQUEST_SELECTOR_H
