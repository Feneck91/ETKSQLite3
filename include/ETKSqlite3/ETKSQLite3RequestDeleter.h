/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3RequestDeleter.h
// Library:     ETKSQLite3
// Purpose:     Code to make delete into database
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/09
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_ETK_SQLITE3_REQUEST_DELETER_H
#define INCLUDE_ETK_SQLITE3_REQUEST_DELETER_H

#include "ETKSQLite3Types.h"
#include "ETKSQLite3Request.h"
#include "ETKSQLite3ResultSet.h"
#include "ETKSQLite3Record.h"
#include <list>

/**
 * Class used to make delete request.
 *
 * Is used to delete rows into table.
 *
 * @see ETKSQLite3Request
 * @author Stéphane Château
 * @version Nom : ETKSQLite3<br>
 *          Révision : <b>1.0</b>
 */
class EXPORT_IMPORT ETKSQLite3RequestDeleter : public ETKSQLite3Request
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
    ETKSQLite3RequestDeleter(ETKSQLite3Database & _rDatabase);

public:
    /**
     * Copy constructor.
     *
     * @param _rRequestDeleter Element to copy into this.
     */
    ETKSQLite3RequestDeleter(const ETKSQLite3RequestDeleter & _rRequestDeleter);

    /** Destructeur.
    *
    */
    virtual ~ETKSQLite3RequestDeleter();
    //@}

    /** @name Operators.
     *
     * Are used to add fields or record to this request.
     */
    //@{
    /**
     * Set the table name, only one (and at least one) record (table name) is allowed.
     *
     * Calling this operator more than once overwrite previous values.
     *
     * @param _rRecord Record (table) where delete row(s).
     */
    void                                operator<<(const ETKSQLite3Record &_rRecord);

    /**
     * Set the table name, only one (and at least one) record (table name) is allowed.
     *
     * Calling this operator more than once overwrite previous values.
     *
     * @param _strTableName Table name where delete row(s).
     */
    void                                operator<<(wxString _strTableName);
    //@}

protected:
    /**
     * Allow ETKSQLite3Database class to create ETKSQLite3RequestInserter instance.
     */
    friend class ETKSQLite3Database;
};

#endif // INCLUDE_ETK_SQLITE3_REQUEST_DELETER_H
