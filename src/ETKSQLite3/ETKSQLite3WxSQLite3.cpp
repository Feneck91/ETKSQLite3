/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3WxSQLite3.h
// Library:     ETKSQLite3
// Purpose:     Code to compile wxSQLite3 into library
// Author:      Stéphane Château
// Modified by:
// Created:     2011/07/31
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// In order to make a dll with wxSQLite3 project, only decompress the
// wxSQLite3 projects and source code and compile it here in including the "wxsqlite3.cpp"
// source file into this one. It use "$(#wxsqlite3)" root path to be able to quickly use newer
// version of wxSQLite3. The SQLite dynamic library is loaded dynamically with
// wxUSE_DYNAMIC_SQLITE3_LOAD define.
// WXSQLITE3_HAVE_CODEC is used to be able to crypt generated database.
// WXMAKINGDLL_WXSQLITE3 to get table name in ResultSet, this option is mandatory

#include "ETKSQLite3Types.h" // Mandatory for __WXDEBUG__

#ifndef __WXDEBUG__
    // Database is not crypted in debug mode
    #define WXSQLITE3_HAVE_CODEC        1
#endif

#ifndef SQLITE_ENABLE_COLUMN_METADATA
    #error The wxSQLite3 must be compiled with SQLITE_ENABLE_COLUMN_METADATA flag to make ETKSQLite3 works
#endif

#if defined(_WIN32) && !defined(__WIN32__)
    #define __WIN32__
#endif

#if defined(_MSC_VER) // Only for MSVC compiler
    #pragma warning(push)
    #pragma warning(disable : 4267) // conversion, possible loss of data
    #pragma warning(disable : 4189) // local variable initialized but not referenced
    #pragma warning(disable : 4100) // unreferenced formal parameter
#elif defined(__GNUC__) || defined(__clang__) // GCC/Clang (Linux/macOS)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wconversion"
    #pragma GCC diagnostic ignored "-Wunused-variable"
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    #ifdef MAKING_ETK_SQLITE3_SHARED
        #pragma GCC diagnostic ignored "-Wattributes"
    #endif
#endif

#include "wxsqlite3.cpp"

#if defined(_MSC_VER) // Only for MSVC compiler
    #pragma warning(pop)
#elif defined(__GNUC__) || defined(__clang__) // GCC/Clang (Linux/macOS)
    #pragma GCC diagnostic pop
#endif
