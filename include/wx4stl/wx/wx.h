/////////////////////////////////////////////////////////////////////////////
// Name:        wx/wx.h
// Purpose:     Contains general constants of wxWidgets
// Library:     Copied from wxWidgets, modifyed to be used with STL library
// Author:      Stéphane Château
// Modified by:
// Created:     14/06/2012
// Copyright:   (c) Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_HEADER
#define WX_HEADER

#include <wx/wxdefines.h>
#include <wx/wxtype.h>
#include <wx/string.h>
#include <wx/logger.h>
#include <assert.h>

/*
#ifndef INCLUDE_WX_ETK_SQLITE3_VALUE_BIND_H
class wxSQLite3Statement;
class wxSQLite3ResultSet;
#include "ETKSQLite3ValueBind.h"
// Bind STL types : export all full template specialization: used to bind STL type directly without using wxWidgets wrappers out of the library
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<etkInt64>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<etkInt64>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<etkString>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<etkString>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<etkDate>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<etkDate>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<etkTime>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<etkTime>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<etkDateTime>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<etkDateTime>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;
#endif //INCLUDE_WX_ETK_SQLITE3_VALUE_BIND_H
*/
#endif // WX_HEADER
