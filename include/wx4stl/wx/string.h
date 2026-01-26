/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/string.h
 * @brief Header file for wxString class.
 *
 * String implementation.
 *
 * @author Stéphane Château
 * @date Created: 2014/06/14
 * @date Modified: 2025/09/02
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_STRING_HEADER
#define WX_STRING_HEADER

#include <wx/wxtype.h>
#include <string>
#include <vector>
#include <cstdarg>
#include <codecvt>
#include <locale>

#ifdef _UNICODE
    #ifndef UNICODE
        #define UNICODE
    #endif // UNICODE
#endif // _UNICODE

#define wxChar                                  char
extern EXPORT_IMPORT const wxChar*              wxEmptyString;
#define wxNOT_FOUND (-1)

#define _T(x)                                   x
#define wxS(x)                                  x

using wxByteArray = std::vector<char>;

class wxCharBuffer : public std::vector<char>
{
public:
    EXPORT_IMPORT wxCharBuffer();
    EXPORT_IMPORT wxCharBuffer(const char *pData, size_t size = -1);
    EXPORT_IMPORT operator const char *() const;
};

/**
 * wxString wrapper class.
 */
class wxString : public std::string
{
private:
    mutable wxByteArray  m_convertToChar; // Used to convert to const char *
public:
    EXPORT_IMPORT static const size_t  npos;
public:
    // Constructors
    EXPORT_IMPORT wxString();
    EXPORT_IMPORT wxString(const wxString &_strString);
    EXPORT_IMPORT wxString(const std::string &_strString);
    EXPORT_IMPORT wxString(const char *_pszString);
    EXPORT_IMPORT wxString(wxChar chPad, size_t nRepeat);

    EXPORT_IMPORT int                 Cmp(const std::string &_strString) const;
    EXPORT_IMPORT size_t              Length() const;
    EXPORT_IMPORT char                operator[](int _iPosition) const;
    EXPORT_IMPORT wxString &          operator<<(const wxString &_rstrString);
    EXPORT_IMPORT wxString&           operator=(const wxString &) = default;
    EXPORT_IMPORT operator const char *() const;
    EXPORT_IMPORT int                 Len() const;
    EXPORT_IMPORT void                ToLong(long *_pLong) const;
    EXPORT_IMPORT bool                IsEmpty() const;
    EXPORT_IMPORT bool                IsSameAs(const wxString &_rstrString, bool _bIsCaseSensitive = true) const;
    EXPORT_IMPORT wxCharBuffer        ToUTF8() const;
    EXPORT_IMPORT static wxString     FromUTF8(const char *pszText,size_t _stLength = std::string::npos);
    EXPORT_IMPORT int                 Find(wxChar ch) const;
    EXPORT_IMPORT wxString            Left(size_t count) const;
    EXPORT_IMPORT wxString            Right(size_t count) const;
    EXPORT_IMPORT const wxChar *      wx_str() const;
    EXPORT_IMPORT const wxChar *      c_str() const;
    EXPORT_IMPORT const wxChar *      utf8_str() const;
    EXPORT_IMPORT wxString            SubString(size_t nStart = 0, size_t nLen = -1) const;
    EXPORT_IMPORT void                Replace(const wxChar *_pstrBefore,const wxChar *_pstrAfter);
    // Adds count copies of chPad to the beginning, or to the end of the string (the default).
    EXPORT_IMPORT wxString &          Pad (size_t count, wxChar chPad= ' ', bool fromRight=true);
    // find first/last occurence of any character not in the set
    EXPORT_IMPORT size_t              find_first_not_of(const wxChar* sz, size_t nStart = 0) const;
    // find first/last occurence of any character in the set
    EXPORT_IMPORT size_t              find_first_of(const wxChar* sz, size_t nStart) const;
    EXPORT_IMPORT wxString            Printf(const char *_pszFormat,...);
    EXPORT_IMPORT static wxString     Format(const char *_pszFormat,...);
    EXPORT_IMPORT static wxString     number(int64_t value);
};

bool wxIsspace(wxChar _cChar);

#endif // WX_STRING_HEADER
