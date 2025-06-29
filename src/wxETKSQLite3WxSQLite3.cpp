/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3WxSQLite3.h
// Library:     wxETKSQLite3
// Purpose:     Code to compile wxSQLite3 into library
// Author:      Stéphane Château (Feneck91@free.fr)
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
// WXMAKINGDLL_WXSQLITE3 to get table name in ResultSet

#ifdef _MSC_VER
    // Include wxSQLITE3 : database managment
    #include <wx/wxsqlite3.h>
#endif

#ifndef __WXDEBUG__
    // Database is not crypted in debug mode
    #define WXSQLITE3_HAVE_CODEC        1
#endif
#define wxUSE_DYNAMIC_SQLITE3_LOAD      1
#define WXSQLITE3_HAVE_METADATA         1
#define WXMAKINGDLL_WXSQLITE3

#if defined(_WIN32) && !defined(__WIN32__)
    #define __WIN32__
#endif

#include "wxsqlite3.cpp"
