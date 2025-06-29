/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3Database.h
// Library:     wxETKSQLite3
// Purpose:     Code for database managment
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/07/31
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "wxETKSQLite3Database.h"
#include <../sqlite3/include/sqlite3.h>
#include <wx/confbase.h>
#include <wx/file.h>

const wxString  wxETKSQLite3Database::STR_DATABASE_SECTION_NAME       = _T("/DATABASE");
const wxString  wxETKSQLite3Database::STR_DATABASE_KEY_NAME_PATH      = _T("Path");

// ----------------------------------------------------------------------------
// wxETKSQLite3Database::SDatabaseEvent
// ----------------------------------------------------------------------------
wxString wxETKSQLite3Database::SDatabaseEvent::GetUpdateTypeString() const
{
    wxString strUpdateType;

    switch(m_UpdateType)
    {
        case SQLITE_DELETE :
            strUpdateType = _T("DELETE");
            break;
        case SQLITE_INSERT :
            strUpdateType = _T("INSERT");
            break;
        case SQLITE_UPDATE :
            strUpdateType = _T("UPDATE");
            break;
        default :
            strUpdateType = _T("??????");
            break;
    }

    return strUpdateType;
}

// ----------------------------------------------------------------------------
// wxETKSQLite3Database
// ----------------------------------------------------------------------------
wxETKSQLite3Database::wxETKSQLite3Database()
{
    m_pDatabase = NULL;
}

wxETKSQLite3Database::~wxETKSQLite3Database()
{
    Close();
}

wxSQLite3Database * wxETKSQLite3Database::GetDatabase() const
{
    return m_pDatabase;
}

wxString wxETKSQLite3Database::GetLastErrorMessage() const
{
    return m_strLastErrorMsg;
}

wxFileName wxETKSQLite3Database::GetDatabaseFilePath() const
{
    wxConfigBase::Get()->SetPath(STR_DATABASE_SECTION_NAME);
    return wxFileName(wxConfigBase::Get()->Read(STR_DATABASE_KEY_NAME_PATH,_T("")));
}

bool wxETKSQLite3Database::Create(wxString _strDatabaseFilePath)
{
    bool bRet = false;
    Close();

    try
    {   // Database object creation
        m_pDatabase = new wxSQLite3Database();
        // Database file creation
        m_pDatabase->Open(_strDatabaseFilePath,GetEncryptKey(),WXSQLITE_OPEN_READWRITE | WXSQLITE_OPEN_CREATE);
        ExecuteConfigAfterOpenOrCreate();
        // Database structure creation
        ExecuteSQLWithTransaction(m_strSQLSchema.IsEmpty() ? GetSQLDatabaseSchema().wx_str() : m_strSQLSchema.wx_str(),true,true);
        // Update ini file to automatically load this database file as startup
        wxConfigBase::Get()->SetPath(STR_DATABASE_SECTION_NAME);
        wxConfigBase::Get()->Write(STR_DATABASE_KEY_NAME_PATH,_strDatabaseFilePath);
        InitCallbacks();
        wxLogVerbose(wxT("Created SQLite3 database file: %s"),_strDatabaseFilePath.c_str());
        bRet = true; // (no exception raised)
    }
    catch(wxSQLite3Exception &_ex)
    {   // Error while creating the database
        LogDatabaseException(_ex,true,true); // log and display error to the user
        bRet = false;
    }

    if (!bRet)
    {   // Error, the database is not created
        if (m_pDatabase != NULL)
        {
            if (IsOpened())
            {   // Not only created, must be opened
                try
                {
                    UnInitCallbacks();
                }
                catch (wxSQLite3Exception &_ex)
                {
                    LogDatabaseException(_ex,true,true); // log and display error to the user
                }
            }

            wxDELETE(m_pDatabase); // Not opened !
        }
        if (wxFileExists(_strDatabaseFilePath))
        {   // Delete created file because it's failed
            ::wxRemoveFile(_strDatabaseFilePath);
        }
    }
    return bRet;
}

