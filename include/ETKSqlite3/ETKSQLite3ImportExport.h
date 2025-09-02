/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3ImportExport.h
// Library:     wxETKSQLite3
// Purpose:     Define import / export.
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2025/09/01
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_ETK_SQLITE3_IMPORT_EXPORT_H
#define INCLUDE_ETK_SQLITE3_IMPORT_EXPORT_H

//################################################################
//##                                                            ##
//##          Definition of macro to export/imports             ##
//##                                                            ##
//################################################################
#ifdef EXPORT_IMPORT
    #undef EXPORT_IMPORT
#endif

#ifdef WIN32
    #ifdef MAKING_ETK_SQLITE3_LIB
        #define             WXMAKINGLIB_WXSQLITE3
        #define             EXPORT_IMPORT
        #define             SQLITE_API
    #else
        #ifdef USING_ETK_SQLITE3_LIB
            #define         EXPORT_IMPORT
        #else
            #ifdef MAKING_ETK_SQLITE3_SHARED
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

// Define WXEXPORT like EXPORT_IMPORT
#define     WXEXPORT            EXPORT_IMPORT

#endif // INCLUDE_ETK_SQLITE3_IMPORT_EXPORT_H