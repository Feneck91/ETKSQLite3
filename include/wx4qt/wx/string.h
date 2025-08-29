/////////////////////////////////////////////////////////////////////////////
// Name:        wx4qt/string.h
// Purpose:     String implementation
// Library:     Inspired from wxWidgets, modifyed to be used with QT library
// Author:      Stéphane Château
// Modified by: 29/08/2025
// Created:     14/06/2012
// Copyright:   (c) Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_STRING_HEADER
#define WX_STRING_HEADER

#include "wxtype.h"
#include <QString>

#ifdef _UNICODE
    #ifndef UNICODE
        #define UNICODE
    #endif // UNICODE
#endif // _UNICODE

#define wxChar                                  char
#define wxCharBuffer                            wxQByteArray
#define wxString                                wxQString
#define wxEmptyString                           wxQString()
#define wxNOT_FOUND (-1)

#define _T(x)               x
#define wxS(x)              x

using wxQByteArray = QByteArray;

/**
 * wxString wrapper class.
 */
class EXPORT_IMPORT wxString : public QString
{
public:
    static const size_t  npos;
    mutable QByteArray m_convertToChar; // Used to convert to const char *
public:
    wxQString()                                                                         { }
    wxQString(const wxQString &_strString) : QString(_strString)                        { }
    wxQString(const QString &_strString)   : QString(_strString)                        { }
    wxQString(const char *_pszString)      : QString(_pszString)                        { }
    wxQString(size_t count, wxChar chPad)                                               { Pad(count,chPad); }

    int                 Cmp(const QString &_strString) const                            { return compare(_strString); }
    int                 Length() const                                                  { return length(); }
    char                operator[](int _iPosition) const                                { return (QString::operator[](_iPosition)).toLatin1(); }
    wxQString &         operator<<(const wxQString &_rstrString)                        { QString::append(_rstrString); return *this; }
    operator const char *() const;
    int                 Len() const                                                     { return length(); }
    void                ToLong(long *_pLong) const                                      { *_pLong = toLong(); }
    bool                IsEmpty() const                                                 { return QString::isEmpty(); }
    bool                IsSameAs(const wxQString &_rstrString, bool bIsCaseSensitive = true) const
                                                                                        { return 0 == QString::compare(_rstrString, bIsCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive); }
    wxCharBuffer        ToUTF8() const                                                  { return toUtf8(); }
    static wxString     FromUTF8(const char *pszText,size_t _stLength)                  { return QString::fromUtf8(pszText, static_cast<int>(_stLength)); }
    static wxQString    FromUTF8(const char *_pszString)                                { return QString::fromUtf8(_pszString); }
    int                 Find(wxChar ch) const                                           { return QString::indexOf(ch); }
    wxString            Left(size_t count) const                                        { return QString::left(static_cast<int>(count)); }
    wxString            Right(size_t count) const                                       { return QString::right(static_cast<int>(count)); }
    const wxChar *      wx_str() const;
    const wxChar *      c_str() const;
    const wxChar *      utf8_str() const;
    wxString            SubString(size_t nStart = 0, size_t nLen = -1) const            { return wxString(QString::midRef(static_cast<int>(nStart), static_cast<int>(nLen)).toString()); }
    void                Replace(const wxChar *_pstrBefore,const wxChar *_pstrAfter)     { replace(QString(_pstrBefore), QString(_pstrAfter)); }
    // Adds count copies of chPad to the beginning, or to the end of the string (the default).
    wxString &          Pad (size_t count, wxChar chPad= ' ', bool fromRight=true)      { operator=(fromRight ? leftJustified(static_cast<int>(count), chPad) : rightJustified(static_cast<int>(count), chPad)); return *this; }
    // find first/last occurence of any character not in the set
    size_t              find_first_not_of(const wxChar* sz, size_t nStart = 0) const;
    // find first/last occurence of any character in the set
    size_t              find_first_of(const wxChar* sz, size_t nStart) const;
    QString             Printf(const char *_pszFormat,...);
    static wxQString    Format(const char *_pszFormat,...);
};

#endif // WX_STRING_HEADER
