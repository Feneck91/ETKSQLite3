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
#include <string>

#define wxICON_ERROR            (1 << 0)
#define wxICON_QUESTION         (1 << 1)
#define wxICON_INFORMATION      (1 << 2)
#define wxICON_WARNING          (1 << 3)
#define wxICON_CRITICAL         wxICON_ERROR
#define wxICON_MASK             (wxICON_ERROR | wxICON_QUESTION | wxICON_INFORMATION | wxICON_WARNING)

#define wxOK                    (1 << 4)
#define wxCANCEL                (1 << 5)
#define wxYES_NO                (1 << 6)
#define wxYES_DEFAULT           (1 << 7)
#define wxNO_DEFAULT            (1 << 8)

class wxMessageDialog
{
private:
    void * m_pParent; // Generic pointer for parent window (can be HWND, NSWindow*, etc.)
    std::string m_strMessage;
    std::string m_strTitle;
    long m_lStyle;

public:
    wxMessageDialog(void* _pParent, const std::string& _strMessage, const std::string& _strTitle, long _lStyle);
    long ShowModal();
};

#endif // WX_MESSAGE_DIALOG_HEADER