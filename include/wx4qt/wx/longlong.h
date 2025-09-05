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

#define wxLongLong                              wxQLongLong

class EXPORT_IMPORT wxQLongLong
{
private:
    int64_t m_i64Value;

public:
    wxQLongLong();
    wxQLongLong(int64_t _i64Value);
    wxQLongLong(int _iValue);
    wxQLongLong(long _lValue);
    wxQLongLong(unsigned long _ulValue);
    wxQLongLong(const wxQLongLong &_rValue);
    int64_t GetValue() const;
    operator const int64_t &() const;

    const wxQLongLong    operator+(wxQLongLong &_rValue);
    const wxQLongLong    operator-(wxQLongLong &_rValue);
    const wxQLongLong    operator*(wxQLongLong &_rValue);
    const wxQLongLong    operator/(wxQLongLong &_rValue);
    const wxQLongLong & operator+=(wxQLongLong &_rValue);
    const wxQLongLong & operator-=(wxQLongLong &_rValue);
    const wxQLongLong & operator*=(wxQLongLong &_rValue);
    const wxQLongLong & operator/=(wxQLongLong &_rValue);

    const wxQLongLong    operator+(int _iValue);
    const wxQLongLong    operator-(int _iValue);
    const wxQLongLong    operator*(int _iValue);
    const wxQLongLong    operator/(int _iValue);
    const wxQLongLong & operator+=(int _iValue);
    const wxQLongLong & operator-=(int _iValue);
    const wxQLongLong & operator*=(int _iValue);
    const wxQLongLong & operator/=(int _iValue);

    const wxQLongLong    operator-();
    bool                operator==(const wxQLongLong &_rValue) const;
    wxString            ToString() const;
};

#endif // WX_LONG_LONG_HEADER
