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
#include <fstream>

// Static instance of wxConfigBase
wxConfigBase wxConfigBase::m_sSettings;

// Constructeur privé
wxConfigBase::wxConfigBase() : m_strFilePath("config.ini")
{
    Load();
}

// Destructeur
wxConfigBase::~wxConfigBase()
{
    Save();
}

// Charge les paramètres depuis le fichier
void wxConfigBase::Load()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ifstream file(m_strFilePath);
    if (!file.is_open()) {
        return;
    }

    std::string line;
    std::string currentGroup;
    m_mapSettings.clear();

    while (std::getline(file, line))
    {
        // Ignorer les commentaires et les lignes vides
        if (line.empty() || line[0] == ';' || line[0] == '#')
        {
            continue;
        }

        // Gestion des groupes
        if (line[0] == '[' && line.back() == ']')
        {
            currentGroup = line.substr(1, line.size() - 2);
            continue;
        }

        // Parsing des paires clé=valeur
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos)
        {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            // Retirer les espaces
            key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
            value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());

            if (!currentGroup.empty())
            {
                key = currentGroup + "/" + key;
            }

            m_mapSettings[key] = value;
        }
    }
}

// Sauvegarde les paramètres dans le fichier
void wxConfigBase::Save()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ofstream file(m_strFilePath);
    if (!file.is_open())
    {
        return;
    }

    std::string currentGroup;
    for (const auto& pair : m_mapSettings)
    {
        size_t groupDelimiter = pair.first.find('/');
        std::string group = pair.first.substr(0, groupDelimiter);
        std::string key = pair.first.substr(groupDelimiter + 1);

        if (group != currentGroup)
        {
            if (!currentGroup.empty())
            {
                file << std::endl;
            }
            file << "[" << group << "]" << std::endl;
            currentGroup = group;
        }

        file << key << "=" << pair.second << std::endl;
    }
}

std::string wxConfigBase::GetFullKey(const wxString& key) const
{
    std::string fullKey;
    if (!m_strGroupName.IsEmpty())
    {
        fullKey += m_strGroupName.ToUTF8().data();
        fullKey += "/";
    }
    fullKey += key.ToUTF8().data();
    return fullKey;
}

wxConfigBase* wxConfigBase::Get()
{
    return &m_sSettings;
}

void wxConfigBase::Set(const wxChar* _pszFilePath)
{
    if (_pszFilePath) {
        m_sSettings.m_strFilePath = _pszFilePath;
        m_sSettings.Load();
    }
}

void wxConfigBase::SetPath(const wxChar* _pszPath)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (_pszPath != nullptr)
    {
        m_strGroupName = _pszPath;
    }
    else
    {
        m_strGroupName.clear();
    }
}

wxString wxConfigBase::Read(const wxChar* _pszKey, const wxChar* _pszDefault)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (_pszKey == nullptr)
    {
        return _pszDefault ? wxString(_pszDefault) : wxString();
    }

    std::string fullKey = GetFullKey(_pszKey);
    auto it = m_mapSettings.find(fullKey);
    if (it != m_mapSettings.end())
    {
        return wxString::FromUTF8(it->second.c_str());
    } else {
        return _pszDefault ? wxString(_pszDefault) : wxString();
    }
}

void wxConfigBase::Write(const wxChar* _pszKey, const wxChar* _pszValue)
{
    if (_pszKey != nullptr)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::string fullKey = GetFullKey(_pszKey);
        if (_pszValue != nullptr)
        {
            m_mapSettings[fullKey] = _pszValue;
        }
        else
        {
            m_mapSettings.erase(fullKey);
        }
    }
}