/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/file.cpp
 * @brief Implentation of wxFile class.
 *
 * wxFile class source.
 *
 * @author Stéphane Château
 * @date Created: 2012/06/14
 * @date Modified: 2025/09/02
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#include <wx/file.h>

bool wxFile::Exists(const wxChar *_pszFilePath)
{
    bool bRet = false;
    if (_pszFilePath != nullptr && *_pszFilePath != 0)
    {
        struct stat buffer;
        bRet = (stat(_pszFilePath, &buffer) == 0);
    }
    return bRet;
};

bool wxFileExists(const wxChar *_pszFilePath)
{
    return wxFile::Exists(_pszFilePath);
}

bool wxRemoveFile(const wxChar *_pszFilePath)
{
    bool bRet = wxFile::Exists(_pszFilePath);
    if (bRet)
    {
        bRet = std::remove(_pszFilePath);
    }

    return bRet;
}

