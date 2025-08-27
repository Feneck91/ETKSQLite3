/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3ResultSetBase.h
// Library:     ETKSQLite3
// Purpose:     Code for database resultset with automatic binding (base class)
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/09/30
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_ETK_SQLITE3_RESULTSET_BASE_H
#define INCLUDE_ETK_SQLITE3_RESULTSET_BASE_H

#include "ETKSQLite3Types.h"
#include "ETKSQLite3Record.h"

/**
 * Class used to manage a recordset and bind automatically the datas.
 *
 * The template type must be a ETKSQLite3Record derived class.
 *
 * @author Stéphane Château
 * @version Name : TTGestLib<br>
 *          Revision : <b>1.0</b>
 */
class EXPORT_IMPORT ETKSQLite3ResultSetBase : public wxSQLite3ResultSet
{
protected:
    /**
     * Used to convert array indexes with Columns names.
     */
    wxArrayInt                          m_arrayBindIndex;

    /**
     * Indicate if the bind is already done or not.
     */
    bool                                m_bBindDone;
public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Default constructor.
     */
    ETKSQLite3ResultSetBase();

    /**
     * Default destructor.
     */
    virtual ~ETKSQLite3ResultSetBase();

    /**
     * Copy constructor.
     *
     *  @param _rResultSet Object to copy from.
     */
    ETKSQLite3ResultSetBase(const ETKSQLite3ResultSetBase& _rResultSet);

    /**
     * Copy constructor.
     *
     *  @param _rResultSet Object to copy from.
     */
    ETKSQLite3ResultSetBase(const wxSQLite3ResultSet & _rResultSet);
    //@}

    /// @name Operators.
    //@{
    /**
     * Assignment operator.
     *
     * @param _rResultSet Object to copy from.
     */
    const ETKSQLite3ResultSetBase &   operator=(const ETKSQLite3ResultSetBase& _rResultSet);

    /**
     * Assignment operator.
     *
     * @param _rResultSet Object to copy from.
     */
    const ETKSQLite3ResultSetBase &   operator=(const wxSQLite3ResultSet &_rResultSet);
    //@}

    /** @name Override wxSQLite3ResultSet methods.
     *
     * Used to bind the content of the record after moving to next.
     */
    //@{
    /**
    * Advances the cursor to the next row.
    * On creation of the result set the cursor is positioned BEFORE the first row, i.e.
    * the first call to this method makes the first row available for processing.
    * \return TRUE while there are still rows to process, FALSE otherwise
    */
    virtual bool                        NextRow();
    //@}

protected:
    /**
     * Bind datas to the structure.
     *
     * @return false if error occurs, true else.
     */
    virtual bool                        BindFrom() = 0;

    /**
     * Bind datas to the structure.
     *
     * Record columns indexes from wxSQLite3ResultSet to own columns record indexex.
     *
     * @return false if error occurs, true else.
     */
    virtual bool                        InitBindFrom() = 0;

    /**
     * Find a column index that match table name and column name.
     *
     * @param _rColumn Column information to found.
     * @return The index of the column found.
     * @throw wxSQLite3Exception if the columns cannot be found.
     */
    int                                 FindColumnIndex(const ETKSQLite3Column &_rColumn);
};

#endif // INCLUDE_ETK_SQLITE3_RESULTSET_BASE_H
