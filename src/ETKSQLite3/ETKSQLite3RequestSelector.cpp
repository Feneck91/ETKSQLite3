/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3RequestSelector.cpp
// Library:     ETKSQLite3
// Purpose:     Code to make select into database
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/09
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "ETKSQLite3RequestSelector.h"
#include "ETKSQLite3Database.h"
#include "ETKSQLite3ResultSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ETKSQLite3RequestSelector::ETKSQLite3RequestSelector(ETKSQLite3Database & _rDatabase)
    : ETKSQLite3Request(_rDatabase)
{
    // Set the correct type for the request criterion
    GetCriterionRequest().SetRequestType(ETKSQLite3Criterion::eRequestTypeSelect);
}

ETKSQLite3RequestSelector::ETKSQLite3RequestSelector(const ETKSQLite3Criterion &_rCriterion)
    : ETKSQLite3Request(* (ETKSQLite3Database *) NULL) // <-- This member is not used in this case else it crash
{
    GetCriterionRequest() = _rCriterion;
}

ETKSQLite3RequestSelector::ETKSQLite3RequestSelector(const ETKSQLite3RequestSelector & _rOther)
    : ETKSQLite3Request(_rOther)
{
}

ETKSQLite3RequestSelector::~ETKSQLite3RequestSelector()
{
}

ETKSQLite3RequestSelector & ETKSQLite3RequestSelector::operator<<(const ETKSQLite3Record &_rRecord)
{
    for (size_t stIndex = 0;stIndex < _rRecord.GetColumnCount();++stIndex)
    {
        operator<<(_rRecord.GetColumn(stIndex));
    }

    return *this;
}

ETKSQLite3RequestSelector & ETKSQLite3RequestSelector::operator<<(const ETKSQLite3Column &_rColumn)
{
    GetCriterionRequest().Add(ETKSQLite3Expression(_rColumn));
    return *this;
}

ETKSQLite3RequestSelector & ETKSQLite3RequestSelector::operator<<(const ETKSQLite3Criterion &_rCriterion)
{
    GetCriterionRequest().Add(_rCriterion);
    return *this;
}

ETKSQLite3RequestSelector & ETKSQLite3RequestSelector::operator<<(const ETKSQLite3Expression &_rExpression)
{
    GetCriterionRequest().Add(_rExpression);
    return *this;
}

ETKSQLite3RequestSelector & ETKSQLite3RequestSelector::AddOrderByAscending(const ETKSQLite3Column& _rColumn)
{
    GetCriterionRequest().AddOrderByAscending(_rColumn);
    return *this;
}

ETKSQLite3RequestSelector & ETKSQLite3RequestSelector::AddOrderByDescending(const ETKSQLite3Column& _rColumn)
{
    GetCriterionRequest().AddOrderByDescending(_rColumn);
    return *this;
}

wxSQLite3ResultSet ETKSQLite3RequestSelector::ExecuteQuery()
{
    wxSQLite3ResultSet resultSet;
    if (PrepareStatement())
    {   // Statement is ready, execute it
        resultSet = GetDatabase().ExecuteQueryStatement(GetStatement());
        DeleteStatement();
    }

    return resultSet;
}

int ETKSQLite3RequestSelector::Execute(bool _bWithTransaction)
{
    wxUnusedVar(_bWithTransaction);
    wxString strErrorMessage(wxT("ETKSQLite3RequestSelector::Execute() must never call(have no sense)!"));
    wxFAIL_MSG(strErrorMessage);
    throw wxSQLite3Exception(-1,strErrorMessage);
    return 0;
}
