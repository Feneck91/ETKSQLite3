/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3RequestSelector.h
// Library:     wxETKSQLite3
// Purpose:     Code to make select into database
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/09
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_WX_ETK_SQLITE3_REQUEST_SELECTOR_H
#define INCLUDE_WX_ETK_SQLITE3_REQUEST_SELECTOR_H

#include "wxETKSQLite3Types.h"
#include "wxETKSQLite3Request.h"
#include "wxETKSQLite3ResultSet.h"
#include "wxETKSQLite3Record.h"
#include <list>

class wxETKSQLite3Record;

/**
 * Class used to make select request.
 *
 * Is used to select rows into table. Unlike selection request, this class update column field
 * directly with binding without convert into text (it is <i>wxSQLite3Statement</i> that makes
 * the last string conversion).
 *
 * @see wxETKSQLite3Request
 * @author Stéphane Château
 * @version Nom : wxETKSQLite3<br>
 *          Révision : <b>1.0</b>
 */
class EXPORT_IMPORT wxETKSQLite3RequestSelector : public wxETKSQLite3Request
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
    wxETKSQLite3RequestSelector(wxETKSQLite3Database & _rDatabase);

    /**
     * Constructor.
     *
     * Allowed only for wxETKSQLite3Expression to make select into select.
     * Use friend to allow wxETKSQLite3Expression to call this constructor.
     * In this case, the database is not set.
     *
     */
    wxETKSQLite3RequestSelector(const wxETKSQLite3Criterion &_rCriterion);

public:
    /**
     * Copy constructor.
     *
     * @param _rOther Element to copy to this.
     */
    wxETKSQLite3RequestSelector(const wxETKSQLite3RequestSelector & _rOther);

    /** Destructeur.
    *
    */
    virtual ~wxETKSQLite3RequestSelector();
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
     * @return A reference to this used to cascade \<\< operators.
     */
    wxETKSQLite3RequestSelector &       operator<<(const wxETKSQLite3Record &_rRecord);

    /**
     * Add a column into of the table into the selector.
     *
     * Select the list of columns to get into the SQL request.
     *
     * @param _rColumn Column to add.
     * @return A reference to this used to cascade \<\< operators.
     */
    wxETKSQLite3RequestSelector &       operator<<(const wxETKSQLite3Column &_rColumn);

    /**
     * Add a criterion into the select.
     *
     * Contains all criterion to put between SELECT and FROM request.
     * This criterion could be count, min, max, etc... Others criterions make errors.
     * All are put near each other, separate with comma.
     *
     * @param _rCriterion Criterion to add.
     * @return A reference to this used to cascade \<\< operators.
     */
    wxETKSQLite3RequestSelector &       operator<<(const wxETKSQLite3Criterion &_rCriterion);

    /**
     * Add an expresion into the select.
     *
     * Contains all criterion to put between SELECT and FROM request.
     * This criterion could be count, min, max, etc... Others criterions make errors.
     * All are put near each other, separate with comma.
     *
     * @param _rExpression Expression to add.
     * @return A reference to this used to cascade \<\< operators.
     */
    wxETKSQLite3RequestSelector &       operator<<(const wxETKSQLite3Expression &_rExpression);
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
     */
    wxETKSQLite3RequestSelector &       AddOrderByAscending(const wxETKSQLite3Column& _rColumn);

    /**
     * Apply descendant sort order.
     *
     * Add this order to other ORDER BY already present.
     *
     * @param _rColumn Column on wich the sort should be apply.
     */
    wxETKSQLite3RequestSelector &       AddOrderByDescending(const wxETKSQLite3Column& _rColumn);
    //@}

    /**
     * Execute the request.
     *
     * The return class contains all rows. You could cast it into template wxETKSQLite3ResultSet class.
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
     * @param _bWithTransaction true to execute with transaction, false to execute without transaction.
     * @return The number of modified rows, -1 is error occurs. In this case, you can call wxETKSQLite3Database::GetLastErrorMessage()
     *         to have error description.
     * @throw wxSQLite3Exception If the function failed, a wxSQLite3Exception is raided so calling this function
     * must be protected by try / catch statement.
     */
    virtual int                         Execute(bool _bWithTransaction) throw(wxSQLite3Exception);

    /**
     * Allow wxETKSQLite3Database class to create wxETKSQLite3RequestInserter instance.
     */
    friend class wxETKSQLite3Database;

    /**
     * Allow wxETKSQLite3Expression class to create wxETKSQLite3RequestInserter instance without database.
     */
    friend class wxETKSQLite3Expression;
};

#endif // INCLUDE_WX_ETK_SQLITE3_REQUEST_SELECTOR_H
