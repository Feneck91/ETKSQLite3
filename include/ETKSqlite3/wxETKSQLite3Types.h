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
#ifndef INCLUDE_WX_ETK_SQLITE3_TYPES_H
#define INCLUDE_WX_ETK_SQLITE3_TYPES_H

//################################################################
//##                                                            ##
//##            Definition of macro to export/imports           ##
//##                    functions and classes                   ##
//##                                                            ##
//################################################################
#ifdef EXPORT_IMPORT
    #undef EXPORT_IMPORT
#endif

#ifdef WIN32
    #ifdef MAKING_WX_ETK_SQLITE3_LIB
        #define             WXMAKINGLIB_WXSQLITE3
        #define             EXPORT_IMPORT
    #else
        #ifdef USING_WX_ETK_SQLITE3_LIB
            #define         EXPORT_IMPORT
        #else
            #ifdef MAKING_WX_ETK_SQLITE3_DLL
                #define     WXMAKINGDLL_WXSQLITE3
                #define     EXPORT_IMPORT      __declspec(dllexport)
            #else
                #define     WXUSINGDLL_WXSQLITE3
                #define     EXPORT_IMPORT      __declspec(dllimport)
            #endif
        #endif
    #endif
#else
    #ifdef LINUX
        #error Not implemented !
    #else
        #error Not implemented !
    #endif
#endif

#ifdef _MSC_VER
    // Visual Studio compatibility the precompiler header file don't work like mingw
    // Must add include into all C++ file ! So, the compiler is defined without precompiler
    // header file, but in this case the project MUST ADD the following files.
    #include <wx/wx.h>
    // Include wxSQLITE3 : database managment
    #include <wx/wxsqlite3.h>
#else
    #include <wx/wx.h>
#endif

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

#endif // INCLUDE_WX_ETK_SQLITE3_TYPES_H
