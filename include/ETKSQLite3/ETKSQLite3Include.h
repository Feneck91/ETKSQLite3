/////////////////////////////////////////////////////////////////////////////
/**
 * @file ETKSQLite3Include.h
 * @brief Header to include to add all library classes.
 *
 * This file contains all include needed to use the ETKSQLite3 library.
 *
 * @author Stéphane Château du header
 * @date Created: 2011/07/31
 * @date Modified: 2025/09/01
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////

// Used to call all includes needed to access to all classes of wxETKSQLite3 library
#ifdef _MSC_VER // Only for MSVC compiler
    #pragma warning(push) // Backup actual warning parameters
    #pragma warning(disable : 4251) // Should have dll interface
#elif defined(__GNUC__) || defined(__clang__) // GCC/Clang (Linux/macOS)
    #pragma GCC diagnostic push
    #ifdef MAKING_ETK_SQLITE3_SHARED
        #pragma GCC diagnostic ignored "-Wattributes"
    #endif
#endif

#include "ETKSQLite3Types.h"
#include "ETKSQLite3Database.h"
#include "ETKSQLite3Record.h"
#include "ETKSQLite3VariantDatas.h"

#ifdef _MSC_VER // Only for MSVC compiler
    #pragma warning(pop) // Restaure warning parameters
#elif defined(__GNUC__) || defined(__clang__) // GCC/Clang (Linux/macOS)
    #pragma GCC diagnostic pop
#endif
