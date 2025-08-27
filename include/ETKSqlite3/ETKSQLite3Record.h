/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3Record.cpp
// Library:     ETKSQLite3
// Purpose:     Code for database table/request managment
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/05
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_ETK_SQLITE3_RECORD_H
#define INCLUDE_ETK_SQLITE3_RECORD_H

#include "ETKSQLite3Types.h"
#include "ETKSQLite3Column.h"
#include <vector>

/** \page Binding Binding datas: how to.

\section binding Binding datas
    How to bind data ?
    Explain how to bind data
*/

/**
 * Class used to manage the record stucture.
 *
 * It contains a list of columns with attributes.
 *
 * @author Stéphane Château
 * @version Name : ETKSQLite3<br>
 *          Revision : <b>1.0</b>
 */
class EXPORT_IMPORT ETKSQLite3Record
{
private:
    /**
     * Type of column list, with vector, faster access than list.
     */
    typedef std::vector<ETKSQLite3Column> tdListColumns;

    /**
     * Columns list.
     */
    tdListColumns                       m_lstColumns;

    /**
     * Table name, used for table.
     *
     * If this record used more than one table, you should not use it.
     */
    wxString                            m_strTableName;
public:
    /**
     * Typedef of the data structure that contains no datas fields.
     *
     * Only use to create an empty ResultSet as "ETKSQLite3ResultSet<ETKSQLite3Record>" instance.
     */
    typedef ETKSQLite3Record tdTypeStruct;

public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Constructor.
     *
     * Default constructor, used when this record used more than one table.
     */
    ETKSQLite3Record();

    /**
     * Constructor.
     *
     * This record is used when binded to one table.
     * If this record used more than one table, don't use this constructor.
     *
     * @param _strTableName Table name.
     */
    ETKSQLite3Record(wxString _strTableName);

    /**
     * Destructor.
     */
    virtual ~ETKSQLite3Record();
    //@}

    /**
     * Used for expression to get TABLENAME\.*.
     *
     * @param _strTableName Table name
     * @return A Column that can use as expression.
     */
    static ETKSQLite3Column           GetAllColumn(wxString _strTableName);

    /**
     * Get the record name.
     *
     * @return The table name.
     */
    wxString                            GetTableName() const;

    /**
     * Add column into the list of columns of this record.
     *
     * @param _rColumn New column to add.
     */
    void                                AddColumn(const ETKSQLite3Column &_rColumn);

    /**
     * Merge this record with another one.
     *
     * Add all columns of record into the list of internal parameters.
     *
     * @param _rRecord Record to add to this.
     * @return A reference to this.
     */
    const ETKSQLite3Record &          MergeRecord(const ETKSQLite3Record &_rRecord);

    /**
     * Set the table name.
     *
     * Change the table name and all table column name.
     *
     * @param _strTableName New table name.
     * @return A reference to this.
     */
    const ETKSQLite3Record &          ChangeTableName(wxString _strTableName);

    /**
     * Generate expression with an 'AS' SQL on each column.
     *
     * When using the As on column you can choose the as value, in the case of using on record
     * you cannot choose this value, it is also automatically computed by concatenate the new table name
     * with the column name for each columns.
     *
     * @see ChangeAs,ETKSQLite3Column::As
     * @param _strTableName New table name.
     * @return A generated expression.
     */
    ETKSQLite3Expression              As(wxString _strTableName) const;

    /**
     * Modify the table to be used as ResultSet record <b>ONLY</b>!
     *
     * Used to bind data into record when the result set is generate with an SQL expression
     * that use the <b>AS</b> expression from the record. Will call \ref ETKSQLite3Column::ChangeAs "ChangeAs"
     * on each column.
     *
     * When using the \ref ETKSQLite3Column::As "As" function on column you can choose the as value, in the case of using on record
     * you cannot choose this value, it is also automatically computed by concatenate the new table name
     * with the column name for each columns.
     *
     * @see As,ETKSQLite3Column::ChangeAs
     * @param _strTableName The AS table name.
     */
    void                                ChangeAs(wxString _strTableName);

    /**
     * Get the number of column.
     *
     * @return The number of columns.
     */
    size_t                              GetColumnCount() const;

    /**
     * Get a column.
     *
     * @param _nIndex Column index to find.
     * @return A column information corresponding to this index.
     */
    const ETKSQLite3Column &            GetColumn(size_t _nIndex) const;

    /**
     * Search a column from its name.
     *
     * @param _strColumnName Column name (without table name) to find.
     * @return A const pointer of column, NULL if nnot found.
     */
    const ETKSQLite3Column *            FindColumnByName(wxString _strColumnName) const;

    /**
     * Search a column from its full name (table name + column name).
     *
     * @param _strColumnFullName Column name (with table name) to find.
     * @return A const pointer of column, NULL if nnot found.
     */
    const ETKSQLite3Column *            FindColumnByFullName(wxString _strColumnFullName) const;

    /**
     * Search a dynamic columns (with value binding) of a columns given into parameter.
     *
     * @param _rColumn Must be a static column, we must found dynamic one (assign with a value).
     * @return An instance of dynamic column that contains value binding. There is no error managment, this column <b>MUST</b> be found.
     * @throws wxSQLite3Exception with error code to -1 if the column is not found, this must NEVER arrives!
     */
    const ETKSQLite3Column &            operator[](const ETKSQLite3Column &_rColumn) const;

    /**
     * Get the columns attribute from the index column.
     *
     * @param _Index Index of the columns.
     * @return A column information corresponding to this index.
     */
    const ETKSQLite3ColumnAttributes &  GetColumnAttributes(int _Index) const;

private:
    /**
     * Operator = is not allowed.
     *
     * Just defined here, no code into cpp, just to forbid it.
     */
    const ETKSQLite3Record &            operator=(const ETKSQLite3Record &_rRecord);
};

#endif // INCLUDE_ETK_SQLITE3_RECORD_H
