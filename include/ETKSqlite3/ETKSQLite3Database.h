/////////////////////////////////////////////////////////////////////////////
/**
 * @file ETKSQLite3Database.h
 * @brief Header file for SQL request creation.
 *
 * This file contains the headers for the ETKSQLite3Database class
 * which is used to manage SQLite3 database into ETKSQLite3 library.
 *
 * @author Stéphane Château du header
 * @date Created: 2011/07/31
 * @date Modified: 2025/08/29
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_ETK_SQLITE3_DATABASE_H
#define INCLUDE_ETK_SQLITE3_DATABASE_H

#include "ETKSQLite3Types.h"
#include "ETKSQLite3RequestInserter.h"
#include "ETKSQLite3RequestDeleter.h"
#include "ETKSQLite3RequestSelector.h"
#include <wx/filename.h>

/** \mainpage ETKSQLite3

\section intro What is ETKSQLite3?

    \b ETKSQLite3 is a C++ wrapper around the public domain
    written in C++ that allow to access <a href="http://www.sqlite.org/">SQLite3</a> database using the <a href="https://github.com/utelle/wxsqlite3">wxSQLite3</a> wrapper library.<br>
    This library is used to allow user to quickly make database binding and make easy request with C++ operators
    and is specifically designed for use in programs based on the \b <a href="http://www.wxwidgets.org/">wxWidgets</a> library.
    A workaround has been made to make it works with \b <a href="http://qt.nokia.com/products/">QT</a> Library.<br>

    Since \b SQLite3 stores strings in UTF-8 encoding, the  \b wxSQLite3 methods provide automatic conversion between wxString and UTF-8 strings.
    The methods ToUTF8 and FromUTF8 of the wxString class (available since  \b wxWidgets 2.8.4) are used for the conversion. Special care has to be taken if external administration
    tools are used to modify the database contents, since not all of these tools operate in Unicode or UTF-8 mode.

    Since version 1.1, \b ETKSQLite3 allowed to be used with \b QT library. All needed \b wxWidgets classes are re-written around  \b QT
    library classes (based on macros or directly by implementing these \b wxWidgets classes by deriving \b QT ones).
    <br><br><b>Be careful, the QT workaround is not done to use wxWidgets classes instead of QT classes. All wxWidgets classes has been tested with ETKSQLite3 but are not
    been implements to be use instead of QT classes. Implementing a software based on ETKSQLite3 library must use all QT classes.</b>

\section version Version history

    <dl>
    <dt><b>1.2</b> - <i>September 2025</i></dt>
    <dd>
    Boost Integration...
    </dd>
    <dt><b>1.1</b> - <i>July 2012</i></dt>
    <dd>
    QT Integration...
    </dd>
    <dt><b>1.0</b> - <i>December 2011</i></dt>
    <dd>
    First release - Not published...
    </dd>
    </dl>

\author Stéphane Château (Feneck91)

\section ackn Acknowledgements

    Kudos to Ulrich Telle (and all wxSQLite3 contributors) for developing the wxSQLite3 library used by ETKSQLite3.

\subpage Installation
*/

/** \page Installation Getting started: installation

    This page will explain how to compile ETKSQLite3 with \b Code::Blocks or with <b>Microsoft Visual Studio</b> IDE.<br>
    <br>
    First, you need to download <a href="http://wxcode.sourceforge.net/components/wxsqlite3/">wxSQLite3</a> library and decompress
    all source code in a folder.<br>

    <br>
    You can compile \b ETKSQLite3 in differents way :
    <ul>
        <li> The original based on \b wxWidgets library: \ref Installation_wx.</li>
            <ul>
                <li> \ref Installation_wx_CodeBlocks.</li>
                <li> \ref Installation_wx_Visual_Studio.</li>
            </ul>
        <li> With workaround based on \b QT library.</li>
            <ul>
                <li> \ref Installation_qt_CodeBlocks Code::Blocks IDE.</li>
                <li> \ref Installation_qt_Visual_Studio Microsoft Visual Studio.</li>
            </ul>
    </ul>

    \subpage Installation_wx_CodeBlocks Install
*/

