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

wxLongLong::wxLongLong()
    : m_i64Value(0)
{
}

wxLongLong::wxLongLong(int64_t _i64Value)
    : m_i64Value(_i64Value)
{
}

wxLongLong::wxLongLong(int _iValue)
    : m_i64Value(_iValue)
{
}

wxLongLong::wxLongLong(long _lValue)
    : m_i64Value(_lValue)
{
}

wxLongLong::wxLongLong(unsigned long _ulValue)
    : m_i64Value(_ulValue)
{
}

wxLongLong::wxLongLong(const wxLongLong &_rValue)
    : m_i64Value(_rValue.GetValue())
{
}

int64_t wxLongLong::GetValue() const
{
    return m_i64Value;
}

wxLongLong::operator const int64_t &() const
{
    return m_i64Value;
}

const wxLongLong wxLongLong::operator+(wxLongLong &_rValue)
{
    return wxLongLong(GetValue() + _rValue.GetValue());
}

const wxLongLong wxLongLong::operator-(wxLongLong &_rValue)
{
    return wxLongLong(GetValue() - _rValue.GetValue());
}

const wxLongLong wxLongLong::operator*(wxLongLong &_rValue)
{
    return wxLongLong(GetValue() * _rValue.GetValue());
}

const wxLongLong wxLongLong::operator/(wxLongLong &_rValue)
{
    return wxLongLong(GetValue() / _rValue.GetValue());
}

const wxLongLong & wxLongLong::operator+=(wxLongLong &_rValue)
{
    m_i64Value+=_rValue.GetValue(); return *this;
}

const wxLongLong & wxLongLong::operator-=(wxLongLong &_rValue)
{
    m_i64Value-=_rValue.GetValue(); return *this; }
const wxLongLong & wxLongLong::operator*=(wxLongLong &_rValue)
{
    m_i64Value*=_rValue.GetValue(); return *this; }
const wxLongLong & wxLongLong::operator/=(wxLongLong &_rValue)
{
    m_i64Value/=_rValue.GetValue(); return *this; }

const wxLongLong wxLongLong::operator+(int _iValue)
{
    return wxLongLong(GetValue() + ((int64_t) _iValue));
}

const wxLongLong wxLongLong::operator-(int _iValue)
{
    return wxLongLong(GetValue() - ((int64_t) _iValue));
}

const wxLongLong wxLongLong::operator*(int _iValue)
{
    return wxLongLong(GetValue() * ((int64_t) _iValue));
}

const wxLongLong wxLongLong::operator/(int _iValue)
{ return wxLongLong(GetValue() / ((int64_t) _iValue));
}

const wxLongLong & wxLongLong::operator+=(int _iValue)
{
    m_i64Value+=((int64_t) _iValue); return *this;
}

const wxLongLong & wxLongLong::operator-=(int _iValue)
{
    m_i64Value-=((int64_t) _iValue); return *this;
}

const wxLongLong & wxLongLong::operator*=(int _iValue)
{
    m_i64Value*=((int64_t) _iValue); return *this;
}

const wxLongLong & wxLongLong::operator/=(int _iValue)
{
    m_i64Value/=((int64_t) _iValue); return *this;
}

const wxLongLong    wxLongLong::operator-()
{
    return wxLongLong(-GetValue());
}

bool wxLongLong::operator==(const wxLongLong &_rValue) const
{
    return GetValue() == _rValue.GetValue();
}

wxString wxLongLong::ToString() const
{
    return wxString::number(GetValue());
}
