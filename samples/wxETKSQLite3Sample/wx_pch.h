/***************************************************************
 * Name:      wx_pch.h
 * Purpose:   Header to create Pre-Compiled Header (PCH)
 * Author:    Stéphane Château (Feneck91@free.fr)
 * Created:   2011-08-11
 * Copyright: Stéphane Château ()
 * License:
 **************************************************************/

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
    // Include wxETKSQLITE3 : database managment library
    #include "wx/wxETKSQLite3Include.h"

    #include <wx/fileconf.h>
    #include <wx/filename.h>
    #include <wx/stdpaths.h>
#endif // WX_PRECOMP

#endif // WX_PCH_H_INCLUDED
