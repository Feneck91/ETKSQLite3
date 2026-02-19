/////////////////////////////////////////////////////////////////////////////
// Name:        wx/wx.h
// Purpose:     Contains general constants of wxWidgets
// Library:     Copied from wxWidgets, modifyed to be used with QT library
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
#include <QVector>
#include <QtGlobal>
#include <assert.h>

/*
#ifndef INCLUDE_WX_ETK_SQLITE3_VALUE_BIND_H
class wxSQLite3Statement;
class wxSQLite3ResultSet;
#include "ETKSQLite3ValueBind.h"
// Bind QT types : export all full template specialization: used to bind QT type directly without using wxWidgets wrappers out of the library
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<qint64>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<qint64>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QString>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QString>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QDate>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QDate>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QTime>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QTime>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QDateTime>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QDateTime>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;
#endif //INCLUDE_WX_ETK_SQLITE3_VALUE_BIND_H
*/
#endif // WX_HEADER
