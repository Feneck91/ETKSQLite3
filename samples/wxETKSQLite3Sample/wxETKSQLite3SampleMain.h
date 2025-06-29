/***************************************************************
 * Name:      wxETKSQLite3SampleMain.h
 * Purpose:   Defines Application Frame
 * Author:    Stéphane Château (Feneck91@free.fr)
 * Created:   2011-08-11
 * Copyright: Stéphane Château ()
 * License:
 **************************************************************/

#ifndef WXETKSQLITE3SAMPLEMAIN_H
#define WXETKSQLITE3SAMPLEMAIN_H

#include "wxETKSQLite3SampleApp.h"

//(*Headers(wxETKSQLite3SampleFrame)
#include <wx/menu.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

class wxETKSQLite3SampleFrame: public wxFrame
{
    public:

        wxETKSQLite3SampleFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~wxETKSQLite3SampleFrame();

    private:

        //(*Handlers(wxETKSQLite3SampleFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnMenuItemViewTableName(wxCommandEvent& event);
        void OnMenuItemAddName(wxCommandEvent& event);
        void OnMenuItemClearTableName(wxCommandEvent& event);
        //*)

        //(*Identifiers(wxETKSQLite3SampleFrame)
        static const long ID_RICHTEXTCTRL1;
        static const long idMenuQuit;
        static const long idMenuAddName;
        static const long idMenuViewName;
        static const long idClearTabName;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(wxETKSQLite3SampleFrame)
        wxMenuItem* MenuItem5;
        wxMenu* Menu3;
        wxMenuItem* MenuItem4;
        wxMenuItem* MenuItem3;
        wxStatusBar* StatusBar1;
        wxRichTextCtrl* m_pRichTextCtrl;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // WXETKSQLITE3SAMPLEMAIN_H
