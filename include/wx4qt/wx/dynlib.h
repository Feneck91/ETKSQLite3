/////////////////////////////////////////////////////////////////////////////
// Name:        wx/buffer.h
// Purpose:     wxDynamicLibrary : dynamic library to load
// Library:     Copied from wxWidgets, modifyed to be used with QT library
// Author:      Stéphane Château
// Modified by:
// Created:     14/06/2012
// Copyright:   (c) Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_DYNAMIC_LIBRARY_HEADER
#define WX_DYNAMIC_LIBRARY_HEADER

#include "wxtype.h"
#include <QLibrary>

#define wxDynamicLibrary    wxQDynamicLibrary

class EXPORT_IMPORT wxQDynamicLibrary : public QLibrary
{
public:
    wxQDynamicLibrary()                                                                 { }
    bool IsLoaded() const                                                               { return isLoaded (); }
    bool Load(const char *_pszFileName)                                                 { setFileName(_pszFileName); return load(); }
    void * GetSymbol(const char *_pszSymbol)                                            { return resolve(_pszSymbol); }
    bool Unload()                                                                       { return unload(); }
};

#endif // WX_DYNAMIC_LIBRARY_HEADER
