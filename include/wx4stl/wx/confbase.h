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
class EXPORT_IMPORT wxConfigBase
{
private:
    static wxConfigBase m_sSettings;
    wxString m_strGroupName;
    std::string m_strFilePath;
    std::map<std::string, std::string> m_mapSettings;
    std::mutex m_mutex;

public:
    // Constructeur privé pour le singleton
    wxConfigBase();

    // Charge les paramètres depuis le fichier
    void Load();

    // Sauvegarde les paramètres dans le fichier
    void Save();

    // Génère la clé complète (groupe + clé)
    std::string GetFullKey(const wxString& key) const;

public:
    // Empêcher la copie et l'affectation
    wxConfigBase(const wxConfigBase&) = delete;
    wxConfigBase& operator=(const wxConfigBase&) = delete;

    // Méthodes statiques
    static wxConfigBase* Get();
    static void Set(const wxChar* _pszFilePath); // Définit le chemin du fichier de configuration

    // Méthodes publiques
    void SetPath(const wxChar* _pszPath);
    wxString Read(const wxChar* _pszKey, const wxChar* _pszDefault);
    void Write(const wxChar* _pszKey, const wxChar* _pszValue);

    // Destructeur
    ~wxConfigBase();
};

#endif // WX_CONFIG_BASE_HEADER
