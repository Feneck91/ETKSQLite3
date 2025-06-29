/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3Request.cpp
// Library:     wxETKSQLite3
// Purpose:     Code for request managment (base class)
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/04
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "wxETKSQLite3Request.h"
#include "wxETKSQLite3Database.h"

// ----------------------------------------------------------------------------
// wxETKSQLite3Request
// ----------------------------------------------------------------------------

wxETKSQLite3Request::wxETKSQLite3Request(wxETKSQLite3Database & _rDatabase)
    : m_rDatabase(_rDatabase)
    , m_llLastRowID(0)
    , m_criterionRequest(wxETKSQLite3Criterion::eRequestTypeUndefined)
    , m_pStatement(NULL)
{
}

wxETKSQLite3Request::wxETKSQLite3Request(const wxETKSQLite3Request & _rOther)
    : m_rDatabase(_rOther.m_rDatabase)
    , m_llLastRowID(_rOther.m_llLastRowID)
    , m_criterionRequest(_rOther.m_criterionRequest)
    , m_pStatement(NULL)
{
}

wxETKSQLite3Request::~wxETKSQLite3Request()
{
    DeleteStatement();
}

wxLongLong wxETKSQLite3Request::GetLastRowId() const
{
    return m_llLastRowID;
}

wxETKSQLite3Database & wxETKSQLite3Request::GetDatabase() const
{
    return m_rDatabase;
}

wxETKSQLite3Criterion & wxETKSQLite3Request::GetCriterionRequest()
{
    return m_criterionRequest;
}

const wxETKSQLite3Criterion & wxETKSQLite3Request::GetCriterionRequest() const
{
    return m_criterionRequest;
}

void wxETKSQLite3Request::Where(const wxETKSQLite3Expression &_rExprWhere)
{
    wxASSERT(!_rExprWhere.IsJoin());
    GetCriterionRequest().SetWhere(_rExprWhere);
}

const wxETKSQLite3Expression & wxETKSQLite3Request::GetWhere()
{
    return GetCriterionRequest().GetWhere();
}

void wxETKSQLite3Request::Join(const wxETKSQLite3Expression &_rExprJoin)
{
    wxASSERT(_rExprJoin.IsJoin());
    GetCriterionRequest().SetJoin(_rExprJoin);
}

void wxETKSQLite3Request::Clear()
{
    m_llLastRowID = 0;
    m_criterionRequest.Clear();
    DeleteStatement();
}

bool wxETKSQLite3Request::InitStatement(wxString _strSQLRequest)
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
        wxFAIL_MSG(wxString::Format(wxT("wxETKSQLite3Request::InitStatement function raise exception (%s)!"),
                                    _ex.GetMessage().wx_str()));
        DeleteStatement();
    }

    return bRet;
}

bool wxETKSQLite3Request::PrepareStatement()
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

bool wxETKSQLite3Request::IsStatementInitialized() const
{
    return m_pStatement!=NULL && m_pStatement->IsOk();
}

void wxETKSQLite3Request::DeleteStatement()
{
    wxDELETE(m_pStatement);
}

wxSQLite3Statement & wxETKSQLite3Request::GetStatement()
{
    wxASSERT_MSG(IsStatementInitialized(),wxT("To get statement, it must be initialized!"));
    return *m_pStatement;
}

bool wxETKSQLite3Request::Bind(wxSQLite3Statement &_rStatement)
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
        wxFAIL_MSG(wxT("wxETKSQLite3Request::Bind function raise exception!"));
    }

    return bRet;
}

int wxETKSQLite3Request::ExecuteWithTransaction(bool _bLogError,bool _bDisplayMsgBox)
{
    return Execute(_bLogError,_bDisplayMsgBox,true);
}

int wxETKSQLite3Request::ExecuteWithoutTransaction(bool _bLogError,bool _bDisplayMsgBox) throw(wxSQLite3Exception)
{
    return Execute(_bLogError,_bDisplayMsgBox,false);
}

int wxETKSQLite3Request::Execute(bool _bLogError,bool _bDisplayMsgBox,bool _bWithTransaction) throw(wxSQLite3Exception)
{
    int iNbRows   = -1;
    m_llLastRowID = 0;

    if (PrepareStatement())
    {   // Statement is ready, execute it
        iNbRows = GetDatabase().ExecuteStatement(*m_pStatement,_bLogError,_bDisplayMsgBox,_bWithTransaction);
        // Record last row ID
        m_llLastRowID = GetDatabase().GetDatabase()->GetLastRowId();
    }

    return iNbRows;
}
