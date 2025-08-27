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
#ifndef WX_FILENAME_HEADER
#define WX_FILENAME_HEADER

#include "wxtype.h"
#include <QFile>

class EXPORT_IMPORT wxFileName : public QString
{
private:
    QString m_strFileName;

public:
    wxFileName(const QString _rstrFileName)                                             { m_strFileName = _rstrFileName; }
    QString GetFullPath() const                                                         { return m_strFileName; }
};

#endif // WX_FILENAME_HEADER