/** \page Installation_wx Installation wxWidgets Based

    To make \b ETKSQLite3 works with \b wxWidgets library first you need to compile \b wxWidgets library.
    <br>

    Download <a href="http://www.wxwidgets.org/downloads/">wxWidgets</a> library and compile it.
    This is an example to compile \b wxWidgets with MinGW:
    \code
    @ECHO OFF
    SET WX_COMPILE_VERSION=2.9.3

    PATH=C:\MinGW\bin;C:\MinGW\mingw32\bin;%PATH%

    REM Record current folder path
    pushd .
    ECHO -----------------------------------
    ECHO Compilation de WxWidgets %WX_COMPILE_VERSION% Debug
    ECHO -----------------------------------
    cd wxWidgets-%WX_COMPILE_VERSION%\build\msw

    mingw32-make -f makefile.gcc USE_XRC=1 SHARED=1 MONOLITHIC=1 BUILD=debug UNICODE=1 clean
    mingw32-make -f makefile.gcc USE_XRC=1 SHARED=1 MONOLITHIC=1 BUILD=debug UNICODE=1

    ECHO -----------------------------------
    ECHO Compilation de WxWidgets %WX_COMPILE_VERSION% Release
    ECHO -----------------------------------
    cd wxWidgets-%WX_COMPILE_VERSION%\build\msw

    mingw32-make -f makefile.gcc USE_XRC=1 SHARED=1 MONOLITHIC=1 BUILD=release UNICODE=1 DEBUG_FLAG=0 clean
    mingw32-make -f makefile.gcc USE_XRC=1 SHARED=1 MONOLITHIC=1 BUILD=release UNICODE=1 DEBUG_FLAG=0

    REM Restore initial path
    popd
    pause
    \endcode

     <li> Download and install <a href="http://www.codeblocks.org/">Code::Blocks</a> compiler or install <a href="http://msdn.microsoft.com/fr-fr/vstudio/aa718325.aspx">Microsoft Visual Studio</a>.</li>
\subpage Installation_wx_CodeBlocks
\subpage Installation_wx_VisualStudio

*/

/** \page Installation_wx_CodeBlocks Installation wxWidgets Based - Code::Blocks IDE
    \section Code::Blocks IDE
    <ul>
     <li> Download and install <a href="http://www.codeblocks.org/">Code::Blocks</a> compiler.</li>
     <li> Download <a href="http://www.wxwidgets.org/">wxWidgets</a> library and compile it or install <a href="http://qt.nokia.com/products/">QT</a> library.</li>
     <li> Download <a href="http://wxcode.sourceforge.net/components/wxsqlite3/">wxSQLite3</a> library and decompress source code in a folder.</li>
     <li> Open project named:
        <ul>
            <li> \b wxWidgets based / <b>Code::Blocks</b> project: <i>ETKSQLite3/build/ETKSQLite3.cbp</i></li>
            <li> \b wxWidgets based / <b>Microsoft Visual Studio</b> project: <i>not implemented yet</i></li>
            <li> \b QT based / <b>Code::Blocks</b> project: <i>ETKSQLite3/build/qtETKSQLite3.cbp</i></li>
            <li> \b QT based / <b>Microsoft Visual Studio</b> project: <i>ETKSQLite3/build/qtETKSQLite3.vcproj</li>
        </ul>
      <ul>
        <li> Select needed target (Debug / Release / Static or Dynamic library).</li>
        <ul> <b>Code::Blocks</b>
            <li> Build ETKSQLite3. You need to update global variables into settings:</li>
              <ul>
                <li> <b>wx</b>:
                    <ul>
                        <li>base: wxWidgets library root folder.</li>
                        <li>lib: $(#wx)\\lib\\gcc_dll <i>(<a href="http://wiki.wxwidgets.org/Compiling_wxWidgets_with_MinGW">gcc compiler</a>)</i> / $(#wx)\\lib\\vc_dll <i>(for <a href="http://wiki.wxwidgets.org/Microsoft_Visual_C%2B%2B_Guide">
                                                                                                          Visual Studio compiler)</a></i></li>
                        <li>verlib: wxWidgets version (28 for \b wxWidgets 2.8.x, 29 for \b wxWidgets 2.8.x). <i>(User-defined fields)</i></li>
                        <li>verdll: wxWidgets version dll ((28 for wxWidgets 2.8.x, 292 for wxWidgets 2.9.2, 293 for wxWidgets 2.9.3). <i>(User-defined fields)</i></li>
                    </ul>
                </li>
                <li> <b>wxsqlite3</b>: wxSQLite3 library root folder.</li>
                <li> <b>wxetksqlite3</b>: ETKSQLite3 library root folder (for latter uses).</li>
          </ul>
        </li>
        <li>Your ETKSQLite3 is now compiled into <i>ETKSQLite3/lib</i> folder.</li>
    </ul>
*/

