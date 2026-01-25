/////////////////////////////////////////////////////////////////////////////
/**
 * @file ETKSQLite3RequestInserter.h
 * @brief Header file for inserter request managment.
 *
 * Code for database request inserter class.
 *
 * @author Stéphane Château
 * @date Created: 2011/08/05
 * @date Modified: 2025/09/01
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_ETK_SQLITE3_REQUEST_INSERTER_H
#define INCLUDE_ETK_SQLITE3_REQUEST_INSERTER_H

#include "ETKSQLite3Types.h"
#include "ETKSQLite3RequestUpdater.h"

/**
 * Class used to make insert request.
 *
 * Is used to insert rows into table. Unlike selection request, this class update column field
 * directly with binding without convert into text (it is <i>wxSQLite3Statement</i> that makes
 * the last string conversion).<br>
 * This class need the schema of the table (the <i>ETKSQLite3Record</i> class) to know each
 * column table's name and make the binding. This class is based on updater class.
 *
 * @see ETKSQLite3RequestUpdater
 * @author Stéphane Château
 * @version Nom : ETKSQLite3<br>
 *          Révision : <b>1.0</b>
 */
class EXPORT_IMPORT ETKSQLite3RequestInserter : public ETKSQLite3RequestUpdater
{
    /// @name Constructor / Destructor.
    //@{
protected:
    /**
     * Protected constructor.
     *
     * This constructor is private, only <i>ETKSQLite3Database::GetInserter()</i> could return
     * an instance of this class.
     *
     * @param _rDatabase Reference on database.
     */
    ETKSQLite3RequestInserter(ETKSQLite3Database & _rDatabase);
public:
    /**
     * Copy constructor.
     *
     * @param _rRequestInserter Element to copy into this.
     */
    ETKSQLite3RequestInserter(const ETKSQLite3RequestInserter & _rRequestInserter);

    /**
     * Destructor.
     */
    virtual ~ETKSQLite3RequestInserter();
    //@}

protected:
    /**
     * Used into adding columns function to add with correct expression type.
     *
     * This type only differs from update to insert request, so operators could be defined
     * into the update base class.
     *
     * @return Always ETKSQLite3Expression::eOperationAssignmentInsert.
     */
    virtual ETKSQLite3Expression::eExpressionOrOperationType GetOperationAssigment() const;

    /**
     * Allow ETKSQLite3Database class to create ETKSQLite3RequestInserter instance.
     */
    friend class ETKSQLite3Database;
};

#endif // INCLUDE_ETK_SQLITE3_REQUEST_INSERTER_H
