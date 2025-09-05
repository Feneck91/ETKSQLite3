/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/filename.h
 * @brief Header of wxFileName class.
 *
 * wxFileName class declaration.
 *
 * @author Stéphane Château
 * @date Created: 2012/06/14
 * @date Modified: 2025/09/02
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_FILENAME_HEADER
#define WX_FILENAME_HEADER

#include <string>

/**
 * Not exported, only internal used.
 */
class wxFileName : public std::string
{
private:
    std::string m_strFileName;

public:
    wxFileName(const std::string _rstrFileName);
    std::string GetFullPath() const;
};

#endif // WX_FILENAME_HEADER