/**
\section Installation_wx_VisualStudio Installation wxWidgets Based - Visual Studio IDE
    <ul>
     <li> Install <a href="http://msdn.microsoft.com/fr-fr/vstudio/aa718325.aspx">Microsoft Visual Studio</a>.</li>
     <li> Download <a href="http://wxcode.sourceforge.net/components/wxsqlite3/">wxSQLite3</a> library and decompress source code in a folder.</li>
     <li> Open project named:
        <ul>
            <li> \b wxWidgets based / <b>Code::Blocks</b> project: <i>ETKSQLite3/build/ETKSQLite3.cbp</i></li>
            <li> \b wxWidgets based / <b>Microsoft Visual Studio</b> project: <i>not implemented yet</i></li>
            <li> \b QT based / <b>Code::Blocks</b> project: <i>ETKSQLite3/build/qtETKSQLite3.cbp</i></li>
            <li> \b QT based / <b>Microsoft Visual Studio</b> project: <i>ETKSQLite3/build/qtETKSQLite3.vcproj</li>
        </ul>
      <ul>
        <li> Select needed target (Debug / Release / Static or Dynamic library).</li>
        <ul> <b>Code::Blocks</b>
            <li> Build ETKSQLite3. You need to update global variables into settings:</li>
              <ul>
                <li> <b>wx</b>:
                    <ul>
                        <li>base: wxWidgets library root folder.</li>
                        <li>lib: $(#wx)\\lib\\gcc_dll <i>(<a href="http://wiki.wxwidgets.org/Compiling_wxWidgets_with_MinGW">gcc compiler</a>)</i> / $(#wx)\\lib\\vc_dll <i>(for <a href="http://wiki.wxwidgets.org/Microsoft_Visual_C%2B%2B_Guide">
                                                                                                          Visual Studio compiler)</a></i></li>
                        <li>verlib: wxWidgets version (28 for \b wxWidgets 2.8.x, 29 for \b wxWidgets 2.8.x). <i>(User-defined fields)</i></li>
                        <li>verdll: wxWidgets version dll ((28 for wxWidgets 2.8.x, 292 for wxWidgets 2.9.2, 293 for wxWidgets 2.9.3). <i>(User-defined fields)</i></li>
                    </ul>
                </li>
                <li> <b>wxsqlite3</b>: wxSQLite3 library root folder.</li>
                <li> <b>wxetksqlite3</b>: ETKSQLite3 library root folder (for latter uses).</li>
          </ul>
        </li>
        <li>Your ETKSQLite3 is now compiled into <i>ETKSQLite3/lib</i> folder.</li>
    </ul>


\section Creating ETKSQLite3's project based.
    <ul>
      <li> Create an empty project file based on \b wxWidgets template.</li>
      <li> Into <b>Build Option</b> properties of the project:
        <ul>
          <li><b>Linker settings</b> tab:
            <ul>
              <li>In Debug mode, add \b libETKSQLite3d.a for dynamic link library, \b libStaticETKSQLite3d.a
                  for static link library.</li>
              <li>In Release mode, add \b libETKSQLite3.a for dynamic link library, \b libStaticETKSQLite3.a
                  for static link library.</li>
            </ul></li>
          <li><b>Search directories</b> tab:<br>
            For <b>all mode</b>, add:
            <ul>
              <li> \b Compiler
               <ul>
                  <li><b>$(\#wx)/include</b> for \b wxWidgets library includes (already done by Code::Blocks wizard).</li>
                  <li><b>$(\#wx)/contrib/include</b> for \b wxWidgets library includes (already done by Code::Blocks wizard).</li>
                  <li><b>$(\#wxsqlite3)/include</b> for \b wxSQLite3 library includes.</li>
                  <li><b>$(\#wxsqlite3)/sqlite3/include</b> for \b wxSQLite3 library includes.</li>
                  <li><b>$(\#wxetksqlite3)/include</b> for \b wxETKSQLite library includes.</li>
                </ul>
              </li>
              <li> \b Linker
               <ul>
                  <li><b>$(\#wxetksqlite3)/lib</b> for \b wxETKSQLite library link.</li>
                </ul>
              </li>
            </ul>
          </li>
        </ul>
      <li> Into <b>wx_pch.h</b> file (precompiler header):
        \code
        #ifdef WX_PRECOMP
            // put here all your rarely-changing header files
            // Include wxETKSQLITE3 : database managment library
            #define USING_WX_ETK_SQLITE3_LIB // <-- MUST be removed if compiling as dynamic library
            #include "ETKSQLite3Include.h"
            //
            // other includes (wxWidgets for example)
            //
            #endif // WX_PRECOMP
        #endif // WX_PRECOMP
        \endcode
        The following line \b MUST be removed if \b ETKSQLite3 is build as dynamic library.
        \code
            #define USING_WX_ETK_SQLITE3_LIB // <-- MUST be removed if compiling as dynamic library
        \endcode
        If the project could support dynamic and static library build, add <b>USING_WX_ETK_SQLITE3_LIB</b> into
        the project defines settings.
    </ul>
*/

