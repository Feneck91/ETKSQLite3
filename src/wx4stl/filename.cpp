/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/filename.cpp
 * @brief Implemntation of wxFileName class.
 *
 * wxFileName class implementation.
 *
 * @author Stéphane Château
 * @date Created: 2012/06/14
 * @date Modified: 2025/09/02
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#include <wx/filename.h>

wxFileName::wxFileName(const std::string _rstrFileName)
{
    m_strFileName = _rstrFileName;
}

std::string wxFileName::GetFullPath() const
{
    return m_strFileName;
}
