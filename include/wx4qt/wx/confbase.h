/////////////////////////////////////////////////////////////////////////////
// Name:        wx/confbase.h
// Purpose:     Config base
// Library:     Copied from wxWidgets, modifyed to be used with QT library
// Author:      Stéphane Château
// Modified by:
// Created:     14/06/2012
// Copyright:   (c) Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_CONFIG_BASE_HEADER
#define WX_CONFIG_BASE_HEADER

#include "wxtype.h"
#include "string.h"
#include <QSettings>

/**
 * Class that simulate the wxConfigBase class.
 */
class EXPORT_IMPORT wxConfigBase
{
private:
    static wxConfigBase m_sSettings;
    QString m_strGroupName;

private:
    QSettings *m_pQSettings;

public:
    static wxConfigBase * Get();
    static void Set(QSettings *_pQSettings); // Replace static pointer to config base
    void SetPath(const wxChar *_pszPath);
    wxString Read(const wxChar *_pszKey,const wxChar *_pszDefault);
    void Write(const wxChar *_pszKey,const wxChar *_pszValue);
};

#endif // WX_CONFIG_BASE_HEADER
