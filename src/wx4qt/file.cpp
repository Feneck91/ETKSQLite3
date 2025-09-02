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
    return QFile::exists(_pszFilePath);
}

bool wxFileExists(const wxChar *_pszFilePath)
{
    QFile file(_pszFilePath);
    return file.exists();
}

bool wxRemoveFile(const wxChar *_pszFilePath)
{
    QFile file(_pszFilePath);
    return file.remove();
}

