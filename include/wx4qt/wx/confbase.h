/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/buffer.h
 * @brief Header file for ConfigBase.
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
#ifndef WX_CONFIG_BASE_HEADER
#define WX_CONFIG_BASE_HEADER

#include <wx/wx.h>
#include <wx/string.h>
#include <wx/messagedialog.h>
#include <QSettings>

/**
 * Class that simulate the wxConfigBase class.
 */
class EXPORT_IMPORT wxConfigBase
{
private:
    static wxConfigBase m_sSettings;
    QString             m_strGroupName;
    QSettings *         m_pQSettings;

public:
    static wxConfigBase *       Get();
    static void                 Set(QSettings *_pQSettings); // Replace static pointer to config base
    void                        SetPath(const wxChar *_pszPath);
    wxString                    Read(const wxChar *_pszKey,const wxChar *_pszDefault);
    void                        Write(const wxChar *_pszKey,const wxChar *_pszValue);
};

#endif // WX_CONFIG_BASE_HEADER
