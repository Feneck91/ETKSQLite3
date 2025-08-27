/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3RequestUpdater.cpp
// Library:     ETKSQLite3
// Purpose:     Code to make update into database
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/04
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "ETKSQLite3RequestUpdater.h"
#include "ETKSQLite3Database.h"
#include "ETKSQLite3ResultSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ETKSQLite3RequestUpdater::ETKSQLite3RequestUpdater(ETKSQLite3Database & _rDatabase)
    : ETKSQLite3Request(_rDatabase)
{
    // Set the correct type for the request criterion
    GetCriterionRequest().SetRequestType(ETKSQLite3Criterion::eRequestTypeUpdate);
}

ETKSQLite3RequestUpdater::ETKSQLite3RequestUpdater(const ETKSQLite3RequestUpdater & _rRequestUpdater)
    : ETKSQLite3Request(_rRequestUpdater)
{
}

ETKSQLite3RequestUpdater::~ETKSQLite3RequestUpdater()
{
}

wxLongLong ETKSQLite3RequestUpdater::GetLastRowId() const
{
    return ETKSQLite3Request::GetLastRowId();
}

void ETKSQLite3RequestUpdater::operator<<(const ETKSQLite3Record &_rRecord)
{
    for (size_t stIndex = 0;stIndex < _rRecord.GetColumnCount();++stIndex)
    {
        operator<<(_rRecord.GetColumn(stIndex));
    }
}

ETKSQLite3RequestUpdater & ETKSQLite3RequestUpdater::operator<<(const ETKSQLite3Column &_rColumn)
{
    GetCriterionRequest().Add(ETKSQLite3Expression(GetOperationAssigment(),
                                                   ETKSQLite3Expression(_rColumn,
                                                                        ETKSQLite3Expression::eExpressionColumnNameOnly
                                                                       ),
                                                   ETKSQLite3Expression(_rColumn.GetColumnAttributes())
                                                  )
                             );
    return *this;
}

ETKSQLite3RequestUpdater & ETKSQLite3RequestUpdater::operator<<(const ETKSQLite3Expression &_rExpression)
{
    GetCriterionRequest().Add(_rExpression);
    return *this;
}

ETKSQLite3Expression::eExpressionOrOperationType ETKSQLite3RequestUpdater::GetOperationAssigment() const
{
    return ETKSQLite3Expression::eOperationAssignmentUpdate;
}