bool wxETKSQLite3Database::Open(wxString _strDatabaseFilePath)
{
    bool bRet = false;
    Close();

    wxString strDatabaseFilePath(_strDatabaseFilePath);

    // Try open the database : look into ini where the database is recorded
    if (strDatabaseFilePath.IsEmpty())
    {
        strDatabaseFilePath = GetDatabaseFilePath().GetFullPath();
    }

    if (!strDatabaseFilePath.IsEmpty())
    {   // Information exists into the ini file ...
        if (wxFile::Exists(strDatabaseFilePath))
        {   // ... and the file exists : open it and don't create it if not exists
            try
            {
                m_pDatabase = new wxSQLite3Database();
                m_pDatabase->Open(strDatabaseFilePath,GetEncryptKey(),WXSQLITE_OPEN_READWRITE);
                // Update ini file to automatically load this database file as startup
                wxConfigBase::Get()->SetPath(STR_DATABASE_SECTION_NAME);
                wxConfigBase::Get()->Write(STR_DATABASE_KEY_NAME_PATH,strDatabaseFilePath);
                ExecuteConfigAfterOpenOrCreate();
                InitCallbacks();
                bRet = true; // No exception raised
            }
            catch(wxSQLite3Exception &_ex)
            {   // Error while open the database
                LogDatabaseException(_ex,true,true); // log and display error to the user
                if (m_pDatabase != NULL)
                {
                    try
                    {
                        UnInitCallbacks();
                    }
                    catch (wxSQLite3Exception &_ex)
                    {
                        LogDatabaseException(_ex,true,true); // log and display error to the user
                    }
                }
                wxDELETE(m_pDatabase); // Not opened !
            }
        }
        else
        {   // Path exist into the ini file but the file doesn't exists.
            wxString strError;

            strError.Printf(_("Error, database \"%s\" doesn't exists!\n\nCannot open database!"),strDatabaseFilePath.wx_str());
            wxMessageDialog msgDlg(NULL,strError,_("Database file is missing"),wxOK | wxICON_ERROR);
            wxLogError(strError);
            msgDlg.ShowModal();
        }
    }
    else
    {
        wxLogVerbose(wxT("Database path not filled into ini file, waiting to open database."));
    }

    return bRet;
}

void wxETKSQLite3Database::CloseAndClearIni()
{
    if (IsOpened())
    {
        Close();
        // Update ini file to automatically don't load this any database file as startup
        wxConfigBase::Get()->SetPath(STR_DATABASE_SECTION_NAME);
        wxConfigBase::Get()->Write(STR_DATABASE_KEY_NAME_PATH,wxEmptyString);
        wxLogVerbose(wxT("Closed current opened SQLite3 database file and clear ini file from automatically loading as startup."));
    }
}

void wxETKSQLite3Database::Close()
{
    if (m_pDatabase != NULL)
    {
        try
        {
            UnInitCallbacks();
        }
        catch (wxSQLite3Exception &_ex)
        {
            LogDatabaseException(_ex,true,true); // log and display error to the user
        }
        wxDELETE(m_pDatabase);
    }
}

bool wxETKSQLite3Database::IsOpened() const
{   // The database is opened when the pointer to internal wxSQLite3 database is not null.
    return m_pDatabase != NULL && m_pDatabase->IsOpen();
}

wxETKSQLite3RequestInserter wxETKSQLite3Database::GetInserter()
{
    return wxETKSQLite3RequestInserter(*this);
}

wxETKSQLite3RequestUpdater wxETKSQLite3Database::GetUpdater()
{
    return wxETKSQLite3RequestUpdater(*this);
}

wxETKSQLite3RequestDeleter wxETKSQLite3Database::GetDeleter()
{
    return wxETKSQLite3RequestDeleter(*this);
}

wxETKSQLite3RequestSelector wxETKSQLite3Database::GetSelector()
{
    return wxETKSQLite3RequestSelector(*this);
}

wxString wxETKSQLite3Database::GetSQLDatabaseSchema() const
{
    wxFAIL_MSG(wxT("Empty database SQL creation command"));
    return wxEmptyString;
}

void wxETKSQLite3Database::SetSQLDatabaseSchema(wxString _strSQLSchema)
{
    m_strSQLSchema = _strSQLSchema;
}

