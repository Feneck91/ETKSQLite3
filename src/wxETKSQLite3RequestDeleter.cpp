/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3RequestDeleter.cpp
// Library:     wxETKSQLite3
// Purpose:     Code to make delete into database
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/09
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "wxETKSQLite3RequestDeleter.h"
#include "wxETKSQLite3Database.h"
#include "wxETKSQLite3ResultSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxETKSQLite3RequestDeleter::wxETKSQLite3RequestDeleter(wxETKSQLite3Database & _rDatabase)
    : wxETKSQLite3Request(_rDatabase)
{
    // Set the correct type for the request criterion
    GetCriterionRequest().SetRequestType(wxETKSQLite3Criterion::eRequestTypeDelete);
}

wxETKSQLite3RequestDeleter::wxETKSQLite3RequestDeleter(const wxETKSQLite3RequestDeleter & _rRequestDeleter)
    : wxETKSQLite3Request(_rRequestDeleter)
{
}

wxETKSQLite3RequestDeleter::~wxETKSQLite3RequestDeleter()
{
}

void wxETKSQLite3RequestDeleter::operator<<(const wxETKSQLite3Record &_rRecord)
{
    operator<<(_rRecord.GetTableName());
}

void wxETKSQLite3RequestDeleter::operator<<(wxString _strTableName)
{   // Mandatory to create table name as eExpressionColumnNameFull to let expression to
    // extract table name
    GetCriterionRequest() = wxETKSQLite3Expression(wxETKSQLite3Value(_strTableName + _T(".x"),wxETKSQLite3Value::eExpressionColumnNameFull));
}
