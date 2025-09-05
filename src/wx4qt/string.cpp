/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/string.cpp
 * @brief Implementation file for wxString class.
 *
 * String implementation.
 *
 * @author Stéphane Château
 * @date Created: 2025/09/02
 * @date Modified:
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#include <wx/wx.h>
#include <wx/string.h>

// Static member
const size_t wxString::npos = (size_t) -1;

#ifdef UNICODE
    #define wxStrlen_   wcslen
#else
    #define wxStrlen_   strlen
#endif

/* safe version of strlen() (returns 0 if passed NULL pointer) */
size_t wxStrlen(const wxChar *psz)
{
    return psz ? strlen(psz) : 0;
}

//implement our own wmem variants
wxChar* wxTmemchr(const wxChar* s, wxChar c, size_t l)
{
    for(;l && *s != c;--l, ++s);

    if (l)
    {
        return (wxChar*)s;
    }

    return NULL;
}


bool wxIsspace(wxChar _cChar)
{
    return _cChar == wxT(' ');
}

wxString::wxString()
{
}

wxString::wxString(const wxString &_strString)
    : QString(_strString)
{
}

wxString::wxString(const QString &_strString)
    : QString(_strString)
{
}

wxString::wxString(const char *_pszString)
    : QString(_pszString)
{
}

wxString::wxString(wxChar chPad, size_t nRepeat)
{
    Pad(nRepeat, chPad);
}

int wxString::Cmp(const QString &_strString) const
{
    return compare(_strString);
}

int wxString::Length() const
{
    return length();
}

char wxString::operator[](int _iPosition) const
{
    return (QString::operator[](_iPosition)).toLatin1();
}

wxString & wxString::operator<<(const wxString &_rstrString)
{
    QString::append(_rstrString);
    return *this;
}

wxString::operator const char *() const
{
    m_convertToChar = toUtf8();

    return m_convertToChar;
}

int wxString::Len() const
{
    return length();
}

void wxString::ToLong(long *_pLong) const
{
    *_pLong = toLong();
}

bool wxString::IsEmpty() const
{
    return QString::isEmpty();
}

bool wxString::IsSameAs(const wxString &_rstrString, bool bIsCaseSensitive) const
{
    return 0 == QString::compare(_rstrString, bIsCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);
}

wxCharBuffer wxString::ToUTF8() const
{
    return toUtf8();
}

wxString wxString::FromUTF8(const char *pszText,size_t _stLength)
{
    return QString::fromUtf8(pszText, static_cast<int>(_stLength));
}

wxString wxString::FromUTF8(const char *_pszString)
{
    return QString::fromUtf8(_pszString);
}

int wxString::Find(wxChar ch) const
{
    return QString::indexOf(ch);
}

wxString wxString::Left(size_t count) const
{
    return QString::left(static_cast<int>(count));
}

wxString wxString::Right(size_t count) const
{
    return QString::right(static_cast<int>(count));
}

const wxChar *wxString::wx_str() const
{
    m_convertToChar = toUtf8();

    return m_convertToChar;
}

const wxChar *wxString::c_str() const
{
    m_convertToChar = toUtf8();

    return m_convertToChar;
}

const wxChar *wxString::utf8_str() const
{
    m_convertToChar = toUtf8();

    return m_convertToChar.toStdString().c_str();
}

wxString wxString::SubString(size_t nStart, size_t nLen) const
{
    return wxString(QString::midRef(static_cast<int>(nStart), static_cast<int>(nLen)).toString());
}

void wxString::Replace(const wxChar *_pstrBefore,const wxChar *_pstrAfter)
{
    replace(QString(_pstrBefore), QString(_pstrAfter));
}

// Adds count copies of chPad to the beginning, or to the end of the string (the default).
wxString & wxString::Pad (size_t count, wxChar chPad, bool fromRight)
{
    operator=(fromRight ? leftJustified(static_cast<int>(count), chPad) : rightJustified(static_cast<int>(count), chPad));
    return *this;
}

// find first/last occurence of any character not in the set
size_t wxString::find_first_not_of(const wxChar* sz, size_t nStart) const
{
    if ( nStart == npos )
    {
        nStart = length();
    }
    else
    {
        wxASSERT( nStart <= (size_t) length() );
    }

    size_t len = strlen(sz);

    size_t i;
    for(i = nStart; i < (size_t) this->length(); ++i)
    {
        if (!wxTmemchr(sz, *(c_str() + i), len))
            break;
    }

    return (i == (size_t) this->length())
        ? npos
        : i;
}

size_t wxString::find_first_of(const wxChar* sz, size_t nStart) const
{
    wxASSERT(nStart <= (size_t) length());

    size_t len = wxStrlen(sz);

    size_t i;
    for(i = nStart; i < (size_t) this->length(); ++i)
    {
        if (wxTmemchr(sz, *(c_str() + i), len))
            break;
    }

    if(i == (size_t) this->length())
        return npos;
    else
        return i;
}

wxString wxString::Printf(const char *_pszFormat,...)
{
    va_list valistArguments;
    va_start(valistArguments,_pszFormat);
    operator=(QString::vasprintf(_pszFormat,valistArguments));
    va_end(valistArguments);
    return *this;
}

wxString wxString::Format(const char *_pszFormat,...)
{
    va_list valistArguments;
    va_start(valistArguments, _pszFormat);
    QString strReturn = QString::vasprintf(_pszFormat, valistArguments);
    va_end(valistArguments);
    return strReturn;
}
