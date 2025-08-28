#pragma once

// Include the wxETKSQLite3 library header files.
#include "ETKSQLite3Include.h"

class CETKSQlite3SampleDatabase : public ETKSQLite3Database
{
public:
    CETKSQlite3SampleDatabase();
    ~CETKSQlite3SampleDatabase();

    /**
     * Get the SQL database creation structure.
     *
     * It is called only on database creation if no SQL schema is set.
     * @return The SQL needed to construct the database.
     */
    virtual wxString                    GetSQLDatabaseSchema() const;

    /**
     * Execute actions after created or opened database.
     *
     * This script enabling Foreign Key Support and must be called each time the database is
     * opened or created. It must be before creating structure ror adding element. This function is
     * the first function to call when the database is opened or create. The database may don't contains
     * the tables structures.
     *
     * @throw wxSQLite3Exception If the function failed, a wxSQLite3Exception is raided so calling this function
     *                           must be protected by try / catch statement.
     */
    virtual void                        ExecuteConfigAfterOpenOrCreate();
};
