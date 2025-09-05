/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/logger.cpp
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
#include <wx/wx.h>
#include <wx/logger.h>

// Logs
wx4qtLogger * wx4qtLogger::m_pInstance = nullptr;
wx4qtLogger  *wx4qtLogger::Getwx4qtLoggerInstance()
{
    return m_pInstance;
}

wx4qtLogger::wx4qtLogger()
{
    m_pInstance = this;
}

wx4qtLogger::~wx4qtLogger()
{
    m_pInstance = nullptr;
}

void wx4qtLogger::LogDebug(wxString _strLogString)
{
    wxUnusedVar(_strLogString);
}

void wx4qtLogger::wxLogDebug(wxString _strLogString)
{
    wxUnusedVar(_strLogString);
}

void wx4qtLogger::wxLogInfo(wxString _strFormat, wxString  _strCodeErrorMessage, int _iErrorCode, wxString _strMessage)
{
    wxUnusedVar(_strFormat);
    wxUnusedVar(_strCodeErrorMessage);
    wxUnusedVar(_iErrorCode);
    wxUnusedVar(_strMessage);
}

void wx4qtLogger::wxLogVerbose(wxString _strLogString)
{
    wxUnusedVar(_strLogString);
}

void wx4qtLogger::wxLogError(wxString _strLogString)
{
    wxUnusedVar(_strLogString);
}

void wx4qtLogger::wxLogFatalError(wxString _strLogString)
{
    wxUnusedVar(_strLogString);
}

void wxLogDebug(const wxChar *_pszMessage)
{
    if (wx4qtLogger::Getwx4qtLoggerInstance() != nullptr)
    {
        wx4qtLogger::Getwx4qtLoggerInstance()->wxLogDebug(wxString(_pszMessage));
    }
}

void wxLogDebug(const wxChar *_pszFormat,const wxChar *_pszMessage)
{
    wxLogDebug(wxString::Format(_pszFormat,_pszMessage));
}

void wxLogInfo(const wxChar *_pszFormat, const wxChar *_pszCodeErrorMessage, int _iErrorCode, wxString _strMessage)
{
    if (wx4qtLogger::Getwx4qtLoggerInstance() != nullptr)
    {
        wx4qtLogger::Getwx4qtLoggerInstance()->wxLogInfo(wxString(_pszFormat), wxString(_pszCodeErrorMessage), _iErrorCode, _strMessage);
    }
}

void wxLogVerbose(const wxChar *_pszMessage)
{
    if (wx4qtLogger::Getwx4qtLoggerInstance() != nullptr)
    {
        wx4qtLogger::Getwx4qtLoggerInstance()->wxLogVerbose(wxString(_pszMessage));
    }
}

void wxLogVerbose(const wxChar *_pszFormat,const wxChar *_pszMessage)
{
    wxLogVerbose(wxString::Format(_pszFormat,_pszMessage));
}

void wxLogError(const wxChar *_pszMessage)
{
    if (wx4qtLogger::Getwx4qtLoggerInstance() != nullptr)
    {
        wx4qtLogger::Getwx4qtLoggerInstance()->wxLogError(wxString(_pszMessage));
    }
}

void wxLogError(const wxChar *_pszFormat,const wxChar *_pszMessage)
{
    wxLogError(wxString::Format(_pszFormat,_pszMessage));
}

void wxLogFatalError(const wxChar *_pszMessage)
{
    if (wx4qtLogger::Getwx4qtLoggerInstance() != nullptr)
    {
        wx4qtLogger::Getwx4qtLoggerInstance()->wxLogFatalError(wxString(_pszMessage));
    }
}

void wxLogFatalError(const wxChar *_pszFormat,const wxChar *_pszMessage)
{
    wxLogFatalError(wxString::Format(_pszFormat,_pszMessage));
}
