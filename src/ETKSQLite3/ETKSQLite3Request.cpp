/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3Request.cpp
// Library:     ETKSQLite3
// Purpose:     Code for request managment (base class)
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/04
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "ETKSQLite3Request.h"
#include "ETKSQLite3Database.h"

// ----------------------------------------------------------------------------
// ETKSQLite3Request
// ----------------------------------------------------------------------------

ETKSQLite3Request::ETKSQLite3Request(ETKSQLite3Database & _rDatabase)
    : m_rDatabase(_rDatabase)
    , m_llLastRowID(0)
    , m_criterionRequest(ETKSQLite3Criterion::eRequestTypeUndefined)
    , m_pStatement(NULL)
{
}

ETKSQLite3Request::ETKSQLite3Request(const ETKSQLite3Request & _rOther)
    : m_rDatabase(_rOther.m_rDatabase)
    , m_llLastRowID(_rOther.m_llLastRowID)
    , m_criterionRequest(_rOther.m_criterionRequest)
    , m_pStatement(NULL)
{
}

ETKSQLite3Request::~ETKSQLite3Request()
{
    DeleteStatement();
}

wxLongLong ETKSQLite3Request::GetLastRowId() const
{
    return m_llLastRowID;
}

ETKSQLite3Database & ETKSQLite3Request::GetDatabase() const
{
    return m_rDatabase;
}

ETKSQLite3Criterion & ETKSQLite3Request::GetCriterionRequest()
{
    return m_criterionRequest;
}

const ETKSQLite3Criterion & ETKSQLite3Request::GetCriterionRequest() const
{
    return m_criterionRequest;
}

void ETKSQLite3Request::Where(const ETKSQLite3Expression &_rExprWhere)
{
    wxASSERT(!_rExprWhere.IsJoin());
    GetCriterionRequest().SetWhere(_rExprWhere);
}

const ETKSQLite3Expression & ETKSQLite3Request::GetWhere()
{
    return GetCriterionRequest().GetWhere();
}

void ETKSQLite3Request::Join(const ETKSQLite3Expression &_rExprJoin)
{
    wxASSERT(_rExprJoin.IsJoin());
    GetCriterionRequest().SetJoin(_rExprJoin);
}

void ETKSQLite3Request::Clear()
{
    m_llLastRowID = 0;
    m_criterionRequest.Clear();
    DeleteStatement();
}

bool ETKSQLite3Request::InitStatement(wxString _strSQLRequest)
{
    bool bRet = false;

    // Delete previous statement if exists
    DeleteStatement();
    try
    {   // Create empty statement
        m_pStatement = new wxSQLite3Statement(GetDatabase().PrepareEmptyStatement(_strSQLRequest));
        bRet = true; // No exception has been raised
    }
    catch(wxSQLite3Exception &_ex)
    {   // Error while open the database
        GetDatabase().LogDatabaseException(_ex,true,true); // log and display error to the user
        wxFAIL_MSG(wxString::Format(wxT("ETKSQLite3Request::InitStatement function raise exception (%s)!"),
                                    _ex.GetMessage().wx_str()));
        DeleteStatement();
    }

    return bRet;
}

bool ETKSQLite3Request::PrepareStatement()
{
    bool bRet = false;
    bool bIsStatementInitialized = IsStatementInitialized();
    if (!bIsStatementInitialized)
    {   // Get the SQL request
        wxString strSQLRequest  = GetCriterionRequest().GetSQL();
        wxLogDebug(wxT("[SQL] %s"),strSQLRequest.wx_str());

        // Init wxSQLite3Statement class
        bIsStatementInitialized = InitStatement(strSQLRequest);

        if (bIsStatementInitialized)
        {
            if (m_pStatement->IsOk())
            {   // Binds are needed
                bRet = Bind(*m_pStatement);
            }
        }

        if (!bRet)
        {   // If error, delete the statement to retry later
            DeleteStatement();
        }
    }
    else
    {
        m_pStatement->Reset();      // Allow to bind new datas
        bRet = Bind(*m_pStatement); // Binds new datas
    }

    return bRet;
}

bool ETKSQLite3Request::IsStatementInitialized() const
{
    return m_pStatement!=NULL && m_pStatement->IsOk();
}

void ETKSQLite3Request::DeleteStatement()
{
    wxDELETE(m_pStatement);
}

wxSQLite3Statement & ETKSQLite3Request::GetStatement()
{
    wxASSERT_MSG(IsStatementInitialized(),wxT("To get statement, it must be initialized!"));
    return *m_pStatement;
}

bool ETKSQLite3Request::Bind(wxSQLite3Statement &_rStatement)
{
    bool bRet = false;

    int iIndex = 1;

    try
    {   // Make the bind
        GetCriterionRequest().BindTo(_rStatement,iIndex);
        bRet = _rStatement.IsOk();
    }
    catch(wxSQLite3Exception &_ex)
    {   // Error while open the database
        GetDatabase().LogDatabaseException(_ex,true,true); // log and display error to the user
        wxFAIL_MSG(wxT("ETKSQLite3Request::Bind function raise exception!"));
    }

    return bRet;
}

int ETKSQLite3Request::ExecuteWithTransaction(bool _bLogError, bool _bDisplayMsgBox)
{
    return Execute(_bLogError,_bDisplayMsgBox,true);
}

int ETKSQLite3Request::ExecuteWithoutTransaction(bool _bLogError, bool _bDisplayMsgBox)
{
    return Execute(_bLogError,_bDisplayMsgBox,false);
}

int ETKSQLite3Request::Execute(bool _bLogError, bool _bDisplayMsgBox, bool _bWithTransaction)
{
    int iNbRows   = -1;
    m_llLastRowID = 0;

    if (PrepareStatement())
    {   // Statement is ready, execute it
        iNbRows = GetDatabase().ExecuteStatement(*m_pStatement,_bLogError, _bDisplayMsgBox, _bWithTransaction);
        // Record last row ID
        m_llLastRowID = GetDatabase().GetDatabase()->GetLastRowId();
    }

    return iNbRows;
}
