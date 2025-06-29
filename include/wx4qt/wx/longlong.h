/////////////////////////////////////////////////////////////////////////////
// Name:        wx/longlong.h
// Purpose:     Contains wxLongLong class of wxWidgets
// Library:     Copied from wxWidgets, modifyed to be used with QT library
// Author:      Stéphane Château
// Modified by:
// Created:     14/06/2012
// Copyright:   (c) Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_LONG_LONG_HEADER
#define WX_LONG_LONG_HEADER

#include "wxtype.h"
#include "string.h"

#define wxLongLong                              wxQLongLong

class EXPORT_IMPORT wxQLongLong
{
private:
    qint64 m_i64Value;

public:
    wxQLongLong() : m_i64Value(0)                                                   { }
    wxQLongLong(qint64 _i64Value) : m_i64Value(_i64Value)                           { }
    wxQLongLong(int _iValue) : m_i64Value(_iValue)                                  { }
    wxQLongLong(long _lValue) : m_i64Value(_lValue)                                 { }
    wxQLongLong(unsigned long _ulValue) : m_i64Value(_ulValue)                      { }
    wxQLongLong(const wxQLongLong &_rValue)    : m_i64Value(_rValue.GetValue())     { }
    qint64 GetValue() const                                                         { return m_i64Value; }
    operator const qint64 &() const                                                 { return m_i64Value; }

    const wxQLongLong    operator+(wxQLongLong &_rValue)                            { return wxQLongLong(GetValue() + _rValue.GetValue()); }
    const wxQLongLong    operator-(wxQLongLong &_rValue)                            { return wxQLongLong(GetValue() - _rValue.GetValue()); }
    const wxQLongLong    operator*(wxQLongLong &_rValue)                            { return wxQLongLong(GetValue() * _rValue.GetValue()); }
    const wxQLongLong    operator/(wxQLongLong &_rValue)                            { return wxQLongLong(GetValue() / _rValue.GetValue()); }
    const wxQLongLong & operator+=(wxQLongLong &_rValue)                            { m_i64Value+=_rValue.GetValue(); return *this; }
    const wxQLongLong & operator-=(wxQLongLong &_rValue)                            { m_i64Value-=_rValue.GetValue(); return *this; }
    const wxQLongLong & operator*=(wxQLongLong &_rValue)                            { m_i64Value*=_rValue.GetValue(); return *this; }
    const wxQLongLong & operator/=(wxQLongLong &_rValue)                            { m_i64Value/=_rValue.GetValue(); return *this; }

    const wxQLongLong    operator+(int _iValue)                                     { return wxQLongLong(GetValue() + ((qint64) _iValue)); }
    const wxQLongLong    operator-(int _iValue)                                     { return wxQLongLong(GetValue() - ((qint64) _iValue)); }
    const wxQLongLong    operator*(int _iValue)                                     { return wxQLongLong(GetValue() * ((qint64) _iValue)); }
    const wxQLongLong    operator/(int _iValue)                                     { return wxQLongLong(GetValue() / ((qint64) _iValue)); }
    const wxQLongLong & operator+=(int _iValue)                                     { m_i64Value+=((qint64) _iValue); return *this; }
    const wxQLongLong & operator-=(int _iValue)                                     { m_i64Value-=((qint64) _iValue); return *this; }
    const wxQLongLong & operator*=(int _iValue)                                     { m_i64Value*=((qint64) _iValue); return *this; }
    const wxQLongLong & operator/=(int _iValue)                                     { m_i64Value/=((qint64) _iValue); return *this; }

    const wxQLongLong    operator-()                                                { return wxQLongLong(-GetValue()); }
    bool                operator==(const wxQLongLong &_rValue) const                { return GetValue() == _rValue.GetValue(); }
    wxString            ToString() const                                            { return wxString(QString::number(GetValue())); }
};

#endif // WX_LONG_LONG_HEADER
