/////////////////////////////////////////////////////////////////////////////
/**
 * @file ETKSQLite3RequestInserter.cpp
 * @brief Implementation file for inserter request managment.
 *
 * Code for database request inserter class.
 *
 * @author Stéphane Château
 * @date Created: 2011/08/04
 * @date Modified: 2025/09/01
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
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
