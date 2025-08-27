/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3RequestUpdater.h
// Library:     wxETKSQLite3
// Purpose:     Code to make update into database
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/05
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_WX_ETK_SQLITE3_REQUEST_UPDATER_H
#define INCLUDE_WX_ETK_SQLITE3_REQUEST_UPDATER_H

#include "wxETKSQLite3Types.h"
#include "wxETKSQLite3Request.h"
#include "wxETKSQLite3ResultSet.h"
#include "wxETKSQLite3Record.h"
#include <list>

/**
 * Class used to make update request.
 *
 * Is used to update rows into table. Unlike selection request, this class update column field
 * directly with binding without convert into text (it is <i>wxSQLite3Statement</i> that makes
 * the last string conversion).
 *
 * @see wxETKSQLite3Request
 * @author Stéphane Château
 * @version Nom : wxETKSQLite3<br>
 *          Révision : <b>1.0</b>
 */
class EXPORT_IMPORT wxETKSQLite3RequestUpdater : public wxETKSQLite3Request
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
    wxETKSQLite3RequestUpdater(wxETKSQLite3Database & _rDatabase);

public:
    /**
     * Copy constructor.
     *
     * @param _rRequestInserter Element to copy into this.
     */
    wxETKSQLite3RequestUpdater(const wxETKSQLite3RequestUpdater & _rRequestUpdater);

    /** Destructeur.
    *
    */
    virtual ~wxETKSQLite3RequestUpdater();
    //@}

    /**
     * Get the row id of last inserted row.
     *
     * @return Last row identifier, 0 if error or not already initialized.
     */
    wxLongLong                          GetLastRowId() const;

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
     * @param _rRecord Record to update/insert.
     */
    void                                operator<<(const wxETKSQLite3Record &_rRecord);

    /**
     * Add a column of the table into the inserter.
     *
     * Make the column binding, All column inserted must be on the same table.
     *
     * @param _rColumn Column to add, contains value and column description.
     * @return A reference to this used to cascade \<\< operators.
     */
    wxETKSQLite3RequestUpdater &        operator<<(const wxETKSQLite3Column &_rColumn);

    /**
     * Add an expression into the select.
     *
     * Used to put binded value to update.
     * All are put near each other, separate with comma.
     *
     * @param _rExpression Expression to add.
     * @return A reference to this used to cascade \<\< operators.
     */
    wxETKSQLite3RequestUpdater &        operator<<(const wxETKSQLite3Expression &_rExpression);
    //@}

protected:
    /**
     * Used into adding columns function to add with correct expression type.
     *
     * This type only differs from update to insert request, so operators could be defined
     * into this base class.
     *
     * @return Always wxETKSQLite3Expression::eOperationAssignmentUpdate.
     */
    virtual wxETKSQLite3Expression::eExpressionOrOperationType GetOperationAssigment() const;

    /**
     * Allow wxETKSQLite3Database class to create wxETKSQLite3RequestInserter instance.
     */
    friend class wxETKSQLite3Database;
};

#endif // INCLUDE_WX_ETK_SQLITE3_REQUEST_UPDATER_H
