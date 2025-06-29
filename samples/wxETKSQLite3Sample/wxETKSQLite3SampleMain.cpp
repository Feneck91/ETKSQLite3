/***************************************************************
 * Name:      wxETKSQLite3SampleMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Stéphane Château (Feneck91@free.fr)
 * Created:   2011-08-11
 * Copyright: Stéphane Château ()
 * License:
 **************************************************************/

#include "wx_pch.h"
#include "wxETKSQLite3SampleMain.h"
#include "wxETKSQLite3SampleDatabaseDatas.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(wxETKSQLite3SampleFrame)
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include <wx/datetime.h>

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(wxETKSQLite3SampleFrame)
const long wxETKSQLite3SampleFrame::ID_RICHTEXTCTRL1 = wxNewId();
const long wxETKSQLite3SampleFrame::idMenuQuit = wxNewId();
const long wxETKSQLite3SampleFrame::idMenuAddName = wxNewId();
const long wxETKSQLite3SampleFrame::idMenuViewName = wxNewId();
const long wxETKSQLite3SampleFrame::idClearTabName = wxNewId();
const long wxETKSQLite3SampleFrame::idMenuAbout = wxNewId();
const long wxETKSQLite3SampleFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxETKSQLite3SampleFrame,wxFrame)
    //(*EventTable(wxETKSQLite3SampleFrame)
    //*)
END_EVENT_TABLE()

