/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3RequestDeleter.cpp
// Library:     ETKSQLite3
// Purpose:     Code to make delete into database
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/09
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "ETKSQLite3RequestDeleter.h"
#include "ETKSQLite3Database.h"
#include "ETKSQLite3ResultSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ETKSQLite3RequestDeleter::ETKSQLite3RequestDeleter(ETKSQLite3Database & _rDatabase)
    : ETKSQLite3Request(_rDatabase)
{
    // Set the correct type for the request criterion
    GetCriterionRequest().SetRequestType(ETKSQLite3Criterion::eRequestTypeDelete);
}

ETKSQLite3RequestDeleter::ETKSQLite3RequestDeleter(const ETKSQLite3RequestDeleter & _rRequestDeleter)
    : ETKSQLite3Request(_rRequestDeleter)
{
}

ETKSQLite3RequestDeleter::~ETKSQLite3RequestDeleter()
{
}

void ETKSQLite3RequestDeleter::operator<<(const ETKSQLite3Record &_rRecord)
{
    operator<<(_rRecord.GetTableName());
}

void ETKSQLite3RequestDeleter::operator<<(wxString _strTableName)
{   // Mandatory to create table name as eExpressionColumnNameFull to let expression to
    // extract table name
    GetCriterionRequest() = ETKSQLite3Expression(ETKSQLite3Value(_strTableName + _T(".x"),ETKSQLite3Value::eExpressionColumnNameFull));
}
