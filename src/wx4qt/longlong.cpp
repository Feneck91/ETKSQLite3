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
#include <wx/longlong.h>
#include <wx/string.h>

#define wxLongLong                              wxQLongLong

wxQLongLong::wxQLongLong()
    : m_i64Value(0)
{
}

wxQLongLong::wxQLongLong(qint64 _i64Value)
    : m_i64Value(_i64Value)
{
}

wxQLongLong::wxQLongLong(int _iValue)
    : m_i64Value(_iValue)
{
}

wxQLongLong::wxQLongLong(long _lValue)
    : m_i64Value(_lValue)
{
}

wxQLongLong::wxQLongLong(unsigned long _ulValue)
    : m_i64Value(_ulValue)
{
}

wxQLongLong::wxQLongLong(const wxQLongLong &_rValue)
    : m_i64Value(_rValue.GetValue())
{
}

qint64 wxQLongLong::GetValue() const
{
    return m_i64Value;
}

wxQLongLong::operator const qint64 &() const
{
    return m_i64Value;
}

const wxQLongLong wxQLongLong::operator+(wxQLongLong &_rValue)
{
    return wxQLongLong(GetValue() + _rValue.GetValue());
}

const wxQLongLong wxQLongLong::operator-(wxQLongLong &_rValue)
{
    return wxQLongLong(GetValue() - _rValue.GetValue());
}

const wxQLongLong wxQLongLong::operator*(wxQLongLong &_rValue)
{
    return wxQLongLong(GetValue() * _rValue.GetValue());
}

const wxQLongLong wxQLongLong::operator/(wxQLongLong &_rValue)
{
    return wxQLongLong(GetValue() / _rValue.GetValue());
}

const wxQLongLong & wxQLongLong::operator+=(wxQLongLong &_rValue)
{
    m_i64Value+=_rValue.GetValue(); return *this;
}

const wxQLongLong & wxQLongLong::operator-=(wxQLongLong &_rValue)
{
    m_i64Value-=_rValue.GetValue(); return *this; }
const wxQLongLong & wxQLongLong::operator*=(wxQLongLong &_rValue)
{
    m_i64Value*=_rValue.GetValue(); return *this; }
const wxQLongLong & wxQLongLong::operator/=(wxQLongLong &_rValue)
{
    m_i64Value/=_rValue.GetValue(); return *this; }

const wxQLongLong wxQLongLong::operator+(int _iValue)
{
    return wxQLongLong(GetValue() + ((qint64) _iValue));
}

const wxQLongLong wxQLongLong::operator-(int _iValue)
{
    return wxQLongLong(GetValue() - ((qint64) _iValue));
}

const wxQLongLong wxQLongLong::operator*(int _iValue)
{
    return wxQLongLong(GetValue() * ((qint64) _iValue));
}

const wxQLongLong wxQLongLong::operator/(int _iValue)
{ return wxQLongLong(GetValue() / ((qint64) _iValue));
}

const wxQLongLong & wxQLongLong::operator+=(int _iValue)
{
    m_i64Value+=((qint64) _iValue); return *this;
}

const wxQLongLong & wxQLongLong::operator-=(int _iValue)
{
    m_i64Value-=((qint64) _iValue); return *this;
}

const wxQLongLong & wxQLongLong::operator*=(int _iValue)
{
    m_i64Value*=((qint64) _iValue); return *this;
}

const wxQLongLong & wxQLongLong::operator/=(int _iValue)
{
    m_i64Value/=((qint64) _iValue); return *this;
}

const wxQLongLong    wxQLongLong::operator-()
{
    return wxQLongLong(-GetValue());
}

bool wxQLongLong::operator==(const wxQLongLong &_rValue) const
{
    return GetValue() == _rValue.GetValue();
}

wxString wxQLongLong::ToString() const
{
    return wxString(QString::number(GetValue()));
}
