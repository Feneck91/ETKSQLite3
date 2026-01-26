/////////////////////////////////////////////////////////////////////////////
/**
 * @file ETKSQLite3ResultSet.h
 * @brief Header file for updater request managment.
 *
 * Code for database resultset with automatic binding.
 *
 * @author Stéphane Château
 * @date Created: 2011/08/05
 * @date Modified: 2025/09/01
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_ETK_SQLITE3_RESULTSET_H
#define INCLUDE_ETK_SQLITE3_RESULTSET_H

#include "ETKSQLite3ResultSetBase.h"

/**
 * Class used to manage a recordset and bind automatically the datas.
 *
 * The template type must be a ETKSQLite3Record derived class.
 *
 * @author Stéphane Château
 * @version Name : TTGestLib<br>
 *          Revision : <b>1.0</b>
 */
template <class TYPE> class ETKSQLite3ResultSet : public ETKSQLite3ResultSetBase
                                                , public TYPE
{
public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Default constructor.
     */
    ETKSQLite3ResultSet();

    /**
     * Default destructor.
     */
    virtual ~ETKSQLite3ResultSet();

    /**
     * Copy constructor.
     *
     *  @param _rResultSet Object to copy from.
     */
    ETKSQLite3ResultSet(const ETKSQLite3ResultSet<TYPE>& _rResultSet);

    /**
     * Copy constructor.
     *
     *  @param _rResultSet Object to copy from.
     */
    ETKSQLite3ResultSet(const wxSQLite3ResultSet & _rResultSet);
    //@}

    /// @name Operators.
    //@{
    /**
     * Assignment operator.
     *
     * @param _rResultSet Object to copy from.
     */
    const ETKSQLite3ResultSet<TYPE> & operator=(const ETKSQLite3ResultSet<TYPE>& _rResultSet);

    /**
     * Assignment operator.
     *
     * @param _rResultSet Object to copy from.
     */
    const ETKSQLite3ResultSet<TYPE> & operator=(const wxSQLite3ResultSet &_rResultSet);
    //@}

    /**
     * Add new binding with column.
     *
     * Be careful, should be called only in ETKSQLite3Record instance.
     *
     * Example:
     * \code
     *    MyCLassRecord2 record2;
     *    ETKSQLite3ResultSet<MyCLassRecord> resultSet = selector.ExecuteQuery(); // MyCLassRecord is derived from ETKSQLite3Record
     *    // The result set contains only MyClass columns, add all of record2
     *    resultSet.AddBind(record2[MyCLassRecord2::MY_COLUMN_2]); // Contains all needed informations
     * \endcode
     *
     * @param _rColumn Column description.
     */
    void                                AddBind(const ETKSQLite3Column &_rColumn);

    /**
     * Add new binding.
     *
     * Example:
     * \code
     *    wxString strBindedString;
     *    MyClass myclassInstance;
     *    ETKSQLite3ResultSet<MyCLassRecord> resultSet = selector.ExecuteQuery(); // MyCLassRecord is derived from ETKSQLite3Record
     *    // The result set contains only MyClass columns, add new
     *    resultSet.AddBind(_T("MY_TABLE.MY_COLUMN"),&strBindedString);
     *    resultSet.AddBind(_T("MY_TABLE.MY_COLUMN_2"),ETKSQLite3ValueBindOther<MyClass>(&myclassInstance); // !! ETKSQLite3ValueBindOther<MyClass> MUST BE IMPLEMENTED !!
     * \endcode
     *
     * @param strFullColumname Full column name (as TABLE.COLUMN).
     * @param _rBindValue Value to bind. Pass your own binded class or only pointer on supported bind value type.
     */
    void                                AddBind(wxString strFullColumname,const ETKSQLite3ValueBind & _rBindValue);

    /**
     * Add new binding.
     *
     * Example:
     * \code
     *    wxString strBindedString;
     *    MyClass myclassInstance;
     *    ETKSQLite3ResultSet<MyCLassRecord> resultSet = selector.ExecuteQuery(); // MyCLassRecord is derived from ETKSQLite3Record
     *    // The result set contains only MyClass columns, add new
     *    resultSet.AddBind(MyCLassRecord2::MY_COLUMN,&strBindedString);
     *    resultSet.AddBind(MyCLassRecord2::MY_COLUMN_2,ETKSQLite3ValueBindOther<MyClass>(&myclassInstance); // !! ETKSQLite3ValueBindOther<MyClass> MUST BE IMPLEMENTED !!
     * \endcode
     *
     * @param _rColumn Column description.
     * @param _rBindValue Value to bind. Pass your own binded class or only pointer on supported bind value type.
     */
    void                                AddBind(const ETKSQLite3Column &_rColumn,const ETKSQLite3ValueBind & _rBindValue);

    /**
     * Add new binding.
     *
     * Used to bind datas on more than only one record type. Used when Join SQL instruction is used.
     * Example:
     * \code
     *    MyClass myclassInstance;
     *    MyCLassRecord2 record2;
     *    ETKSQLite3ResultSet<MyCLassRecord> resultSet = selector.ExecuteQuery(); // MyCLassRecord is derived from ETKSQLite3Record
     *    // The result set contains only MyClass columns, add all of record2
     *    resultSet.AddBind(record2);
     * \endcode
     *
     * @param _rRecord Record description.
     */
    void                                AddBind(const ETKSQLite3Record &_rRecord);

    /**
     * Add new binding.
     *
     * Used to bind datas directly with the index of the result column. Useful when used as COUNT or SUM SQL functions.
     * Example:
     * \code
     *    long lRowCount;
     *    ETKSQLite3ResultSet<ETKSQLite3Record> resultSetNbItems;     // Empty resultset, does contains nothing
     *    resultSetNbItems.AddBind(0,ETKSQLite3ValueBind(&lRowCount));  // Add the bind with result index
     *    resultSetNbItems = selector.ExecuteQuery();                     // The query could be anything, ex a COUNT(), the name is not simple to bind!
     *    resultSetNbItems.NextRow();                                     // Go to first row, the lRowCount variable is up to date!
     * \endcode
     *
     * @param _iIndexColumn Result column index (0 based index).
     * @param _rBindValue Value to bind. Pass your own binded class or only pointer on supported bind value type.
     */
    void                                AddBind(int _iIndexColumn,const ETKSQLite3ValueBind & _rBindValue);

    /**
     * Get the data structure.
     *
     * @return The data structure.
     */
    const typename TYPE::tdTypeStruct & GetResultDataStruct() const;
protected:
    /**
     * Bind datas to the structure.
     *
     * @return false if error occurs, true else.
     */
    virtual bool                        BindFrom();

    /**
     * Bind datas to the structure.
     *
     * Record columns indexes from wxSQLite3ResultSet to own columns record indexex.
     *
     * @return false if error occurs, true else.
     */
    virtual bool                        InitBindFrom();
};
// Template implementation
template <class TYPE> ETKSQLite3ResultSet<TYPE>::ETKSQLite3ResultSet()
    : ETKSQLite3ResultSetBase()
    , TYPE()
{
}

