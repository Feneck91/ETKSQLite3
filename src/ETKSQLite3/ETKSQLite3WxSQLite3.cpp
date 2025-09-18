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

// Defines that can be used
// #define WXMAKINGDLL_WXSQLITE3

#if defined(_WIN32) && !defined(__WIN32__)
    #define __WIN32__
#endif

#pragma warning(push) // Backup actual warning parameters
#pragma warning(disable : 4267) // Conversion lost data
#pragma warning(disable : 4189) // Local variable not used
#pragma warning(disable : 4100) // Parameter not referenced
#include "wxsqlite3.cpp"
#pragma warning(pop) // Restaure warning parameters