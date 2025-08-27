/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3RequestUpdater.h
// Library:     ETKSQLite3
// Purpose:     Code to make update into database
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/05
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_ETK_SQLITE3_REQUEST_UPDATER_H
#define INCLUDE_ETK_SQLITE3_REQUEST_UPDATER_H

#include "ETKSQLite3Types.h"
#include "ETKSQLite3Request.h"
#include "ETKSQLite3ResultSet.h"
#include "ETKSQLite3Record.h"
#include <list>

/**
 * Class used to make update request.
 *
 * Is used to update rows into table. Unlike selection request, this class update column field
 * directly with binding without convert into text (it is <i>wxSQLite3Statement</i> that makes
 * the last string conversion).
 *
 * @see ETKSQLite3Request
 * @author Stéphane Château
 * @version Nom : ETKSQLite3<br>
 *          Révision : <b>1.0</b>
 */
class EXPORT_IMPORT ETKSQLite3RequestUpdater : public ETKSQLite3Request
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
    ETKSQLite3RequestUpdater(ETKSQLite3Database & _rDatabase);

public:
    /**
     * Copy constructor.
     *
     * @param _rRequestInserter Element to copy into this.
     */
    ETKSQLite3RequestUpdater(const ETKSQLite3RequestUpdater & _rRequestUpdater);

    /** Destructeur.
    *
    */
    virtual ~ETKSQLite3RequestUpdater();
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
    void                                operator<<(const ETKSQLite3Record &_rRecord);

    /**
     * Add a column of the table into the inserter.
     *
     * Make the column binding, All column inserted must be on the same table.
     *
     * @param _rColumn Column to add, contains value and column description.
     * @return A reference to this used to cascade \<\< operators.
     */
    ETKSQLite3RequestUpdater &        operator<<(const ETKSQLite3Column &_rColumn);

    /**
     * Add an expression into the select.
     *
     * Used to put binded value to update.
     * All are put near each other, separate with comma.
     *
     * @param _rExpression Expression to add.
     * @return A reference to this used to cascade \<\< operators.
     */
    ETKSQLite3RequestUpdater &        operator<<(const ETKSQLite3Expression &_rExpression);
    //@}

protected:
    /**
     * Used into adding columns function to add with correct expression type.
     *
     * This type only differs from update to insert request, so operators could be defined
     * into this base class.
     *
     * @return Always ETKSQLite3Expression::eOperationAssignmentUpdate.
     */
    virtual ETKSQLite3Expression::eExpressionOrOperationType GetOperationAssigment() const;

    /**
     * Allow ETKSQLite3Database class to create ETKSQLite3RequestInserter instance.
     */
    friend class ETKSQLite3Database;
};

#endif // INCLUDE_ETK_SQLITE3_REQUEST_UPDATER_H