template <class TYPE> ETKSQLite3ResultSet<TYPE>::~ETKSQLite3ResultSet()
{
}

template <class TYPE> ETKSQLite3ResultSet<TYPE>::ETKSQLite3ResultSet(const ETKSQLite3ResultSet<TYPE>& _rResultSet)
    : ETKSQLite3ResultSetBase(_rResultSet)
{
}

template <class TYPE> ETKSQLite3ResultSet<TYPE>::ETKSQLite3ResultSet(const wxSQLite3ResultSet & _rResultSet)
    : ETKSQLite3ResultSetBase(_rResultSet)
{
}

template <class TYPE> const ETKSQLite3ResultSet<TYPE> & ETKSQLite3ResultSet<TYPE>::operator=(const ETKSQLite3ResultSet<TYPE> & _rResultSet)
{
    ETKSQLite3ResultSetBase::operator=(_rResultSet);
    return *this;
}

template <class TYPE> const ETKSQLite3ResultSet<TYPE> & ETKSQLite3ResultSet<TYPE>::operator=(const wxSQLite3ResultSet &_rResultSet)
{
    ETKSQLite3ResultSetBase::operator=(_rResultSet);
    return *this;
}

template <class TYPE> void ETKSQLite3ResultSet<TYPE>::AddBind(const ETKSQLite3Column &_rColumn)
{
    // Create new column from these informations
    wxASSERT_MSG(_rColumn.IsBindedValueExists(), wxT("No binded value for column into ETKSQLite3ResultSet<TYPE>::AddBind function.\nDo you forget to call operator [] on record and give only static column?"));
    TYPE::AddColumn(_rColumn);
}

