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

#ifdef WIN32
// Windows version
#include <windows.h>
wxMessageDialog::wxMessageDialog(void* _pParent, const std::string& _strTitle, const std::string& _strMessage, long _lStyle)
    : m_pParent(_pParent), m_strTitle(_strTitle), m_strMessage(_strMessage), m_lStyle(_lStyle)
{
}

long wxMessageDialog::ShowModal()
{
    UINT uType = 0;

    // Icon
    if (m_lStyle & wxICON_ERROR)
    {
        uType |= MB_ICONERROR;
    }
    else if (m_lStyle & wxICON_QUESTION)
    {
        uType |= MB_ICONQUESTION;
    }
    else if (m_lStyle & wxICON_INFORMATION)
    {
        uType |= MB_ICONINFORMATION;
    }
    else if (m_lStyle & wxICON_WARNING)
    {
        uType |= MB_ICONWARNING;
    }

    // Buttons
    if (m_lStyle & wxYES_NO)
    {
        uType |= MB_YESNO;
        if (m_lStyle & wxYES_DEFAULT)
        {
            uType |= MB_DEFBUTTON1;
        }
        else if (m_lStyle & wxNO_DEFAULT)
        {
            uType |= MB_DEFBUTTON2;
        }
    }
    else
    {
        uType |= MB_OK;
        if (m_lStyle & wxCANCEL)
        {
            uType |= MB_OKCANCEL;
        }
    }

    // Display dialog box
    int result = MessageBoxA(static_cast<HWND>(m_pParent), m_strMessage.c_str(), m_strTitle.c_str(), uType);

    // Translate result
    if (result == IDOK)
    {
        return wxOK;
    }
    else if (result == IDCANCEL)
    {
        return wxCANCEL;
    }
    else if (result == IDYES)
    {
        return wxOK | wxYES_NO;
    }
    else if (result == IDNO)
    {
        return wxCANCEL | wxYES_NO;
    }

    return 0;
}
#else
// Version Linux/macOS (GTK+ or native API)
#include <gtk/gtk.h>

wxMessageDialog::wxMessageDialog(void* _pParent, const std::string& _strTitle, const std::string& _strMessage, long _lStyle)
    : m_pParent(_pParent), m_strTitle(_strTitle), m_strMessage(_strMessage), m_lStyle(_lStyle)
{
}

long wxMessageDialog::ShowModal()
{
    GtkWidget* dialog;
    GtkButtonsType buttons = GTK_BUTTONS_NONE;
    GtkMessageType messageType = GTK_MESSAGE_INFO;

    // Icon
    if (m_lStyle & wxICON_ERROR)
    {
        messageType = GTK_MESSAGE_ERROR;
    }
    else if (m_lStyle & wxICON_QUESTION)
    {
        messageType = GTK_MESSAGE_QUESTION;
    }
    else if (m_lStyle & wxICON_INFORMATION)
    {
        messageType = GTK_MESSAGE_INFO;
    }
    else if (m_lStyle & wxICON_WARNING)
    {
        messageType = GTK_MESSAGE_WARNING;
    }

    // Button
    if (m_lStyle & wxYES_NO)
    {
        buttons = GTK_BUTTONS_YES_NO;
    }
    else if (m_lStyle & wxCANCEL)
    {
        buttons = GTK_BUTTONS_OK_CANCEL;
    }
    else
    {
        buttons = GTK_BUTTONS_OK;
    }

    // Create dialog box
    dialog = gtk_message_dialog_new(static_cast<GtkWindow*>(m_pParent),
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    messageType,
                                    buttons,
                                    "%s", m_strMessage.c_str());

    gtk_window_set_title(GTK_WINDOW(dialog), m_strTitle.c_str());

    // Display dialog box
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    // Clean
    gtk_widget_destroy(dialog);

    // Trnaslate result
    if (result == GTK_RESPONSE_OK)
    {
        return wxOK;
    }
    else if (result == GTK_RESPONSE_CANCEL)
    {
        return wxCANCEL;
    }
    else if (result == GTK_RESPONSE_YES)
    {
        return wxOK | wxYES_NO;
    }
    else if (result == GTK_RESPONSE_NO)
    {
        return wxCANCEL | wxYES_NO;
    }

    return 0;
}

#endif