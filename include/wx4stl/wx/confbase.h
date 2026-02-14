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
#include <map>
#include <mutex>
#include <algorithm>

/**
 * Class that simulate the wxConfigBase class.
 */
class wxConfigBase
{
private:
    static wxConfigBase m_sSettings;
    wxString m_strGroupName;
    std::string m_strFilePath;
    std::map<std::string, std::string> m_mapSettings;
    std::mutex m_mutex;

public:
    // Constructeur privé pour le singleton
    EXPORT_IMPORT wxConfigBase();

    // Charge les paramètres depuis le fichier
    EXPORT_IMPORT void Load();

    // Sauvegarde les paramètres dans le fichier
    EXPORT_IMPORT void Save();

    // Génère la clé complète (groupe + clé)
    EXPORT_IMPORT std::string GetFullKey(const wxString& key) const;

public:
    // Empêcher la copie et l'affectation
    EXPORT_IMPORT wxConfigBase(const wxConfigBase&) = delete;
    EXPORT_IMPORT wxConfigBase& operator=(const wxConfigBase&) = delete;

    // Destructor
    EXPORT_IMPORT ~wxConfigBase();

    // Static public
    EXPORT_IMPORT void SetPath(const wxChar* _pszPath);
    EXPORT_IMPORT wxString Read(const wxChar* _pszKey, const wxChar* _pszDefault);
    EXPORT_IMPORT void Write(const wxChar* _pszKey, const wxChar* _pszValue);

    // Static methods
    EXPORT_IMPORT static wxConfigBase* Get();
    EXPORT_IMPORT static void Set(const wxChar* _pszFilePath); // Définit le chemin du fichier de configuration
};

#endif // WX_CONFIG_BASE_HEADER
