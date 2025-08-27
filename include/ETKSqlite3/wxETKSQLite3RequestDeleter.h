/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3RequestDeleter.h
// Library:     wxETKSQLite3
// Purpose:     Code to make delete into database
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/09
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_WX_ETK_SQLITE3_REQUEST_DELETER_H
#define INCLUDE_WX_ETK_SQLITE3_REQUEST_DELETER_H

#include "wxETKSQLite3Types.h"
#include "wxETKSQLite3Request.h"
#include "wxETKSQLite3ResultSet.h"
#include "wxETKSQLite3Record.h"
#include <list>

/**
 * Class used to make delete request.
 *
 * Is used to delete rows into table.
 *
 * @see wxETKSQLite3Request
 * @author Stéphane Château
 * @version Nom : wxETKSQLite3<br>
 *          Révision : <b>1.0</b>
 */
class EXPORT_IMPORT wxETKSQLite3RequestDeleter : public wxETKSQLite3Request
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
    wxETKSQLite3RequestDeleter(wxETKSQLite3Database & _rDatabase);

public:
    /**
     * Copy constructor.
     *
     * @param _rRequestDeleter Element to copy into this.
     */
    wxETKSQLite3RequestDeleter(const wxETKSQLite3RequestDeleter & _rRequestDeleter);

    /** Destructeur.
    *
    */
    virtual ~wxETKSQLite3RequestDeleter();
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
    void                                operator<<(const wxETKSQLite3Record &_rRecord);

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
     * Allow wxETKSQLite3Database class to create wxETKSQLite3RequestInserter instance.
     */
    friend class wxETKSQLite3Database;
};

#endif // INCLUDE_WX_ETK_SQLITE3_REQUEST_DELETER_H