/**
 * Class used to manage the wxSQLite3 database (sqlite3).
 *
 * This class contains the pointer on wxSQLite3Database class. It allow to use crypt or uncrypted
 * database.
 *
 * @author Stéphane Château
 * @version Name : ETKSQLite3<br>
 *          Revision : <b>1.1</b>
 */
class EXPORT_IMPORT ETKSQLite3Database : public wxSQLite3Hook
{
public:
    /**
     * Section name into the ini file of all informations about database.
     */
    static const etkString              STR_DATABASE_SECTION_NAME;

    /**
     * Key name for the path where the database must be opened.
     */
    static const etkString              STR_DATABASE_KEY_NAME_PATH;

    /**
     * Structure that contains informations about database events.
     *
     * Allocated each time an element is inserted, deleted or modified into database.
     * Implementing IRefCount to reduce memory fragmentation of shared pointer.
     *
     * @author Stéphane Château
     * @version Name : ETKSQLite3<br>
     *          Revision : <b>1.0</b>
     */
    struct EXPORT_IMPORT SDatabaseEvent : public ETKSQLite3::IRefCountImpl<SDatabaseEvent>
    {
        /**
         * Event type, could be SQLITE_DELETE, SQLITE_INSERT or SQLITE_UPDATE.
         */
        wxSQLite3Hook::wxUpdateType         m_UpdateType;

        /**
         * Name of the database.
         */
        etkString                           m_strDatabaseName;

        /**
         * Table name.
         */
        etkString                           m_strTableName;

        /**
         * Row identifier.
         */
        wxLongLong                          m_llRowID;

        /**
         * Get the update type as string (used to log for example).
         *
         * @return "DELETE", "INSERT" or "UPDATE" string depending of update type.
         */
        etkString                           GetUpdateTypeString() const;
    };

    /**
     * Shared pointer on database event structure.
     */
    typedef ETKSQLite3::shared_ptr<SDatabaseEvent> tdPtrDatabaseEvent;

private:
    /**
     * List of database event type.
     */
    typedef std::list<tdPtrDatabaseEvent>   tdLstDatabaseEvent;

    /**
     * Pointer on database managment.
     */
    wxSQLite3Database *                 m_pDatabase;

    /**
     * List of database events.
     *
     * Is filled when database events occurs, used when commit transaction or is dropped when rollback transaction.
     */
    tdLstDatabaseEvent                  m_lstDatabaseEvents;

    /**
     * Database SQL creation command.
     */
    etkString                           m_strSQLSchema;

    /**
     * Last error description.
     *
     * Is mutable because is set on const function call (LogDatabaseException).
     */
    mutable etkString                   m_strLastErrorMsg;

public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Default constructor.
     */
    ETKSQLite3Database();

    /**
     * Destructor.
     */
    virtual ~ETKSQLite3Database();
    //@}

    /**
     * Get the pointer on the real wxSQLite3 database.
     *
     * @return Pointer on database.
     */
    wxSQLite3Database *                 GetDatabase() const;

