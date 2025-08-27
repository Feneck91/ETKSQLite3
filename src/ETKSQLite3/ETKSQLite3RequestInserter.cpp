/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3RequestInserter.cpp
// Library:     ETKSQLite3
// Purpose:     Code for inserter sql managment
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/04
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "ETKSQLite3RequestInserter.h"
#include "ETKSQLite3Database.h"
#include "ETKSQLite3ResultSet.h"
#include "ETKSQLite3Record.h"

#include <algorithm>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ETKSQLite3RequestInserter::ETKSQLite3RequestInserter(ETKSQLite3Database & _rDatabase)
    : ETKSQLite3RequestUpdater(_rDatabase)
{
    // Set the correct type for the request criterion
    GetCriterionRequest().SetRequestType(ETKSQLite3Criterion::eRequestTypeInsert);
}

ETKSQLite3RequestInserter::ETKSQLite3RequestInserter(const ETKSQLite3RequestInserter & _rRequestInserter)
    : ETKSQLite3RequestUpdater(_rRequestInserter)
{
}

ETKSQLite3RequestInserter::~ETKSQLite3RequestInserter()
{
}

ETKSQLite3Expression::eExpressionOrOperationType ETKSQLite3RequestInserter::GetOperationAssigment() const
{
    return ETKSQLite3Expression::eOperationAssignmentInsert;
}
