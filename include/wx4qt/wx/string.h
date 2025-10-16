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
#include <memory>
#include <QString>

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

class wxSharedBuffer : public std::enable_shared_from_this<wxSharedBuffer>
{
private:
    QByteArray m_ByteArray;

public:
    EXPORT_IMPORT wxSharedBuffer() = default;
    EXPORT_IMPORT void setByteArray(const QByteArray & _qBytArray);
    EXPORT_IMPORT const char* getConstData() const;
};

class wxCharBuffer
{
public:
    EXPORT_IMPORT wxCharBuffer();
    EXPORT_IMPORT wxCharBuffer(wxSharedBuffer *pSharedBuffer);
    EXPORT_IMPORT wxCharBuffer(const char *_pszString);
    EXPORT_IMPORT void operator = (const QByteArray & _qBytArray);
    EXPORT_IMPORT const char* c_str() const;
    EXPORT_IMPORT operator const char*() const;
    EXPORT_IMPORT std::shared_ptr<wxSharedBuffer> & getSharedBuffer();
    EXPORT_IMPORT bool isInit() const;
private:
    std::shared_ptr<wxSharedBuffer>         m_pSharedBuffer;
};

/**
 * wxString wrapper class.
 */
class EXPORT_IMPORT wxString : public QString
{
private:
    mutable wxCharBuffer m_convertToChar; // Used to convert to const char *

public:
    static const size_t npos;
public:
    wxString();
    wxString(const wxString &_strString);
    wxString(const QString &_strString);
    wxString(const char *_pszString);
    wxString(wxChar chPad, size_t nRepeat);

    int                 Cmp(const QString &_strString) const;
    int                 Length() const;
    char                operator[](int _iPosition) const;
    wxString &          operator<<(const wxString &_rstrString);
    operator const char *() const;
    int                 Len() const;
    void                ToLong(long *_pLong) const;
    bool                IsEmpty() const;
    bool                IsSameAs(const wxString &_rstrString, bool bIsCaseSensitive = true) const;
    wxCharBuffer        ToUTF8() const;
    static wxString     FromUTF8(const char *pszText,size_t _stLength);
    static wxString     FromUTF8(const char *_pszString);
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
};

bool wxIsspace(wxChar _cChar);

#endif // WX_STRING_HEADER