void wxETKSQLite3Database::LogDatabaseException(const wxSQLite3Exception &_rException,bool _bLogError,bool _bDisplayMsgBox) const
{
    wxString strError;

    m_strLastErrorMsg = _rException.GetMessage();
    strError.Printf(_("Database error: %s"),_rException.GetMessage().c_str());

    if (_bLogError)
    {   // log into logger
        wxLogError(strError);
    }

    if (_bDisplayMsgBox)
    {   // Display error to the user
        wxMessageDialog msgDlg(NULL,strError,_("Database error"),wxOK | wxICON_ERROR);
        msgDlg.ShowModal();
    }
}

wxString wxETKSQLite3Database::GetEncryptKey() const
{
    return wxEmptyString;
}

wxSQLite3Statement wxETKSQLite3Database::PrepareEmptyStatement(wxString _strStatementSQL) throw(wxSQLite3Exception)
{
    if (IsOpened())
    {
        return m_pDatabase->PrepareStatement(_strStatementSQL);
    }
    else
    {
        throw  wxSQLite3Exception(WXSQLITE_ERROR, wxT("Cannot prepare request on closed database!")); // Use wxSQLite3Exception Exception !
    }
}

wxSQLite3ResultSet wxETKSQLite3Database::ExecuteQueryStatement(wxSQLite3Statement &_rstmQuery)
{
    wxSQLite3ResultSet resultSet;

    if (IsOpened())
    {
        try
        {
#ifndef SQLITE_VERSION_NUMBER
    #error Error, must include sqlite3.h to define SQLITE_VERSION_NUMBER version number
#endif
#if SQLITE_VERSION_NUMBER <= 3007007
            resultSet = _rstmQuery.ExecuteQuery(true); // Transfert owner to ResultSet
#else
            resultSet = _rstmQuery.ExecuteQuery();
#endif
        }
        catch(wxSQLite3Exception &_ex)
        {   // Error while open the database
            LogDatabaseException(_ex,true,true); // log and display error to the user
        }
    }

    return resultSet;
}

int wxETKSQLite3Database::ExecuteStatementWithTransaction(wxSQLite3Statement &_rstmRequest,bool _bLogError,bool _bDisplayMsgBox)
{
    return ExecuteStatement(_rstmRequest,_bLogError,_bDisplayMsgBox,true);
}

int wxETKSQLite3Database::ExecuteStatementWithoutTransaction(wxSQLite3Statement &_rstmRequest,bool _bLogError,bool _bDisplayMsgBox) throw(wxSQLite3Exception)
{
    return ExecuteStatement(_rstmRequest,_bLogError,_bDisplayMsgBox,false);
}

int wxETKSQLite3Database::ExecuteSQLWithTransaction(wxString _strSQL,bool _bLogError,bool _bDisplayMsgBox) throw(wxSQLite3Exception)
{
    return ExecuteSQL(_strSQL,_bLogError,_bDisplayMsgBox,true);
}

int wxETKSQLite3Database::ExecuteSQLWithoutTransaction(wxString _strSQL,bool _bLogError,bool _bDisplayMsgBox) throw(wxSQLite3Exception)
{
    return ExecuteSQL(_strSQL,_bLogError,_bDisplayMsgBox,false);
}

int wxETKSQLite3Database::ExecuteStatement(wxSQLite3Statement &_rstmRequest,bool _bLogError,bool _bDisplayMsgBox,bool _bUseTransaction) throw(wxSQLite3Exception)
{
    int iNbRowAffected = 0;

    try
    {   // Execute statement
        if (IsOpened())
        {
            if (_rstmRequest.IsOk())
            {
                if (_bUseTransaction)
                {
                    wxSQLite3Transaction transaction(m_pDatabase);
                    iNbRowAffected = _rstmRequest.ExecuteUpdate();
                    transaction.Commit();
                }
                else
                {
                    iNbRowAffected = _rstmRequest.ExecuteUpdate();
                }
            }
            else
            {
                wxString strError(wxString::Format(wxT("Invalid request (%s)!"),_rstmRequest.GetSQL().wx_str()));
                wxLogError(strError);
                wxFAIL_MSG(strError);
            }
        }
        else
        {
            wxString strError(wxT("Cannot execute request on closed database!"));
            wxLogError(strError);
            wxFAIL_MSG(strError);
        }
    }
    catch(wxSQLite3Exception &_ex)
    {   // Error while open the database
        LogDatabaseException(_ex,_bLogError,_bDisplayMsgBox); // log and display error to the user
        if (!_bUseTransaction)
        {   // Throw exception to calling function
            throw;
        }
    }

    return iNbRowAffected;
}

