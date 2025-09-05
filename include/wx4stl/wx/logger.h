/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/logger.h
 * @brief Implementation of logger to use with stl.
 *
 * Allow to use internal log by derived loggers class.
 *
 * @author Stéphane Château
 * @date Created: 2025/09/02
 * @date Modified:
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
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
void EXPORT_IMPORT wxLogInfo(const wxChar *_pszFormat, const wxChar *_pszCodeErrorMessage, int _iErrorCode, wxString _strMessage);
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

    virtual void LogDebug(wxString _strLogString);
    virtual void wxLogDebug(wxString _strLogString);
    virtual void wxLogInfo(wxString _strFormat, wxString  _strCodeErrorMessage, int _iErrorCode, wxString _strMessage);
    virtual void wxLogVerbose(wxString _strLogString);
    virtual void wxLogError(wxString _strLogString);
    virtual void wxLogFatalError(wxString _strLogString);

    // Get the instance of logger
    static wx4qtLogger  *Getwx4qtLoggerInstance();
};

#endif // WX_LOGGER_HEADER