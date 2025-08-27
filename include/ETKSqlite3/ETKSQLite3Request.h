/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3Request.h
// Library:     ETKSQLite3
// Purpose:     Code for request managment (base class)
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/04
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_ETK_SQLITE3_REQUEST_H
#define INCLUDE_ETK_SQLITE3_REQUEST_H

#include "ETKSQLite3Types.h"
#include "ETKSQLite3Criterion.h"

class ETKSQLite3Database;

/** Base class use to make request.
 *
 * It allow derived class to correctly format INSERT, UPDATE, DELETE, SELECT requests.
 *
 * @author Stéphane Château
 * @version Name : ETKSQLite3<br>
 *          Revision : <b>1.0</b>
 */
class EXPORT_IMPORT ETKSQLite3Request
{
private:
    /**
     * Reference on database.
     */
    ETKSQLite3Database &              m_rDatabase;

    /**
     * Record the last row id of the last request.
     */
    wxLongLong                          m_llLastRowID;

    /**
     * Criterion instance.
     *
     * This criterion is used to construct the request.
     */
    ETKSQLite3Criterion               m_criterionRequest;

    /**
     * Statement that will make the request.
     */
    wxSQLite3Statement *                m_pStatement;

protected:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Constructor.
     *
     * Allowed only for derived classes.
     *
     * @param _rDatabase Reference on database.
     */
    ETKSQLite3Request(ETKSQLite3Database & _rDatabase);

    /**
     * Copy constructor.
     *
     * @param _rOther Element to copy to this.
     */
    ETKSQLite3Request(const ETKSQLite3Request & _rOther);

public:
    /**
     * Destructor.
     */
    virtual ~ETKSQLite3Request();
    //@}

    /**
     * Execute the request.
     *
     * The return value indicate the number of modified rows in update request. For an inserter
     * request it can be -1 (failed) or 1 (succeeded).<br>
     *
     * A transaction is used, to call Execute without a transaction, call ExecuteWithoutTransaction.
     *
     * Call the Execute function.
     *
     * @see ExecuteWithoutTransaction
     * @param _bLogError If true, log database error.
     * @param _bDisplayMsgBox If true, display the error to the user.
     * @return The number of modified rows, -1 is error occurs. In this case, you can call ETKSQLite3Database::GetLastErrorMessage()
     *         to have error description.
     */
    int                                 ExecuteWithTransaction(bool _bLogError = true, bool _bDisplayMsgBox = true);

    /**
     * Execute the request without using transaction.
     *
     * The return value indicate the number of modified rows in update request. For an inserter
     * request it can be -1 (failed) or 1 (succeeded).<br>
     *
     * No transaction are used, to call Execute with a transaction, call ExecuteWithTransaction.
     *
     * Call the Execute function.
     *
     * @see ExecuteWithTransaction
     * @param _bLogError If true, log database error.
     * @param _bDisplayMsgBox If true, display the error to the user.
     * @return The number of modified rows, -1 is error occurs. In this case, you can call ETKSQLite3Database::GetLastErrorMessage()
     *         to have error description.
     * @throw wxSQLite3Exception If the function failed, a wxSQLite3Exception is raided so calling this function
     *                           must be protected by try / catch statement.
     */
    int                                 ExecuteWithoutTransaction(bool _bLogError = true, bool _bDisplayMsgBox = true);

    /**
     * Get the criterion used to construct the request.
     *
     * @return A const reference to internal criterion request.
     */
    const ETKSQLite3Criterion &       GetCriterionRequest() const;

    /**
     * Get the criterion used to construct the request.
     *
     * @return A reference to internal criterion request.
     */
    ETKSQLite3Criterion &             GetCriterionRequest();

    /**
     * Set the Where expression.
     *
     * Same as GetCriterionRequest().SetWhere() function.
     *
     * @param _rExprWhere Where expression.
     */
    void                                Where(const ETKSQLite3Expression &_rExprWhere);

    /**
     * Get the Where expression.
     *
     * Same as GetCriterionRequest().GetWhere() function.
     *
     * @return The Where expression.
     */
    const ETKSQLite3Expression &      GetWhere();

    /**
     * Set the Join expression.
     *
     * Same as GetCriterionRequest().SetJoin() function.
     *
     * @param _rExprJoin Join expression.
     */
    void                                Join(const ETKSQLite3Expression &_rExprJoin);

    /**
     * Clear the current request.
     *
     * After calling this function, the instance of this class could be reuse.
     */
    virtual void                        Clear();
protected:
    /**
     * Execute the request with ou without using transaction.
     *
     * The return value indicate the number of modified rows in update request. For an inserter
     * request it can be -1 (failed) or 1 (succeeded).
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
    virtual int                         Execute(bool _bLogError, bool _bDisplayMsgBox, bool _bWithTransaction);

    /**
     * Get the row id of last inserted/update row.
     *
     * Avalaible only for inserter and updater.
     *
     * @return Last row identifier, 0 if error or not already initialized.
     */
    wxLongLong                          GetLastRowId() const;

    /**
     * Get the database reference.
     *
     * Only for internal use for derived class.
     *
     * @return The database reference.
     */
    ETKSQLite3Database &              GetDatabase() const;

    /**
     * Initialize the request statement.
     *
     * Only for internal use for derived class.
     *
     * @param _strSQLRequest SQL request needed to create statement. If database is closed, the
     *                       statement will be empty.
     * @return true if the statement correctly created, false else.
     * @see PrepareStatement
     */
    bool                                InitStatement(wxString _strSQLRequest);

    /**
     * Prepare the statement.
     *
     * First call InitStatement with computed SQL request.
     *
     * @return true if the statement is ready, false else.
     * @see InitStatement
     */
    virtual bool                        PrepareStatement();

    /**
     * Indicate if the statement is initialized or not.
     *
     * @return true if the statement is initialized, false else.
     */
    bool                                IsStatementInitialized() const;

    /**
     * Delete the statement.
     *
     * Used by selector class to use the statement once, else it crash.
     */
    void                                DeleteStatement();

    /**
     * Get the current statement.
     *
     * @return The current statemet.
     */
    wxSQLite3Statement &                GetStatement();

   /**
     * Bind the statement.
     *
     * Make the bind, could be override by derived classes if needed.
     *
     * @param _rStatement Statement to bind.
     * @return true if the bind succeeded.
     */
    virtual bool                        Bind(wxSQLite3Statement &_rStatement);
};

#endif // INCLUDE_ETK_SQLITE3_REQUEST_H