    /**
     * Get last error message.
     *
     * This error is recorded when LogDatabaseException function is called.
     *
     *  @return The error message description of last exception log by LogDatabaseException function call.
     */
    etkString                           GetLastErrorMessage() const;

    /**
     * Get the database file path
     *
     * The database file path is read from ini file, even the database is not opened
     *
     * @return The database file path to automatically load (or already loaded), wxFileName empty if no
     *         database is to be loaded.
     */
    wxFileName                          GetDatabaseFilePath() const;

    /**
     * Close the database if opened.
     */
    void                                Close();

    /**
     * Close the database if opened and clear database entry into the ini file.
     *
     * If the database is not opened, nothing is done.
     */
    void                                CloseAndClearIni();

    /**
     * Create a new database file.
     *
     * @param _strDatabaseFilePath Database file path to create.
     * @return true is the database is correctly created, false else.
     */
    bool                                Create(etkString _strDatabaseFilePath);

    /**
     * Open an existing database file.
     *
     * @param _strDatabaseFilePath Database file path to open. If empty string, the database file path is read from
     *                             current ini file.
     * @return true is the database is correctly opened, false else.
     */
    bool                                Open(etkString _strDatabaseFilePath = wxEmptyString);

    /**
     * Ask if the database is correctly opened or not.
     *
     * @return true if the database is open, false else.
     */
    bool                                IsOpened() const;

    /** @name Get the modifier/reader class instances.
     *
     * These functions are the only way to get instances of:
     * <ul>
     *  <li> <b>ETKSQLite3RequestInserter</b>: Used to add rows into table.
     *  <li> <b>ETKSQLite3RequestUpdater</b>: Used to modify rows into table.
     *  <li> <b></b>
     *  <li> <b></b>
     *  <li>
     * </ul>
     */
    //@{
    /**
     * Get an inserter request instance.
     *
     * @return A new instance of inserter.
     */
    ETKSQLite3RequestInserter         GetInserter();

    /**
     * Get an updater request instance.
     *
     * @return A new instance of updater.
     */
    ETKSQLite3RequestUpdater          GetUpdater();

    /**
     * Get an updater request instance.
     *
     * @return A new instance of updater.
     */
    ETKSQLite3RequestDeleter          GetDeleter();

    /**
     * Get an updater request instance.
     *
     * @return A new instance of updater.
     */
    ETKSQLite3RequestSelector          GetSelector();
    //@}

    /**
     * Set the SQL database creation structure.
     *
     * It is called only on database creation if no SQL schema is set.
     *
     * @return The SQL needed to construct the database.
     */
    void                                SetSQLDatabaseSchema(etkString _strSQLSchema);

    /**
     * Log Exception into log or/and a message box to the user.
     *
     * Can be ovveride by derived classes.
     *
     * @param _rException Catched database exception.
     * @param _bLogError If true, log database error.
     * @param _bDisplayMsgBox If true, display the error to the user.
     */
    virtual void                        LogDatabaseException(const wxSQLite3Exception &_rException, bool _bLogError, bool _bDisplayMsgBox) const;

protected:
    /**
     * Get the SQL database creation structure.
     *
     * It is called only on database creation if no SQL schema is set.
     * @return The SQL needed to construct the database.
     */
    virtual etkString                   GetSQLDatabaseSchema() const;

    /**
     * Create the encrypt key used to crypt database file.
     *
     * @return The encrypt key used to crypt the database, by default returns empty string.
     */
    virtual etkString                   GetEncryptKey() const;

    /**
     * Execute SQL request select with transaction using SQL.
     *
     * No transaction is created, only use this function to make select.
     *
     * @param _rstmQuery Statement used to execute SQL query (select).
     * @return The recordset of this request.
     */
    wxSQLite3ResultSet                  ExecuteQueryStatement(wxSQLite3Statement &_rstmQuery);

    /**
     * Execute SQL request with transaction using wxSQLite3Statement class.
     *
     * If the request fails while execution, the transaction is rollback. The throw is catched and error is logged.
     *
     * @param _rstmRequest Statement used to execute SQL request (update / delete / insert).
     * @param _bLogError If true, log database error.
     * @param _bDisplayMsgBox If true, display the error to the user.
     * @return The number of database rows that were changed (or inserted or deleted), 0 if error.
     */
    int                                 ExecuteStatementWithTransaction(wxSQLite3Statement &_rstmRequest, bool _bLogError, bool _bDisplayMsgBox);

