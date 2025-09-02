/////////////////////////////////////////////////////////////////////////////
// Name:        wx/logger.h
// Purpose:     Contains logger to use with Qt.
// Library:     Class added to be used with QT library
// Author:      Stéphane Château
// Modified by:
// Created:     2025/09/02
// Copyright:   (c) Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef WX_LOGGER_HEADER
#define WX_LOGGER_HEADER

#include <wx/wxtype.h>
#include <wx/string.h>

//
// Logger
//
void EXPORT_IMPORT wxLogDebug(const wxChar *_pszMessage);
void EXPORT_IMPORT wxLogDebug(const wxChar *_pszFormat,const wxChar *_pszMessage);
void EXPORT_IMPORT wxLogInfo(const wxChar *_pszFormat, const wxChar *_pszCodeErrorMessage, int _iErrorCode, wxQString _strMessage);
void EXPORT_IMPORT wxLogVerbose(const wxChar *_pszMessage);
void EXPORT_IMPORT wxLogVerbose(const wxChar *_pszFormat,const wxChar *_pszMessage);
void EXPORT_IMPORT wxLogError(const wxChar *_pszMessage);
void EXPORT_IMPORT wxLogError(const wxChar *_pszFormat,const wxChar *_pszMessage);
void EXPORT_IMPORT wxLogFatalError(const wxChar *_pszMessage);
void EXPORT_IMPORT wxLogFatalError(const wxChar *_pszFormat,const wxChar *_pszMessage);

/**
 * To use it, just derive it and instanciate derived class.
 *
 * Need to override virtual functions.
 */
class EXPORT_IMPORT wx4qtLogger
{
private:
    static wx4qtLogger  *m_pInstance;

public:
    wx4qtLogger();
    virtual ~wx4qtLogger();

    virtual void LogDebug(QString _strLogString);
    virtual void wxLogDebug(QString _strLogString);
    virtual void wxLogInfo(QString _strFormat, QString  _strCodeErrorMessage, int _iErrorCode, QString _strMessage);
    virtual void wxLogVerbose(QString _strLogString);
    virtual void wxLogError(QString _strLogString);
    virtual void wxLogFatalError(QString _strLogString);

    // Get the instance of logger
    static wx4qtLogger  *Getwx4qtLoggerInstance();
};

#endif // WX_LOGGER_HEADER