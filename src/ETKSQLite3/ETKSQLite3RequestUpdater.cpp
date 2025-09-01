/////////////////////////////////////////////////////////////////////////////
/**
 * @file ETKSQLite3RequestUpdater.cpp
 * @brief Implementation file for updater request managment.
 *
 * Code for database request update class.
 *
 * @author Stéphane Château
 * @date Created: 2011/08/04
 * @date Modified: 2025/09/01
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
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

etkInt64 ETKSQLite3RequestUpdater::GetLastRowId() const
{
    return ETKSQLite3Request::GetLastRowId();
}

void ETKSQLite3RequestUpdater::operator<<(const ETKSQLite3Record &_rRecord)
{
    for (size_t stIndex = 0; stIndex < _rRecord.GetColumnCount(); ++stIndex)
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
