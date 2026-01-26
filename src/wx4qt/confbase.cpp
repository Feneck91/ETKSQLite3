/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/confbase.cpp
 * @brief Implementation file for ConfigBase.
 *
 * ConfigBase, simul wxWidget class to be used with other frameworks.
 *
 * @author Stéphane Château
 * @date Created: 2012/06/14
 * @date Modified: 2025/09/01
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#include "wx/confbase.h"


// Static instance of wxConfigBase
wxConfigBase wxConfigBase::m_sSettings;

wxConfigBase *wxConfigBase::Get()
{
    return &m_sSettings;
}

void wxConfigBase::Set(QSettings *_pSettings)
{   // Replace pointer
    m_sSettings.m_pQSettings = _pSettings;
}

void wxConfigBase::SetPath(const wxChar *_pszPath)
{
    m_strGroupName = _pszPath;
}

wxString wxConfigBase::Read(const wxChar *_pszKey,const wxChar *_pszDefault)
{
    wxUnusedVar(_pszDefault);
    if (m_pQSettings != NULL)
    {
        m_pQSettings->beginGroup(m_strGroupName);
        wxString strRet(m_pQSettings->value(_pszKey).toString());
        m_pQSettings->endGroup();
        return strRet;
    }

    wxASSERT_MSG(false, wxT("m_pQSettings is not initialized, CALL wxConfigBase::Set function!"));
    return wxString();
}

void wxConfigBase::Write(const wxChar *_pszKey,const wxChar *_pszValue)
{
    wxASSERT_MSG(m_pQSettings != NULL, wxT("m_pQSettings is not initialized, CALL wxConfigBase::Set function!"));

    if (m_pQSettings != NULL)
    {
        m_pQSettings->beginGroup(m_strGroupName);
        m_pQSettings->setValue(_pszKey,_pszValue);
        m_pQSettings->endGroup();
    }
}

