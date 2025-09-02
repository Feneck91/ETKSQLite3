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

#include <QString>

/**
 * Not exported, only internal used.
 */
class wxFileName : public QString
{
private:
    QString m_strFileName;

public:
    wxFileName(const QString _rstrFileName);
    QString GetFullPath() const;
};

#endif // WX_FILENAME_HEADER