template <class TYPE> void ETKSQLite3ResultSet<TYPE>::AddBind(wxString strFullColumname,const ETKSQLite3ValueBind & _rBindValue)
{
    int iFindIndex = strFullColumname.Find(_T('.'));
    if (iFindIndex != wxNOT_FOUND)
    {   // Create new column from these informations
        TYPE::AddColumn(ETKSQLite3Column(ETKSQLite3Column(strFullColumname.Left(iFindIndex),strFullColumname.Right(strFullColumname.Len() - iFindIndex -1)),ETKSQLite3ColumnAttributes(_rBindValue)));
    }
    else
    {   // Else add a column without table name (in case using AS)
        TYPE::AddColumn(ETKSQLite3Column(ETKSQLite3Column(_T(""),strFullColumname),ETKSQLite3ColumnAttributes(_rBindValue)));
    }
}

template <class TYPE> void ETKSQLite3ResultSet<TYPE>::AddBind(const ETKSQLite3Column &_rColumn,const ETKSQLite3ValueBind & _rBindValue)
{
    // Create new column from these informations
    TYPE::AddColumn(ETKSQLite3Column(_rColumn,ETKSQLite3ColumnAttributes(_rBindValue)));
}

template <class TYPE> void ETKSQLite3ResultSet<TYPE>::AddBind(const ETKSQLite3Record &_rRecord)
{
    for (size_t stIndex = 0;stIndex < _rRecord.GetColumnCount();++stIndex)
    {
        TYPE::AddColumn(_rRecord.GetColumn(stIndex));
    }
}

template <class TYPE> void ETKSQLite3ResultSet<TYPE>::AddBind(int _iIndexColumn,const ETKSQLite3ValueBind & _rBindValue)
{
    // Create new column from these informations
    TYPE::AddColumn(ETKSQLite3Column(ETKSQLite3Column(_iIndexColumn),ETKSQLite3ColumnAttributes(_rBindValue)));
}

template <class TYPE> const typename TYPE::tdTypeStruct & ETKSQLite3ResultSet<TYPE>::GetResultDataStruct() const
{
    return *this;
}

template <class TYPE> bool ETKSQLite3ResultSet<TYPE>::BindFrom()
{
    bool bRet = true;

    for (int iIndex=0;(iIndex<(int) m_arrayBindIndex.GetCount()) && bRet;++iIndex)
    {
        int iIndexBind = m_arrayBindIndex[iIndex];
        if (iIndexBind != -1)
        {   // If it is correctly binded, bind it
            TYPE::GetColumn(m_arrayBindIndex[iIndex]).BindFrom(*this,iIndex);
        }
    }

    return bRet;
}