int wxETKSQLite3Database::ExecuteSQL(wxString _strSQL,bool _bLogError,bool _bDisplayMsgBox,bool _bUseTransaction) throw(wxSQLite3Exception)
{
    int iNbRowAffected = 0;

    try
    {   // Execute statement
        if (IsOpened())
        {
            if (_bUseTransaction)
            {
                wxSQLite3Transaction transaction(m_pDatabase);
                iNbRowAffected = m_pDatabase->ExecuteUpdate(_strSQL);
                transaction.Commit();
            }
            else
            {
                iNbRowAffected = m_pDatabase->ExecuteUpdate(_strSQL);
            }
        }
        else
        {
            wxString strError(wxT("Cannot execute request on closed database!"));
            wxLogError(strError);
            wxFAIL_MSG(strError);
        }
    }
    catch(wxSQLite3Exception &_ex)
    {   // Error while open the database
        LogDatabaseException(_ex,_bLogError,_bDisplayMsgBox); // log and display error to the user
        if (!_bUseTransaction)
        {   // Throw exception to calling function
            throw;
        }
    }

    return iNbRowAffected;
}

void wxETKSQLite3Database::ExecuteConfigAfterOpenOrCreate() throw(wxSQLite3Exception)
{
    wxASSERT_MSG(IsOpened(),wxT("Database is not opened!"));
    if (IsOpened())
    {   // Activate foreign keys update/delete cascade
        if (!m_pDatabase->IsForeignKeySupportEnabled())
        {
            m_pDatabase->EnableForeignKeySupport(true);
        }
    }
}

bool wxETKSQLite3Database::InitCallbacks() throw(wxSQLite3Exception)
{
    wxASSERT_MSG(IsOpened(),wxT("Database is not opened!"));
   if (IsOpened())
    {
        // Initialize all callbacks
        wxSQLite3Hook::SetDatabase(m_pDatabase);
        m_pDatabase->SetUpdateHook(this);
        m_pDatabase->SetRollbackHook(this);
        m_pDatabase->SetCommitHook(this);

        return true;
    }

    return false;
}

void wxETKSQLite3Database::UnInitCallbacks() throw(wxSQLite3Exception)
{
    wxASSERT_MSG(IsOpened(),wxT("Database is not opened!"));
    if (IsOpened())
    {
        // Initialize all callbacks
        m_pDatabase->SetUpdateHook(NULL);
        m_pDatabase->SetRollbackHook(NULL);
        m_pDatabase->SetCommitHook(NULL);
        wxSQLite3Hook::SetDatabase(NULL);
    }
}

void wxETKSQLite3Database::UpdateCallback(wxUpdateType _UpdateType,const wxString &_strDatabaseName,const wxString &_strTableName,wxLongLong _llRowID)
{
    tdPtrDatabaseEvent pEvent = AllocNewDatabaseEvent();

    pEvent->m_UpdateType      = _UpdateType;
    pEvent->m_llRowID         = _llRowID;
    pEvent->m_strDatabaseName = _strDatabaseName;
    pEvent->m_strTableName    = _strTableName;

    m_lstDatabaseEvents.push_back(pEvent);
}

bool wxETKSQLite3Database::CommitCallback()
{
    static bool _bReentre = false;

    if (!_bReentre)
    {
        _bReentre = true;
        while (!m_lstDatabaseEvents.empty())
        {   // Don't need to delete object, shared pointer make it
            tdPtrDatabaseEvent pFrontElement = m_lstDatabaseEvents.front();
            m_lstDatabaseEvents.pop_front();
            OnUpdateCallback(pFrontElement);  // Notify this database update
        }
        _bReentre = false;
    }
    return false;
}

void wxETKSQLite3Database::RollbackCallback()
{
    m_lstDatabaseEvents.clear(); // Discard database event list.
}

wxETKSQLite3Database::tdPtrDatabaseEvent wxETKSQLite3Database::AllocNewDatabaseEvent()
{
    return new SDatabaseEvent;
}

void wxETKSQLite3Database::OnUpdateCallback(tdPtrDatabaseEvent _pDatabaseEvent)
{
    // Does nothing
}
