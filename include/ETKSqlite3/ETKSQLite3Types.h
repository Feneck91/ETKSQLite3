/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3Types.h
// Library:     wxETKSQLite3
// Purpose:     Define types, export/import managment.
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/07/31
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_ETK_SQLITE3_TYPES_H
#define INCLUDE_ETK_SQLITE3_TYPES_H

#include "ETKSQLite3ImportExport.h"

// Visual Studio compatibility the precompiler header file don't work like mingw
// Must add include into all C++ file ! So, the compiler is defined without precompiler
// header file, but in this case the project MUST ADD the following files.
#include <wx/wx.h>
#include <wx/variant.h>
#include <wx/confbase.h>

// Include wxSQLITE3 : database managment: should add this path to project
// into : Search Directories / Compiler, add :
// $(#wxsqlite3)\include
// $(#wxsqlite3)\sqlite3\include
#include "wx/wxsqlite3.h"

// Include SQLITE3 : database managment: should add this path to project
#include "sqlite3mc_amalgamation.h"

// Difference with 2.8.x version and later
#if wxCHECK_VERSION(2,9,0)
    // --> Error, with this : elif defined(__GNUC__) && !wxCHECK_GCC_VERSION(4, 5)
    // --> Into 2.9.x
    //
    // While gcc also supports __declspec(dllexport), it creates unusably huge
    // DLL files since gcc 4.5 (while taking horribly long amounts of time),
    // see http://gcc.gnu.org/bugzilla/show_bug.cgi?id=43601. Because of this
    // we rely on binutils auto export/import support which seems to work
    // quite well for 4.5+.
    #undef WXEXPORT
    #define     WXEXPORT        EXPORT_IMPORT
#endif

// Define all wx classes alias to etk classes.
using etkString             = wxString;
using etkVariant            = wxVariant;
using etkArrayInt           = wxArrayInt;
using etkInt64              = wxLongLong;
using etkDateTime           = wxDateTime;

#endif // INCLUDE_ETK_SQLITE3_TYPES_H
