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
#ifndef WX_FILE_HEADER
#define WX_FILE_HEADER

#include "wxtype.h"
#include <QFile>

class EXPORT_IMPORT wxFile : public QFile
{
public:
    static bool Exists(const wxChar *_pszFilePath)                                      { return QFile::exists(_pszFilePath); }
};

bool EXPORT_IMPORT wxFileExists(const wxChar *_pszFilePath);
bool EXPORT_IMPORT wxRemoveFile(const wxChar *_pszFilePath);

#endif // WX_FILE_HEADER
