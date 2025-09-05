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
    wxCharBuffer();
    wxCharBuffer(const char *pData, size_t size = -1);
    operator const char *() const;
};


/**
 * wxString wrapper class.
 */
class EXPORT_IMPORT wxString : public std::string
{
private:
    mutable wxByteArray  m_convertToChar; // Used to convert to const char *
public:
    static const size_t  npos;
public:
    // Constructors
    wxString();
    wxString(const wxString &_strString);
    wxString(const std::string &_strString);
    wxString(const char *_pszString);
    wxString(wxChar chPad, size_t nRepeat);

    int                 Cmp(const std::string &_strString) const;
    size_t              Length() const;
    char                operator[](int _iPosition) const;
    wxString &          operator<<(const wxString &_rstrString);
    operator const char *() const;
    int                 Len() const;
    void                ToLong(long *_pLong) const;
    bool                IsEmpty() const;
    bool                IsSameAs(const wxString &_rstrString, bool _bIsCaseSensitive = true) const;
    wxCharBuffer        ToUTF8() const;
    static wxString     FromUTF8(const char *pszText,size_t _stLength = std::string::npos);
    int                 Find(wxChar ch) const;
    wxString            Left(size_t count) const;
    wxString            Right(size_t count) const;
    const wxChar *      wx_str() const;
    const wxChar *      c_str() const;
    const wxChar *      utf8_str() const;
    wxString            SubString(size_t nStart = 0, size_t nLen = -1) const;
    void                Replace(const wxChar *_pstrBefore,const wxChar *_pstrAfter);
    // Adds count copies of chPad to the beginning, or to the end of the string (the default).
    wxString &          Pad (size_t count, wxChar chPad= ' ', bool fromRight=true);
    // find first/last occurence of any character not in the set
    size_t              find_first_not_of(const wxChar* sz, size_t nStart = 0) const;
    // find first/last occurence of any character in the set
    size_t              find_first_of(const wxChar* sz, size_t nStart) const;
    wxString            Printf(const char *_pszFormat,...);
    static wxString     Format(const char *_pszFormat,...);
    static wxString     number(int64_t value);
};

bool wxIsspace(wxChar _cChar);

#endif // WX_STRING_HEADER