    /**
     * Execute SQL request without transaction using wxSQLite3Statement class.
     *
     * If the request fails while execution, the transaction is not rollback. The throw is catched, error is logged
     * and displayed to the user and throw to calling function.
     * Calling this function should be done with transation  as:
     * <pre>
     *     try
     *     {   // Execute statement
     *         wxSQLite3Transaction transaction;
     *         // Preparing requests...
     *         // ...
     *         // Calling GetDatabase().ExecuteStatementWithoutTransaction(...parameters...)
     *         // Calling GetDatabase().ExecuteStatementWithoutTransaction(...parameters...)
     *         // Calling GetDatabase().ExecuteStatementWithoutTransaction(...parameters...)
     *         // ...
     *         transaction.Commit();
     *     }
     *     catch(wxSQLite3Exception &_ex)
     *     {   // Error while executing request to the database
     *         // log and display error to the user, choose parameters you want
     *         GetDatabase().LogDatabaseException(_ex, true (log error), true (display Message box));
     *     }
     * </pre>
     *
     * @param _rstmRequest Statement used to execute SQL request (update / delete / insert).
     * @param _bLogError If true, log database error.
     * @param _bDisplayMsgBox If true, display the error to the user.
     * @return The number of database rows that were changed (or inserted or deleted), 0 if error.
     * @throw wxSQLite3Exception If the function failed, a wxSQLite3Exception is raided so calling this function
     *                           must be protected by try / catch statement.
     */
    int                                 ExecuteStatementWithoutTransaction(wxSQLite3Statement &_rstmRequest, bool _bLogError, bool _bDisplayMsgBox);

    /**
     * Execute SQL request with transaction using SQL.
     *
     * If the request fails while execution, the transaction is rollback and an exception is raised.
     *
     * @param _strSQL SQL Update request.
     * @param _bLogError If true, log database error.
     * @param _bDisplayMsgBox If true, display the error to the user.
     * @return The number of database rows that were changed (or inserted or deleted).
     * @throw wxSQLite3Exception If the function failed, a wxSQLite3Exception is raided so calling this function
     *                           must be protected by try / catch statement.
     */
    int                                 ExecuteSQLWithTransaction(etkString _strSQL, bool _bLogError, bool _bDisplayMsgBox);

    /**
     * Execute SQL request with transaction using SQL.
     *
     * If the request fails while execution, the transaction is rollback and an exception is raised.
     *
     * @param _strSQL SQL Update request.
     * @param _bLogError If true, log database error.
     * @param _bDisplayMsgBox If true, display the error to the user.
     * @return The number of database rows that were changed (or inserted or deleted).
     * @throw wxSQLite3Exception If the function failed, a wxSQLite3Exception is raided so calling this function
     *                           must be protected by try / catch statement.
     */
    int                                 ExecuteSQLWithoutTransaction(etkString _strSQL, bool _bLogError, bool _bDisplayMsgBox);

private:
    /**
     * Prepare the statement to execute request.
     *
     * Used to make request binding. The database must be opened, else it raise a wxSQLite3Exception exception.
     *
     * @param _strStatementSQL SQL request.
     * @return The empty statement, must be bind.
     * @throw wxSQLite3Exception If the function failed, a wxSQLite3Exception is raided so calling this function
     * must be protected by try / catch statement.
     */
    wxSQLite3Statement                  PrepareEmptyStatement(etkString _strStatementSQL);

    /**
     * Execute SQL request with transaction or not, using wxSQLite3Statement class.
     *
     * If the request fails while execution, the transaction is rollback only when using transaction
     * or an exception is raised if not using transaction..
     *
     * @param _rstmRequest Statement request.
     * @param _bLogError If true, log database error.
     * @param _bDisplayMsgBox If true, display the error to the user.
     * @param _bUseTransaction If true, use transaction, else in case of error the exception is throw to parent.
     * @return The number of database rows that were changed (or inserted or deleted).
     * @throw wxSQLite3Exception If the function failed and don't use transaction, a wxSQLite3Exception is raided so calling this function
     *                           must be protected by try / catch statement.
     */
    int                                 ExecuteStatement(wxSQLite3Statement &_rstmRequest, bool _bLogError, bool _bDisplayMsgBox, bool _bUseTransaction);

