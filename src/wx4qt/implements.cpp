/////////////////////////////////////////////////////////////////////////////
// Name:        wx/implements.h
// Purpose:     Implementations of wx classes to be used with QT
// Library:     Copied from wxWidgets, modifyed to be used with QT library
// Author:      Stéphane Château
// Modified by:
// Created:     14/06/2012
// Copyright:   (c) Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/wxtype.h>
#include <wx/confbase.h>
#include <wx/tokenzr.h>
#include <wx/string.h>
#include <wx/filename.h>
#include <wx/variant.h>

#include <time.h>
#include <QLocale>

//----------------------------------------------------------------------------------------------------------------
//--                                        Binding QT Classes                                                  --
//----------------------------------------------------------------------------------------------------------------

#include "ETKSQLite3ValueBind.h"
#include "wx/wxSQLite3.h"

// Bind QT types : export all full template specialization
template<> void ETKSQLite3ValueBindOther<qint64>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest, bool _bForInsertRequest) const
{
    wxUnusedVar(_bAddForInsertRequest);
    wxUnusedVar(_bForInsertRequest);
    _rstmt.Bind(_iIndex,wxLongLong(*m_pDataPtr));
}

template<> void ETKSQLite3ValueBindOther<qint64>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
{
    wxASSERT_MSG(!_rResultSet.IsNull(_iIndex), wxT("BindFrom must not be called on null column!"));
    *m_pDataPtr = _rResultSet.GetInt64(_iIndex).GetValue();
}

template<> void ETKSQLite3ValueBindOther<QString>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const
{
    wxUnusedVar(_bAddForInsertRequest);
    wxUnusedVar(_bForInsertRequest);
    _rstmt.Bind(_iIndex,wxString(*m_pDataPtr));
}

template<> void ETKSQLite3ValueBindOther<QString>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
{
    wxASSERT_MSG(!_rResultSet.IsNull(_iIndex), wxT("BindFrom must not be called on null column!"));
    *m_pDataPtr = _rResultSet.GetAsString(_iIndex);
}

template<> void ETKSQLite3ValueBindOther<QDate>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const
{
    wxUnusedVar(_bAddForInsertRequest);
    wxUnusedVar(_bForInsertRequest);
    _rstmt.BindDate(_iIndex,wxDateTime(*m_pDataPtr));
}

template<> void ETKSQLite3ValueBindOther<QDate>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
{
    wxASSERT_MSG(!_rResultSet.IsNull(_iIndex),wxT("BindFrom must not be called on null column!"));
    *m_pDataPtr = _rResultSet.GetDate(_iIndex).date();
}

template<> void ETKSQLite3ValueBindOther<QTime>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const
{
    wxUnusedVar(_bAddForInsertRequest);
    wxUnusedVar(_bForInsertRequest);
    _rstmt.BindTime(_iIndex,wxDateTime(*m_pDataPtr));
}

template<> void ETKSQLite3ValueBindOther<QTime>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
{
    wxASSERT_MSG(!_rResultSet.IsNull(_iIndex), wxT("BindFrom must not be called on null column!"));
    *m_pDataPtr = _rResultSet.GetTime(_iIndex).time();
}

template<> void ETKSQLite3ValueBindOther<QDateTime>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const
{
    wxUnusedVar(_bAddForInsertRequest);
    wxUnusedVar(_bForInsertRequest);
    _rstmt.BindDateTime(_iIndex,wxDateTime(*m_pDataPtr));
}

template<> void ETKSQLite3ValueBindOther<QDateTime>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
{
    wxASSERT_MSG(!_rResultSet.IsNull(_iIndex), wxT("BindFrom must not be called on null column!"));
    *m_pDataPtr = _rResultSet.GetDateTime(_iIndex);
}
