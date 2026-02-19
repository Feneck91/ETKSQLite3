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
#ifdef _WIN32
    #include <sys/types.h>
    #include <sys/stat.h>
#else
    #include <sys/stat.h>
#endif

bool wxFile::Exists(const wxChar *_pszFilePath)
{
    bool bRet = false;
    if (_pszFilePath != nullptr && *_pszFilePath != 0)
    {
#ifdef _WIN32
    #ifdef UNICODE
        struct _stat buffer;
        bRet = (_wstat(_pszFilePath, &buffer) == 0);
    #else
        struct _stat buffer;
        bRet = (_stat(_pszFilePath, &buffer) == 0);
    #endif
#else
    struct stat buffer;
    bRet = (stat(_pszFilePath, &buffer) == 0);
#endif        
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

