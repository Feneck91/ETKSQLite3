/***************************************************************
 * Name:      wxETKSQLite3SampleApp.h
 * Purpose:   Defines Application Class
 * Author:    Stéphane Château (Feneck91@free.fr)
 * Created:   2011-08-11
 * Copyright: Stéphane Château ()
 * License:
 **************************************************************/

#ifndef WXETKSQLITE3SAMPLEAPP_H
#define WXETKSQLITE3SAMPLEAPP_H

#include <wx/app.h>

class wxETKSQLite3SampleApp : public wxApp
{
public:
    // Instance of the SQLite3 database
    wxETKSQLite3Database                m_database;
public:
    virtual bool                        OnInit();
    wxString                            GetAppNameWithoutDebugTag() const;
};

DECLARE_APP(wxETKSQLite3SampleApp);

#endif // WXETKSQLITE3SAMPLEAPP_H
