/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3Include.h
// Library:     wxETKSQLite3
// Purpose:     Header to include to add all library classes.
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/07/31
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// Used to call all includes needed to access to all classes of wxETKSQLite3 library
#include "wx/wx.h"
#include "wxETKSQLite3Types.h"

// Include wxSQLITE3 : database managment: should add this path to project
// into : Search Directories / Compiler, add :
// $(#wxsqlite3)\include
// $(#wxsqlite3)\sqlite3\include
#include <wx/wxsqlite3.h>

#include "wxETKSQLite3Database.h"
#include "wxETKSQLite3Record.h"
#include "wxETKSQLite3VariantDatas.h"
