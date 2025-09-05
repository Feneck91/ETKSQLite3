/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/file.h
 * @brief Header of wxFile class.
 *
 * wxFile class declaration.
 *
 * @author Stéphane Château
 * @date Created: 2012/06/14
 * @date Modified: 2025/09/02
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_FILE_HEADER
#define WX_FILE_HEADER

#include <wx/string.h>

/**
 * Not exported, only internal used.
 */
class wxFile
{
public:
    static bool Exists(const wxChar *_pszFilePath);
};

bool wxFileExists(const wxChar *_pszFilePath);
bool wxRemoveFile(const wxChar *_pszFilePath);

#endif // WX_FILE_HEADER
