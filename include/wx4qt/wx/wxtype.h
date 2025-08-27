/////////////////////////////////////////////////////////////////////////////
// Name:        wx/wxtype.h
// Purpose:     Used to import / export function and classes
// Library:     Copied from wxWidgets, modifyed to be used with QT library
// Author:      Stéphane Château
// Modified by:
// Created:     14/06/2012
// Copyright:   (c) Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_TYPE_HEADER
#define WX_TYPE_HEADER

#if defined(WIN32)
    /*
       __declspec works in BC++ 5 and later, Watcom C++ 11.0 and later as well
       as VC++ and gcc
     */
#    if defined(__VISUALC__) || defined(_MSC_VER) || defined(__BORLANDC__) || defined(__GNUC__) || defined(__WATCOMC__)
#        define WXEXPORT __declspec(dllexport)
#        define WXIMPORT __declspec(dllimport)
#    else /* compiler doesn't support __declspec() */
#        define WXEXPORT
#        define WXIMPORT
#    endif
#endif

/* for other platforms/compilers we don't anything */
#ifndef WXEXPORT
#    define WXEXPORT
#    define WXIMPORT
#endif

#include "ETKSQLite3Types.h"

#endif // WX_TYPE_HEADER
