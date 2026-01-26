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
    // int64_t : remove if int64_t is same as long
    template<typename T = int64_t, std::enable_if_t<!std::is_same_v<T, long>, int> = 0>
    explicit wxQLongLong(T _i64Value)
        : m_i64Value(_i64Value)
    {
    }
    wxQLongLong(int _iValue);
    wxQLongLong(long _lValue);
    wxQLongLong(time_t _tValue);
    wxQLongLong(unsigned long _ulValue);
    wxQLongLong(const wxQLongLong &_rValue);
    int64_t GetValue() const;
    operator const int64_t &() const;

    const wxQLongLong       operator+(wxQLongLong &_rValue);
    const wxQLongLong       operator-(wxQLongLong &_rValue);
    const wxQLongLong       operator*(wxQLongLong &_rValue);
    const wxQLongLong       operator/(wxQLongLong &_rValue);
    const wxQLongLong &     operator+=(wxQLongLong &_rValue);
    const wxQLongLong &     operator-=(wxQLongLong &_rValue);
    const wxQLongLong &     operator*=(wxQLongLong &_rValue);
    const wxQLongLong &     operator/=(wxQLongLong &_rValue);

    const wxQLongLong       operator+(int _iValue);
    const wxQLongLong       operator-(int _iValue);
    const wxQLongLong       operator*(int _iValue);
    const wxQLongLong       operator/(int _iValue);
    const wxQLongLong &     operator+=(int _iValue);
    const wxQLongLong &     operator-=(int _iValue);
    const wxQLongLong &     operator*=(int _iValue);
    const wxQLongLong &     operator/=(int _iValue);

    const wxQLongLong       operator-();
    bool                    operator==(const wxQLongLong &_rValue) const;
    wxQLongLong &           operator=(const wxQLongLong &_rValue);
    wxString                ToString() const;
};

#endif // WX_LONG_LONG_HEADER