template <class TYPE> bool ETKSQLite3ResultSet<TYPE>::InitBindFrom()
{
    wxASSERT(!m_bBindDone); // Must not be already done

    if (m_arrayBindIndex.size() == 0 && !m_bBindDone)
    {   // Create array of indexes:
        if (!m_bBindDone)
        {   // Find corresponding indexes from wxSQLite3ResultSet and my record structure
            if (wxSQLite3ResultSet::GetColumnCount()>0)
            {   // Find into record column, the corresponding indexes
                int *pIndexes  = new int[wxSQLite3ResultSet::GetColumnCount()];
                try
                {
                    // Set all contents of pIndexes to -1
                    for (int iIndexColumnToSet = 0;iIndexColumnToSet < wxSQLite3ResultSet::GetColumnCount();pIndexes[iIndexColumnToSet++]=-1);
                    int iColFound = 0; // At the end, should be equal to wxSQLite3ResultSet::GetColumnCount()
                                       // If not, all columns are not binded, the user could directly access to
                                       // these values by calling wxSQLite3ResultSet functions
                    for (size_t nIndex = 0;nIndex < TYPE::GetColumnCount();++nIndex)
                    {
                        try
                        {
                            int iIndexColumnFound;
                            iIndexColumnFound = FindColumnIndex(TYPE::GetColumn(nIndex));
                            // Here, iIndexColumnFound is always found and valid else exception is raised
                            ++iColFound;
                            wxASSERT_MSG(iIndexColumnFound >= 0 && iIndexColumnFound < wxSQLite3ResultSet::GetColumnCount(), wxT("Bad index returns by sqlite3!"));
                            /*
                            wxASSERT_MSG(pIndexes[iIndexColumnFound] == -1,
                                         wxString::Format(wxT("Index (%d) returns by sqlite3 found twice (previous = %d) - Column[%d]=%s!"),
                                                          nIndex,
                                                          pIndexes[iIndexColumnFound],
                                                          iIndexColumnFound,
                                                          TYPE::GetColumn(nIndex).GetFullName().wx_str()));

                            wxLogDebug(wxT("Column[%d] %s -> Record[%d] %s"),
                                       iIndexColumnFound,
                                       (wxSQLite3ResultSet::GetTableName(iIndexColumnFound)+wxT(".")+wxSQLite3ResultSet::GetColumnName(iIndexColumnFound)).wx_str(),
                                       nIndex,
                                       TYPE::GetColumn(nIndex).GetFullName().wx_str());
                            */
                            pIndexes[iIndexColumnFound] = static_cast<int>(nIndex); // Record index found
                        }
                        catch (wxSQLite3Exception &)
                        {   // It is not a bug, own struct just have more information than the request, just
                            // the request want a part of this struct, else it will mandatory to create a class for each
                            // request (and all columns in the result should not exists in struct)
                            wxLogDebug(wxString::Format(wxT("'%s' column couldn't be binded because it doesn't exist into wxSQLite3ResultSet recordset (it is not a bug)"),
                                                        TYPE::GetColumn(nIndex).GetFullName().wx_str()));
                        }
                    }
                    // Put indexes in correct orders into m_arrayBindIndex
                    for (iColFound=0;iColFound<wxSQLite3ResultSet::GetColumnCount();++iColFound)
                    {
                        m_arrayBindIndex.Add(pIndexes[iColFound]);
                        if (pIndexes[iColFound] == -1)
                        {
                            wxLogDebug(wxString::Format(wxT("wxSQLite3ResultSet '%s.%s' column is not be binded because the matched column's data doesn't exists (it is not a bug)"),
                                                        wxSQLite3ResultSet::GetTableName(iColFound).wx_str(),wxSQLite3ResultSet::GetColumnName(iColFound).wx_str()));
                        }
                    }
                    if (iColFound != wxSQLite3ResultSet::GetColumnCount())
                    {
                        wxLogDebug(wxString::Format(wxT("Binding data: cannot found all the resultset columns into own structure, found only %d/%d"),
                                                    iColFound,
                                                    wxSQLite3ResultSet::GetColumnCount()));
                    }
                }
                catch(...)
                {
                    delete []pIndexes;
                    throw;
                }
                delete []pIndexes;
            }
        }
    }
    m_bBindDone = true;
    return (m_arrayBindIndex.size() > 0);
}

#endif // INCLUDE_ETK_SQLITE3_RESULTSET_H
