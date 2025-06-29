/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3RequestSelector.cpp
// Library:     wxETKSQLite3
// Purpose:     Code to make select into database
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/09
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "wxETKSQLite3RequestSelector.h"
#include "wxETKSQLite3Database.h"
#include "wxETKSQLite3ResultSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxETKSQLite3RequestSelector::wxETKSQLite3RequestSelector(wxETKSQLite3Database & _rDatabase)
    : wxETKSQLite3Request(_rDatabase)
{
    // Set the correct type for the request criterion
    GetCriterionRequest().SetRequestType(wxETKSQLite3Criterion::eRequestTypeSelect);
}

wxETKSQLite3RequestSelector::wxETKSQLite3RequestSelector(const wxETKSQLite3Criterion &_rCriterion)
    : wxETKSQLite3Request(* (wxETKSQLite3Database *) NULL) // <-- This member is not used in this case else it crash
{
    GetCriterionRequest() = _rCriterion;
}

wxETKSQLite3RequestSelector::wxETKSQLite3RequestSelector(const wxETKSQLite3RequestSelector & _rOther)
    : wxETKSQLite3Request(_rOther)
{
}

wxETKSQLite3RequestSelector::~wxETKSQLite3RequestSelector()
{
}

wxETKSQLite3RequestSelector & wxETKSQLite3RequestSelector::operator<<(const wxETKSQLite3Record &_rRecord)
{
    for (size_t stIndex = 0;stIndex < _rRecord.GetColumnCount();++stIndex)
    {
        operator<<(_rRecord.GetColumn(stIndex));
    }

    return *this;
}

wxETKSQLite3RequestSelector & wxETKSQLite3RequestSelector::operator<<(const wxETKSQLite3Column &_rColumn)
{
    GetCriterionRequest().Add(wxETKSQLite3Expression(_rColumn));
    return *this;
}

wxETKSQLite3RequestSelector & wxETKSQLite3RequestSelector::operator<<(const wxETKSQLite3Criterion &_rCriterion)
{
    GetCriterionRequest().Add(_rCriterion);
    return *this;
}

wxETKSQLite3RequestSelector & wxETKSQLite3RequestSelector::operator<<(const wxETKSQLite3Expression &_rExpression)
{
    GetCriterionRequest().Add(_rExpression);
    return *this;
}

wxETKSQLite3RequestSelector & wxETKSQLite3RequestSelector::AddOrderByAscending(const wxETKSQLite3Column& _rColumn)
{
    GetCriterionRequest().AddOrderByAscending(_rColumn);
    return *this;
}

wxETKSQLite3RequestSelector & wxETKSQLite3RequestSelector::AddOrderByDescending(const wxETKSQLite3Column& _rColumn)
{
    GetCriterionRequest().AddOrderByDescending(_rColumn);
    return *this;
}

wxSQLite3ResultSet wxETKSQLite3RequestSelector::ExecuteQuery()
{
    wxSQLite3ResultSet resultSet;
    if (PrepareStatement())
    {   // Statement is ready, execute it
        resultSet = GetDatabase().ExecuteQueryStatement(GetStatement());
        DeleteStatement();
    }

    return resultSet;
}

int wxETKSQLite3RequestSelector::Execute(bool _bWithTransaction) throw(wxSQLite3Exception)
{
    wxString strErrorMessage(wxT("wxETKSQLite3RequestSelector::Execute() must never call(have no sense)!"));
    wxFAIL_MSG(strErrorMessage);
    throw wxSQLite3Exception(-1,strErrorMessage);
    return 0;
}
