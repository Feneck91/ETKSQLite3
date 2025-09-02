/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/messagedialog.h
 * @brief Header file for wxMessageDialog class.
 *
 * Used to display message box.
 *
 * @author Stéphane Château
 * @date Created: 2025/09/02
 * @date Modified:
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_MESSAGE_DIALOG_HEADER
#define WX_MESSAGE_DIALOG_HEADER

#include <QMessageBox>

#define wxICON_ERROR            (QMessageBox::Critical)
#define wxICON_QUESTION         (QMessageBox::Question)
#define wxICON_INFORMATION      (QMessageBox::Information)
#define wxICON_WARNING          (QMessageBox::Warning)
#define wxICON_CRITICAL         (QMessageBox::Critical)
#define wxICON_MASK             (wxICON_ERROR | wxICON_INFORMATION | wxICON_WARNING | wxICON_CRITICAL)

#define wxOK                    (QMessageBox::Ok)
#define wxCANCEL                (QMessageBox::Cancel)

class wxMessageDialog
{
private:
    QWidget *   m_pParent;
    QString     m_strTitle;
    QString     m_strMessage;
    long        m_lStyle;

public:
    wxMessageDialog(QWidget *_pParent,QString _strTitle,QString _strMessage,long _lStyle);

    long ShowModal();
};

#endif // WX_MESSAGE_DIALOG_HEADER