wxETKSQLite3SampleFrame::wxETKSQLite3SampleFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(wxETKSQLite3SampleFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    m_pRichTextCtrl = new wxRichTextCtrl(this, ID_RICHTEXTCTRL1, wxEmptyString, wxPoint(80,136), wxDefaultSize, wxRE_MULTILINE|wxRE_READONLY, wxDefaultValidator, _T("ID_RICHTEXTCTRL1"));
    	wxRichTextAttr rchtxtAttr_1;
    wxFont Font_1 = wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT);
    if ( !Font_1.Ok() ) Font_1 = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    rchtxtAttr_1.SetFontFaceName(Font_1.GetFaceName());
    rchtxtAttr_1.SetFontSize(Font_1.GetPointSize());
    rchtxtAttr_1.SetFontStyle(Font_1.GetStyle());
    rchtxtAttr_1.SetFontUnderlined(Font_1.GetUnderlined());
    rchtxtAttr_1.SetFontWeight(Font_1.GetWeight());
    m_pRichTextCtrl->SetBasicStyle(rchtxtAttr_1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu3 = new wxMenu();
    MenuItem4 = new wxMenuItem(Menu3, idMenuAddName, _("Add 100 names"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItem4);
    MenuItem3 = new wxMenuItem(Menu3, idMenuViewName, _("View Table Name"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItem3);
    MenuItem5 = new wxMenuItem(Menu3, idClearTabName, _("Clear Table Name"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItem5);
    MenuBar1->Append(Menu3, _("&Database"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxETKSQLite3SampleFrame::OnQuit);
    Connect(idMenuAddName,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxETKSQLite3SampleFrame::OnMenuItemAddName);
    Connect(idMenuViewName,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxETKSQLite3SampleFrame::OnMenuItemViewTableName);
    Connect(idClearTabName,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxETKSQLite3SampleFrame::OnMenuItemClearTableName);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxETKSQLite3SampleFrame::OnAbout);
    //*)
}

wxETKSQLite3SampleFrame::~wxETKSQLite3SampleFrame()
{
    //(*Destroy(wxETKSQLite3SampleFrame)
    //*)
}

void wxETKSQLite3SampleFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void wxETKSQLite3SampleFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void wxETKSQLite3SampleFrame::OnMenuItemViewTableName(wxCommandEvent& event)
{
    m_pRichTextCtrl->Clear();


    //---------------------------------------
    // Count number of row in the table
    wxETKSQLite3RequestSelector selectorCount = wxGetApp().m_database.GetSelector();
    // On this column
    //selectorCount.GetCriterionRequest() = dbCount(wxETKSQLite3SampleDataTName::COLUMN_FIELD_ClubID);
    // Or on all columns
    selectorCount.GetCriterionRequest() = dbCount(wxETKSQLite3Record::GetAllColumn(wxETKSQLite3SampleDataTName::TABLE_NAME));
    m_pRichTextCtrl->AppendText(wxString::Format(wxT("SQL count = %s\r\n"),selectorCount.GetCriterionRequest().GetSQL().wx_str()));

    wxSQLite3ResultSet resultSetCount = selectorCount.ExecuteQuery();
    int iRow = 1;
    while( resultSetCount.NextRow())
    {
        wxString message;
        message.Printf(wxT("Row %d : "),iRow);
        m_pRichTextCtrl->AppendText(message);
        for (int iIndexColumn=0;iIndexColumn<resultSetCount.GetColumnCount();++iIndexColumn)
        {
            wxString message;
            message.Printf(wxT("%s.%s = %s"),
                           resultSetCount.GetTableName(iIndexColumn).wx_str(),
                           resultSetCount.GetColumnName(iIndexColumn).wx_str(),
                           resultSetCount.GetAsString(iIndexColumn).wx_str()
                          );
            m_pRichTextCtrl->AppendText(message);
        }
        m_pRichTextCtrl->AppendText(_T("\r\n"));
    }

    // View content of the table
    wxETKSQLite3RequestSelector selector = wxGetApp().m_database.GetSelector();
    selector<<wxETKSQLite3SampleDataTName();   // <-- Add all columns of this table
    selector<<wxETKSQLite3SampleDataTClub::COLUMN_FIELD_ClubName;
    //--------------------------------------------------------------------------------
    // Selector with Where on two column of different table makes JOIN between tables
    // It give result ONLY if element exists in the second table. Here, all field with NULL as TName.idTClub are not
    // into the result set
    //selector.Where(wxETKSQLite3SampleDataTName::COLUMN_FIELD_ClubID == wxETKSQLite3SampleDataTClub::COLUMN_FIELD_ID);

    //--------------------------------------------------------------------------------
    // Same result as previous one but using Join SQL instuction
    //selector.Join(wxETKSQLite3SampleDataTName::COLUMN_FIELD_ClubID.Join(wxETKSQLite3SampleDataTClub::COLUMN_FIELD_ID));

    //--------------------------------------------------------------------------------
    // All lines, even TName.idTClub is null with Left Join SQL instruction
    selector.Join(wxETKSQLite3SampleDataTName::COLUMN_FIELD_ClubID.LeftJoin(wxETKSQLite3SampleDataTClub::COLUMN_FIELD_ID));

    m_pRichTextCtrl->AppendText(wxString::Format(wxT("SQL= %s\r\n"),selector.GetCriterionRequest().GetSQL().wx_str()));
    // The binding is into resultSet for reading
    wxString strClubName;
    wxETKSQLite3ResultSet<wxETKSQLite3SampleDataTName> resultSet = selector.ExecuteQuery();

    // First way to bind data, directly with a wxString
    // The result set contains only
    resultSet.AddBind(wxETKSQLite3SampleDataTClub::COLUMN_FIELD_ClubName,&strClubName);

    //-----------------------------------------------------
    // Anoyher way to bind this value, directly with column name
    // resultSet.AddBind(_T("TClub.ClubName"),&strClubName);

    //-----------------------------------------------------
    // Another way to bind this value, directly with record
    // Use club.m_strClubName instead of strClubName (case (1)), all fields of club are not binded because they
    // are not into the result set (not into the SQL request), but doesn't matter, it bind all need
    // wxETKSQLite3SampleDataTClub club;
    // resultSet.AddBind(club);


    wxString strText;
    strText.Printf(_T("------------------------------------------------------------------------------------------------------\r\n"
                      "|   ID   | First name           | Name                 | Birthday             | Club Name            |\r\n"
                      "------------------------------------------------------------------------------------------------------\r\n"));
    m_pRichTextCtrl->AppendText(strText);

    while (resultSet.NextRow())
    {
        strText.Printf(_T("| %6s | %20s | %20s | %20s | %20s |\r\n"),
                       resultSet[wxETKSQLite3SampleDataTName::COLUMN_FIELD_ID]->IsNull()
                           ? wxString(_T("(null)")).c_str()
                           : resultSet.GetID().ToString().wx_str(),
                       resultSet[wxETKSQLite3SampleDataTName::COLUMN_FIELD_FirstName]->IsNull()
                           ? wxString(_T("(null)")).c_str()
                           : resultSet.GetFirstName().c_str(),
                       resultSet[wxETKSQLite3SampleDataTName::COLUMN_FIELD_Name]->IsNull()
                           ? wxString(_T("(null)")).c_str()
                           : resultSet.GetName().c_str(),
                       resultSet[wxETKSQLite3SampleDataTName::COLUMN_FIELD_Birthday]->IsNull()
                           ? wxString(_T("(null)")).c_str()
                           : resultSet.GetBirthday().Format(_T("%x %X")).c_str(),
                       resultSet[wxETKSQLite3SampleDataTClub::COLUMN_FIELD_ClubName]->IsNull()
                           ? wxString(_T("(none)")).c_str()
                           : strClubName.c_str() // club.m_strClubName.c_str() in case (1)
                      );
        m_pRichTextCtrl->AppendText(strText);
    }
    m_pRichTextCtrl->AppendText(_T("\r\nDone."));
}

void wxETKSQLite3SampleFrame::OnMenuItemAddName(wxCommandEvent& event)
{
    m_pRichTextCtrl->Clear();
    // Create one instance for binding
    wxETKSQLite3SampleDataTName myNameInstance;

    wxETKSQLite3RequestInserter inserter = wxGetApp().m_database.GetInserter();
    inserter<<myNameInstance;   // <-- Add all columns

    wxString strText;
    strText.Printf(_T("-----------------------------------------------------------------------------------------|\r\n"
                      "|   ID   | First name           | Name                 | Birthday             | Status   |\r\n"
                      "-----------------------------------------------------------------------------------------|\r\n"));
    m_pRichTextCtrl->AppendText(strText);

    for (int i=0;i<100;++i)
    {
        myNameInstance.SetFirstName(wxString::Format(_T("First_Name %d"),i+1));
        myNameInstance.SetName(wxString::Format(_T("Name %d"),i+1));
        wxDateTime dtBirthday;
        dtBirthday.Set(1,wxDateTime::Jan,2005,0,0,0,0);
        dtBirthday.Add(wxTimeSpan::Hours(12) + wxTimeSpan::Hours(24).Multiply(i)); // One day more on each i
        myNameInstance.SetBirthday(dtBirthday);

        wxString strClubName;
        wxETKSQLite3RequestSelector selector = wxGetApp().m_database.GetSelector();
        selector << wxETKSQLite3SampleDataTClub::COLUMN_FIELD_ID; // Find ID
        // Can do : selector << tClubInstance; to add all column of TClub table

        switch (i % 5)
        {   // Finding club id
            case 0 : // The New York club
                // The simplest way is to search the club name
                selector.Where(wxETKSQLite3SampleDataTClub::COLUMN_FIELD_ClubName == wxT("The New York club"));
                break;
            case 1 : // The Paris club
                // But for example, I could find only with substring of club name
                selector.Where(dbSubString(wxETKSQLite3SampleDataTClub::COLUMN_FIELD_ClubName,3,7) == wxT("e Paris"));
                break;
            case 2 : // The London club
                // But for example, I could find the name on club name and city with substring
                selector.Where(   dbSubString(wxETKSQLite3SampleDataTClub::COLUMN_FIELD_ClubName,3,7) == wxT("e Londo")
                               && dbSubString(wxETKSQLite3SampleDataTClub::COLUMN_FIELD_ClubCity,2,4) == wxT("ondo"));
                break;
            case 3 : // The Berlin club
                // But for example, doesn't search from city with EQUAL operator (==) but with
                // NOT differents operators (! !=) (same but just as an example)
                selector.Where(!(wxETKSQLite3SampleDataTClub::COLUMN_FIELD_ClubCity != wxT("Berlin")));
                break;
            case 4 : // No club
                break;
        }
        if ((i % 5) != 4)
        {
            wxETKSQLite3ResultSet<wxETKSQLite3SampleDataTClub> resultset= selector.ExecuteQuery();
            if (resultset.NextRow())
            {   // The only one element bind is m_lID
                myNameInstance[wxETKSQLite3SampleDataTName::COLUMN_FIELD_ClubID]->SetNull(false);
                myNameInstance.SetClubID(resultset.GetID());
            }
            else
            {
                wxASSERT(false);
            }
        }
        else
        {
            myNameInstance[wxETKSQLite3SampleDataTName::COLUMN_FIELD_ClubID]->SetNull(true);
        }


        int iNbRowInserted = inserter.ExecuteWithTransaction();
        if (iNbRowInserted == 1)
        {
            strText.Printf(_T("| %6s | %20s | %20s | %20s | %8s |\r\n"),
                           inserter.GetLastRowId().ToString().wx_str(),
                           myNameInstance.GetFirstName().c_str(),
                           myNameInstance.GetName().c_str(),
                           myNameInstance.GetBirthday().Format(_T("%x %X")).c_str(),
                           wxString::Format(_T("%s"),(iNbRowInserted == 1) ? _T("ok") : _T("failed")).c_str()
                          );
            m_pRichTextCtrl->AppendText(strText);
        }
        else
        {
            m_pRichTextCtrl->AppendText(wxGetApp().m_database.GetLastErrorMessage());
            break; // exit for
        }
    }
}

void wxETKSQLite3SampleFrame::OnMenuItemClearTableName(wxCommandEvent& event)
{
    m_pRichTextCtrl->Clear();
    wxETKSQLite3RequestDeleter deleter = wxGetApp().m_database.GetDeleter();
    deleter<<wxETKSQLite3SampleDataTName::TABLE_NAME;   // <-- Add with table name
    // Could also do (it does same as previosu line):
    // deleter<<wxETKSQLite3SampleDataTName();                 <-- With the record

    wxString strText;
    strText.Printf(_T("Remove [%d] rows"),deleter.ExecuteWithTransaction());
    m_pRichTextCtrl->AppendText(strText);
}
