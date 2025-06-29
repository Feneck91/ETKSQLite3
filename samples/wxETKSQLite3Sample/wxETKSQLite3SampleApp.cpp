/***************************************************************
 * Name:      wxETKSQLite3SampleApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Stéphane Château (Feneck91@free.fr)
 * Created:   2011-08-11
 * Copyright: Stéphane Château ()
 * License:
 **************************************************************/

#include "wx_pch.h"
#include "wxETKSQLite3SampleApp.h"
#include "wxETKSQLite3SampleDatabaseDatas.h"

//(*AppHeaders
#include "wxETKSQLite3SampleMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(wxETKSQLite3SampleApp);

bool wxETKSQLite3SampleApp::OnInit()
{
    bool bRet = true;

    wxFileName fileExePath(wxStandardPaths::Get().GetExecutablePath());
    wxString strGetExePath =  fileExePath.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR); // With '\' at the end

    // Set current path same as EXE
    wxSetWorkingDirectory(strGetExePath);

    wxString strAppName(GetAppNameWithoutDebugTag());

    // Compute ini file path
    wxFileName fnIniFilename;
    fnIniFilename.Assign(strGetExePath,strAppName+_T(".ini"),wxPATH_NATIVE);

    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	wxETKSQLite3SampleFrame* Frame = new wxETKSQLite3SampleFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    else
    {
        bRet = false;
    }
    if (bRet)
    {
        bRet = false;


    m_database.SetSQLDatabaseSchema(  wxETKSQLite3SampleDataTName::GetSQLTableConstruction()
                                    + wxETKSQLite3SampleDataTClub::GetSQLTableConstruction()
                                    );

        if (!m_database.Open())
        {
            wxFileDialog saveFileDialog(NULL,_("Create database file"),wxEmptyString,_T("wxETKSQLiteSample.db3"),
                                        _("SQlite3 database (*.db3)|*.db3"),
                                        wxFD_SAVE,
                                        wxDefaultPosition
                                       );

            // Save as...
            if (saveFileDialog.ShowModal() == wxID_OK)
            {   // The file must not already exists
                if (wxFileExists(saveFileDialog.GetPath()))
                {
                    if (m_database.Open(saveFileDialog.GetPath()))
                    {
                        wxMessageDialog msgDlg(NULL,wxString::Format(_("The file '%s' cannot be loaded!"),saveFileDialog.GetPath().c_str()),_("Cannot load database"),wxOK | wxICON_ERROR);
                        msgDlg.ShowModal();
                    }
                }
                else
                {   // Create database
                    if (m_database.Create(saveFileDialog.GetPath()))
                    {   //
                        // Create clubs.
                        wxETKSQLite3SampleDataTClub club;
                        wxETKSQLite3RequestInserter inserter = m_database.GetInserter();
                        inserter<<club;   // <-- Add all columns
                        club.SetClubName(wxT("The New York club"));
                        club.SetClubCity(wxT("New York"));
                        inserter.ExecuteWithTransaction(); // Add this one

                        club.SetClubName(wxT("The Paris club"));
                        club.SetClubCity(wxT("Paris"));
                        inserter.ExecuteWithTransaction(); // Add this one

                        club.SetClubName(wxT("The London club"));
                        club.SetClubCity(wxT("London"));
                        inserter.ExecuteWithTransaction(); // Add this one

                        club.SetClubName(wxT("The Berlin club"));
                        club.SetClubCity(wxT("Berlin"));
                        inserter.ExecuteWithTransaction(); // Add this one

                        club.SetClubName(wxT("The Unused club"));
                        club.SetClubCity(wxT("NeverUsed"));
                        inserter.ExecuteWithTransaction(); // Add this one
                        // You can add lot of elements with only one transaction, else it's take a while !
                        try
                        {
                            wxMessageBox(wxT("start"), _("Begin"));
                            wxSQLite3Transaction transaction(m_database.GetDatabase());

                            for (int i=0;i<10000;++i)
                            {
                                club.SetClubName(wxString::Format(wxT("Club%03d"),i));
                                club.SetClubCity(wxString::Format(wxT("Club City%03d"),i));
                                inserter.ExecuteWithTransaction(); // Add this one
                            }
                            transaction.Commit();
                            bRet = true;
                            wxMessageBox(wxT("end"), _("End"));
                        }
                        catch(wxSQLite3Exception &)
                        {   // Error executing transation, is automatically rolled back
                            // Nothing to log, already done
                            bRet = false;
                        }
                    }
                }
            }
        }
        else
        {
            bRet = true;
        }
    }

    return bRet;
}

wxString wxETKSQLite3SampleApp::GetAppNameWithoutDebugTag() const
{
    wxString strAppName(GetAppName());
    #ifdef __WXDEBUG__
    if (strAppName.Right(1).Upper() == _T("D"))
    {   // d for debug, remove d at the end of exe for ini file
        strAppName = strAppName.Left(strAppName.Length() - 1);
    }
    #endif
    return strAppName;
}
