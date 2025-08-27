/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3RequestInserter.h
// Library:     wxETKSQLite3
// Purpose:     Code to make insert into database
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/05
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_WX_ETK_SQLITE3_REQUEST_INSERTER_H
#define INCLUDE_WX_ETK_SQLITE3_REQUEST_INSERTER_H

#include "wxETKSQLite3Types.h"
#include "wxETKSQLite3RequestUpdater.h"

/**
 * Class used to make insert request.
 *
 * Is used to insert rows into table. Unlike selection request, this class update column field
 * directly with binding without convert into text (it is <i>wxSQLite3Statement</i> that makes
 * the last string conversion).<br>
 * This class need the schema of the table (the <i>wxETKSQLite3Record</i> class) to know each
 * column table's name and make the binding. This class is based on updater class.
 *
 * @see wxETKSQLite3RequestUpdater
 * @author Stéphane Château
 * @version Nom : wxETKSQLite3<br>
 *          Révision : <b>1.0</b>
 */
class EXPORT_IMPORT wxETKSQLite3RequestInserter : public wxETKSQLite3RequestUpdater
{
    /// @name Constructor / Destructor.
    //@{
protected:
    /**
     * Protected constructor.
     *
     * This constructor is private, only <i>wxETKSQLite3Database::GetInserter()</i> could return
     * an instance of this class.
     *
     * @param _rDatabase Reference on database.
     */
    wxETKSQLite3RequestInserter(wxETKSQLite3Database & _rDatabase);
public:
    /**
     * Copy constructor.
     *
     * @param _rRequestInserter Element to copy into this.
     */
    wxETKSQLite3RequestInserter(const wxETKSQLite3RequestInserter & _rRequestInserter);

    /**
     * Destructor.
     */
    virtual ~wxETKSQLite3RequestInserter();
    //@}

protected:
    /**
     * Used into adding columns function to add with correct expression type.
     *
     * This type only differs from update to insert request, so operators could be defined
     * into the update base class.
     *
     * @return Always wxETKSQLite3Expression::eOperationAssignmentInsert.
     */
    virtual wxETKSQLite3Expression::eExpressionOrOperationType GetOperationAssigment() const;

    /**
     * Allow wxETKSQLite3Database class to create wxETKSQLite3RequestInserter instance.
     */
    friend class wxETKSQLite3Database;
};

#endif // INCLUDE_WX_ETK_SQLITE3_REQUEST_INSERTER_H
