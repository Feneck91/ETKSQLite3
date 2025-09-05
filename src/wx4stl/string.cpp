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

wxCharBuffer::wxCharBuffer()
{
}

wxCharBuffer::wxCharBuffer(const char *pData, size_t size)
{
    if (size == static_cast<size_t>(-1))
    {
        // If size not specified, compute string lenghte
        size = std::strlen(pData);
    }
    // Alloue l'espace et copie les données
    resize(size);
    if (size > 0)
    {
        std::memcpy(data(), pData, size);
    }
}


wxCharBuffer::operator const char *() const
{
    return empty() ? "" : data();
}

#ifdef UNICODE
    #define wxStrlen_   wcslen
#else
    #define wxStrlen_   strlen
#endif

/* safe version of strlen() (returns 0 if passed NULL pointer) */
size_t wxStrlen(const wxChar *psz)
{
    return psz != nullptr ? strlen(psz) : 0;
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                               wxString                                                //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
wxString::wxString()
{
}

wxString::wxString(const wxString &_strString)
    : std::string(_strString)
{
}

wxString::wxString(const std::string &_strString)
    : std::string(_strString)
{
}

wxString::wxString(const char *_pszString)
    : std::string(_pszString)
{
}


wxString::wxString(wxChar chPad, size_t nRepeat)
    : std::string(nRepeat, chPad)
{
}

int wxString::Cmp(const std::string &_strString) const
{
    return compare(_strString);
}

size_t wxString::Length() const
{
    return length();
}

char wxString::operator[](int _iPosition) const
{
    return std::string::operator[](_iPosition); //).toLatin1();
}

wxString & wxString::operator<<(const wxString &_rstrString)
{
    std::string::append(_rstrString);
    return *this;
}

wxString::operator const char *() const
{
    m_convertToChar = ToUTF8();
    return m_convertToChar.data();
}

int wxString::Len() const
{
    return static_cast<int>(length());
}

void wxString::ToLong(long *_pLong) const
{
    if (_pLong != nullptr)
    {
        try
        {
            size_t pos = 0;
            long value = std::stol(c_str(), &pos);
            // Vérifie si toute la chaîne a été convertie
            if (pos == length())
            {
                *_pLong =  value; // Retourne 0 en cas d'erreur de conversion (comme wxWidgets)
            }
        }
        catch (const std::invalid_argument&)
        {
        }
        catch (const std::out_of_range&)
        {
        }
    }
}

bool wxString::IsEmpty() const
{
    return std::string::empty();
}

bool wxString::IsSameAs(const wxString &_rstrString, bool _bIsCaseSensitive) const
{
    if (_bIsCaseSensitive)
    {
        return *this == _rstrString;
    }
    else
    {
        if (length() != _rstrString.length())
        {
            return false;
        }
        for (int i = 0; i < length(); ++i)
        {
            if (tolower(static_cast<unsigned char>((*this)[i])) != tolower(static_cast<unsigned char>(_rstrString[i])))
            {
                return false;
            }
        }
        return true;
    }
}

wxCharBuffer wxString::ToUTF8() const
{
    wxCharBuffer buffer;
    buffer.assign(begin(), end());
    return buffer;
}

wxString wxString::FromUTF8(const char *pszText,size_t _stLength)
{
    if (pszText == nullptr)
    {
        return wxString();
    }

    if (_stLength == std::string::npos)
    {
        return wxString(pszText);
    }
    // else
    return std::string(pszText, _stLength);
}

int wxString::Find(wxChar ch) const
{
    return static_cast<int>(this->find(ch));
}

wxString wxString::Left(size_t count) const
{
    return (count >= this->length())
        ? *this
        : substr(0, count);
}

wxString wxString::Right(size_t count) const
{
    return (count >= length())
        ? *this
        : this->substr(length() - count, count);
}

const wxChar *wxString::wx_str() const
{
    return c_str();
}

const wxChar *wxString::c_str() const
{
    return std::string::c_str();
}

const wxChar *wxString::utf8_str() const
{
    return c_str();
}

wxString wxString::SubString(size_t nStart, size_t nLen) const
{
    return substr(nStart, nLen);
}

void wxString::Replace(const wxChar *_pstrBefore,const wxChar *_pstrAfter)
{
    if (_pstrBefore && *_pstrBefore)
    {
        std::string beforeStr(_pstrBefore);
        std::string afterStr(_pstrAfter != nullptr ? _pstrAfter : "");

        size_t pos = 0;
        while ((pos = find(beforeStr, pos)) != std::string::npos)
        {
            replace(pos, beforeStr.length(), afterStr);
            pos += afterStr.length();
        }
    }
}

// Adds count copies of chPad to the beginning, or to the end of the string (the default).
wxString & wxString::Pad(size_t count, wxChar chPad, bool fromRight)
{
    if (length() < count)
    {
        size_t paddingLength = count - length();
        std::string padding(paddingLength, chPad);

        if (fromRight)
        {
            *this += padding;   // Add padding to the right
        }
        else
        {
            insert(0, padding); // Add padding to the left
        }
    }

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

    return (i == (size_t) this->length())
        ? npos
        : i;
}

wxString wxString::Printf(const char* _pszFormat, ...)
{
    va_list args;
    va_start(args, _pszFormat);

    // Compute size needed for the buffer
    va_list argsCopy;
    va_copy(argsCopy, args);
    int iSize = vsnprintf(nullptr, 0, _pszFormat, argsCopy);
    va_end(argsCopy);

    if (iSize < 0)
    {
        va_end(args);
        return wxEmptyString; // Format error
    }

    // Allocate temporary buffer
    std::vector<char> buffer(iSize + 1);

    // Format the string
    vsnprintf(buffer.data(), buffer.size(), _pszFormat, args);
    va_end(args);

    // Assign formatted string
    assign(buffer.data(), iSize);

    return *this;
}

wxString wxString::Format(const char *_pszFormat,...)
{
    va_list args;
    va_start(args, _pszFormat);

    // Compute size needed for the buffer
    va_list argsCopy;
    va_copy(argsCopy, args);
    int iSize = vsnprintf(nullptr, 0, _pszFormat, argsCopy);
    va_end(argsCopy);

    if (iSize < 0)
    {
        va_end(args);
        return wxEmptyString; // Format error
    }

    // Allocate temporary buffer
    std::vector<char> buffer(iSize + 1);

    // Format the string
    vsnprintf(buffer.data(), buffer.size(), _pszFormat, args);
    va_end(args);

    // Assign formatted string
    wxString strRet;
    strRet.assign(buffer.data(), iSize);

    return strRet;
}

wxString wxString::number(int64_t value)
{
    return wxString(std::to_string(value));
}