    /**
     * Execute SQL request with transaction or not.
     *
     * If the request fails while execution, the transaction is rollback only when using transaction
     * or an exception is raised if not using transaction..
     *
     * @param _rstmRequest Statement request.
     * @param _bLogError If true, log database error.
     * @param _bDisplayMsgBox If true, display the error to the user.
     * @param _bUseTransaction If true, use transaction, else in case of error the exception is throw to parent.
     * @return The number of database rows that were changed (or inserted or deleted).
     * @throw wxSQLite3Exception If the function failed and don't use transaction, a wxSQLite3Exception is raided so calling this function
     *                           must be protected by try / catch statement.
     */
    int                                 ExecuteSQL(wxString _strSQL, bool _bLogError, bool _bDisplayMsgBox, bool _bUseTransaction);

protected:
    /**
     * Execute actions after created or opened database.
     *
     * This script enabling Foreign Key Support and must be called each time the database is
     * opened or created. It must be before creating structure ro adding element. This function is
     * the first function to call when the database is opened or create. The database may don't contains
     * the tables structures.
     * If this function is overwrite, don't forget to call this base class function to enable the Foreign Key Support (if needed).
     *
     * @throw wxSQLite3Exception If the function failed, a wxSQLite3Exception is raided so calling this function
     *                           must be protected by try / catch statement (already done by the ETKSQLite3Database class).
     */
    virtual void                        ExecuteConfigAfterOpenOrCreate();

    /**
     * Initialize callback when the database is ready to work.
     *
     * The callbacks will be used to notify views when elements are added or updated or deleted.
     * @return true if no error, false else: in this case the database is closed.
     *
     * @throw wxSQLite3Exception If the function failed, a wxSQLite3Exception is raided so calling this function
     *                           must be protected by try / catch statement.
     */
    virtual bool                        InitCallbacks();

    /**
     * Uninitialize callback when the database will be closed.
     *
     * The callbacks will be used to notify views when elements are added or updated or deleted.
     *
     * @throw wxSQLite3Exception If the function failed, a wxSQLite3Exception is raided so calling this function
     *                           must be protected by try / catch statement.
     */
    virtual void                        UnInitCallbacks();

    /**
     * Called by SQLite3 when row is add/inserted or modified.
     *
     * Record all update to notify the OnUpdateCallback when commit is done.<br>
     * <b>DO NOT OVERWRITE</b> this method to get database update, use OnUpdateCallback instead.
     *
     * @param _UpdateType The value signifies what kind of operation is to be done.
     * @param _strDatabaseName Name of the database.
     * @param _strTableName Name of the table.
     * @param _llRowID The rowid of the affected row.
     */
    void                                UpdateCallback(wxUpdateType _UpdateType, const wxString &_strDatabaseName, const wxString &_strTableName, wxLongLong _llRowID);

    /// Execute the commit hook callback function
    /**
    * Please refer to the SQLite documentation for further information.
    *
    * @return true to request rollback of the transaction, false to continue with commit
    */
    bool                                CommitCallback();

    /// Execute the rollback hook callback function
    /**
    * Please refer to the SQLite documentation for further information.
    */
    void                                RollbackCallback();

protected:
    /**
     * Alloc new SDatabaseEvent structure.
     *
     * Only make a new SDatabaseEvent but can be overloaded to let derived classes to allocate a
     * derived class of SDatabaseEvent.
     */
    tdPtrDatabaseEvent                  AllocNewDatabaseEvent();

    /**
     * Called by SQLite3 when row is add/inserted or modified.
     *
     * By default, does nothing.
     *
     * @param _pDatabaseEvent Contains a shared pointer to SDatabaseEvent that contains all informations
     *        about the database event.
     */
    virtual void                        OnUpdateCallback(tdPtrDatabaseEvent _pDatabaseEvent);

    friend class ETKSQLite3Request;
    friend class ETKSQLite3RequestUpdater;
    friend class ETKSQLite3RequestInserter;
    friend class ETKSQLite3RequestDeleter;
    friend class ETKSQLite3RequestSelector;
};

#endif // INCLUDE_ETK_SQLITE3_DATABASE_H
