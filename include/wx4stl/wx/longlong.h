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

#include <wx/wxtype.h>
#include <wx/string.h>

class EXPORT_IMPORT wxLongLong
{
private:
    int64_t m_i64Value;

public:
    wxLongLong();
    wxLongLong(int64_t _i64Value);
    wxLongLong(int _iValue);
    wxLongLong(long _lValue);
    wxLongLong(unsigned long _ulValue);
    wxLongLong(const wxLongLong &_rValue);
    int64_t GetValue() const;
    operator const int64_t &() const;

    const wxLongLong        operator+(wxLongLong &_rValue);
    const wxLongLong        operator-(wxLongLong &_rValue);
    const wxLongLong        operator*(wxLongLong &_rValue);
    const wxLongLong        operator/(wxLongLong &_rValue);
    const wxLongLong &      operator+=(wxLongLong &_rValue);
    const wxLongLong &      operator-=(wxLongLong &_rValue);
    const wxLongLong &      operator*=(wxLongLong &_rValue);
    const wxLongLong &      operator/=(wxLongLong &_rValue);

    const wxLongLong        operator+(int _iValue);
    const wxLongLong        operator-(int _iValue);
    const wxLongLong        operator*(int _iValue);
    const wxLongLong        operator/(int _iValue);
    const wxLongLong &      operator+=(int _iValue);
    const wxLongLong &      operator-=(int _iValue);
    const wxLongLong &      operator*=(int _iValue);
    const wxLongLong &      operator/=(int _iValue);

    const wxLongLong        operator-();
    bool                    operator==(const wxLongLong &_rValue) const;
    wxString                ToString() const;
};

#endif // WX_LONG_LONG_HEADER
