#ifndef ETK_SQLITE3_VERSION_HEADER
#define ETK_SQLITE3_VERSION_HEADER

#if defined(_DEBUG) || defined(__WXDEBUG__) || defined(DEBUG)
    #define __ETKSQLITE3_DEBUG__
#endif

#define ETKSQLITE3_MAJOR_VERSION        1
#define ETKSQLITE3_MINOR_VERSION        0
#define ETKSQLITE3_RELEASE_NUMBER       0
#define ETKSQLITE3_SUBRELEASE_NUMBER    0
#define ETKSQLITE3_VERSION_STRING       "ETKSQLite3 1.0.0"
#ifdef ETK_SQLITE3_USE_WXWIDGETS
    #ifdef __ETKSQLITE3_DEBUG__
        #define ETKSQLITE3_DLLNAME      "wxETKSQLite3d.dll"
    #else
        #define ETKSQLITE3_DLLNAME      "wxETKSQLite3.dll"
    #endif
#elif ETK_SQLITE3_USE_QT
    #ifdef __ETKSQLITE3_DEBUG__
        #define ETKSQLITE3_DLLNAME      "qtETKSQLite3d.dll"
    #else
        #define ETKSQLITE3_DLLNAME      "qtETKSQLite3.dll"
    #endif
#elif ETK_SQLITE3_USE_BOOST
    #ifdef __ETKSQLITE3_DEBUG__
        #define ETKSQLITE3_DLLNAME      "boostETKSQLite3d.dll"
    #else
        #define ETKSQLITE3_DLLNAME      "boostETKSQLite3.dll"
    #endif
#elif ETK_SQLITE3_USE_STL
    #ifdef __ETKSQLITE3_DEBUG__
        #define ETKSQLITE3_DLLNAME      "stlETKSQLite3d.dll"
    #else
        #define ETKSQLITE3_DLLNAME      "stlETKSQLite3.dll"
    #endif
#endif

#endif // ETK_SQLITE3_VERSION_HEADER
