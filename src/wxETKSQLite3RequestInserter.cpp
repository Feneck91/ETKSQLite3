/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3RequestInserter.cpp
// Library:     wxETKSQLite3
// Purpose:     Code for inserter sql managment
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/04
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "wxETKSQLite3RequestInserter.h"
#include "wxETKSQLite3Database.h"
#include "wxETKSQLite3ResultSet.h"
#include "wxETKSQLite3Record.h"

#include <algorithm>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
wxETKSQLite3RequestInserter::wxETKSQLite3RequestInserter(wxETKSQLite3Database & _rDatabase)
    : wxETKSQLite3RequestUpdater(_rDatabase)
{
    // Set the correct type for the request criterion
    GetCriterionRequest().SetRequestType(wxETKSQLite3Criterion::eRequestTypeInsert);
}

wxETKSQLite3RequestInserter::wxETKSQLite3RequestInserter(const wxETKSQLite3RequestInserter & _rRequestInserter)
    : wxETKSQLite3RequestUpdater(_rRequestInserter)
{
}

wxETKSQLite3RequestInserter::~wxETKSQLite3RequestInserter()
{
}

wxETKSQLite3Expression::eExpressionOrOperationType wxETKSQLite3RequestInserter::GetOperationAssigment() const
{
    return wxETKSQLite3Expression::eOperationAssignmentInsert;
}
