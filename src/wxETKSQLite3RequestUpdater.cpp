/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3RequestUpdater.cpp
// Library:     wxETKSQLite3
// Purpose:     Code to make update into database
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/04
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "wxETKSQLite3RequestUpdater.h"
#include "wxETKSQLite3Database.h"
#include "wxETKSQLite3ResultSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxETKSQLite3RequestUpdater::wxETKSQLite3RequestUpdater(wxETKSQLite3Database & _rDatabase)
    : wxETKSQLite3Request(_rDatabase)
{
    // Set the correct type for the request criterion
    GetCriterionRequest().SetRequestType(wxETKSQLite3Criterion::eRequestTypeUpdate);
}

wxETKSQLite3RequestUpdater::wxETKSQLite3RequestUpdater(const wxETKSQLite3RequestUpdater & _rRequestUpdater)
    : wxETKSQLite3Request(_rRequestUpdater)
{
}

wxETKSQLite3RequestUpdater::~wxETKSQLite3RequestUpdater()
{
}

wxLongLong wxETKSQLite3RequestUpdater::GetLastRowId() const
{
    return wxETKSQLite3Request::GetLastRowId();
}

void wxETKSQLite3RequestUpdater::operator<<(const wxETKSQLite3Record &_rRecord)
{
    for (size_t stIndex = 0;stIndex < _rRecord.GetColumnCount();++stIndex)
    {
        operator<<(_rRecord.GetColumn(stIndex));
    }
}

wxETKSQLite3RequestUpdater & wxETKSQLite3RequestUpdater::operator<<(const wxETKSQLite3Column &_rColumn)
{
    GetCriterionRequest().Add(wxETKSQLite3Expression(   GetOperationAssigment(),
                                                        wxETKSQLite3Expression(_rColumn,
                                                                               wxETKSQLite3Expression::eExpressionColumnNameOnly
                                                                              ),
                                                        wxETKSQLite3Expression(_rColumn.GetColumnAttributes())
                                                    )
                             );
    return *this;
}

wxETKSQLite3RequestUpdater & wxETKSQLite3RequestUpdater::operator<<(const wxETKSQLite3Expression &_rExpression)
{
    GetCriterionRequest().Add(_rExpression);
    return *this;
}

wxETKSQLite3Expression::eExpressionOrOperationType wxETKSQLite3RequestUpdater::GetOperationAssigment() const
{
    return wxETKSQLite3Expression::eOperationAssignmentUpdate;
}
