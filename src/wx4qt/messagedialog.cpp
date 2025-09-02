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
#include <wx/messagedialog.h>

wxMessageDialog::wxMessageDialog(QWidget *_pParent,QString _strTitle,QString _strMessage,long _lStyle)
    : m_pParent(_pParent)
    , m_strTitle(_strTitle)
    , m_strMessage(_strMessage)
    , m_lStyle(_lStyle)
{
}

long wxMessageDialog::ShowModal()
{
    QMessageBox msgBox;
    //set inforative text
    msgBox.setInformativeText(m_strTitle);
    // Set Message
    msgBox.setText(m_strMessage);
    // Add ok and cancel button.
    msgBox.setStandardButtons((QMessageBox::StandardButton) (m_lStyle & ~wxICON_MASK));
    //Set predefined icon, icon is show on left side of text.
    msgBox.setIcon((QMessageBox::Icon) (m_lStyle & wxICON_MASK));
    // Set modal
    msgBox.setWindowModality(Qt::WindowModal);
    // Execute message box. method exec() return the button value of cliecke button
    return msgBox.exec();
}
