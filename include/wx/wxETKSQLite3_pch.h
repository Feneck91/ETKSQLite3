/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3_pch.h
// Library:     wxETKSQLite3
// Purpose:     Header to create Pre-Compiled Header (PCH)
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/07/31
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef WX_PCH_H_INCLUDED
#define WX_PCH_H_INCLUDED

// basic wxWidgets headers
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifdef WX_PRECOMP
    // put here all your rarely-changing header files

    // Used to call all includes needed to access to all classes of wxSQLite3 library
    // and correct import / export wxSQLite3 library.
    #include "wxETKSQLite3Types.h"  // Correctly defines constants to compile wxsqlite3.h
    // Include wxSQLITE3 : database managment
    #include <wx/wxsqlite3.h>
#endif // WX_PRECOMP

#endif // WX_PCH_H_INCLUDED
