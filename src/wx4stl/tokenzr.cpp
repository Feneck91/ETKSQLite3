/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/tokenzr.cpp
 * @brief Implementation file for wxStringTokenizer class.
 *
 * String tokenizer - a C++ replacement for strtok(3).
 *
 * @author Stéphane Château
 * @date Created: 2025/09/02
 * @date Modified:
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#include <wx/tokenzr.h>

// ----------------------------------------------------------------------------
// wxStringTokenizer: replaces infamous strtok() and has some other features
// ----------------------------------------------------------------------------
// ctors and initializers
    // default ctor, call SetString() later
wxStringTokenizer::wxStringTokenizer()
{
    m_mode = wxTOKEN_INVALID;
}

// ctor which gives us the string
wxStringTokenizer::wxStringTokenizer(const wxString& str,
                                     const wxString& delims,
                                     wxStringTokenizerMode mode)
{
    SetString(str, delims, mode);
}

// args are same as for the non default ctor above
void wxStringTokenizer::SetString(const wxString& str,
                                  const wxString& delims,
                                  wxStringTokenizerMode mode)
{
    if ( mode == wxTOKEN_DEFAULT )
    {
        // by default, we behave like strtok() if the delimiters are only
        // whitespace characters and as wxTOKEN_RET_EMPTY otherwise (for
        // whitespace delimiters, strtok() behaviour is better because we want
        // to count consecutive spaces as one delimiter)
        const wxChar *p;
        for ( p = delims.c_str(); *p; p++ )
        {
            if ( !wxIsspace(*p) )
                break;
        }

        if ( *p )
        {
            // not whitespace char in delims
            mode = wxTOKEN_RET_EMPTY;
        }
        else
        {
            // only whitespaces
            mode = wxTOKEN_STRTOK;
        }
    }

    m_delims = delims;
    m_mode = mode;

    Reinit(str);
}

// reinitialize the tokenizer with the same delimiters/mode
void wxStringTokenizer::Reinit(const wxString& str)
{
    wxASSERT( IsOk() ); // you should call SetString() first

    m_string = str;
    m_pos = 0;
    m_lastDelim = wxT('\0');
}

// tokens access
// count the number of (remaining) tokens in the string
size_t wxStringTokenizer::CountTokens() const
{
    STL_ASSERT(IsOk()); // you should call SetString() first

    // VZ: this function is IMHO not very useful, so it's probably not very
    //     important if its implementation here is not as efficient as it
    //     could be -- but OTOH like this we're sure to get the correct answer
    //     in all modes
    wxStringTokenizer tkz(m_string.c_str() + m_pos, m_delims, m_mode);

    size_t count = 0;
    while ( tkz.HasMoreTokens() )
    {
        count++;

        (void)tkz.GetNextToken();
    }

    return count;
}

// did we reach the end of the string?
// do we have more of them?
bool wxStringTokenizer::HasMoreTokens() const
{
    STL_ASSERT(IsOk()); // you should call SetString() first

    if ( m_string.find_first_not_of(m_delims, m_pos) != wxString::npos )
    {
        // there are non delimiter characters left, so we do have more tokens
        return true;
    }

    switch ( m_mode )
    {
        case wxTOKEN_RET_EMPTY:
        case wxTOKEN_RET_DELIMS:
            // special hack for wxTOKEN_RET_EMPTY: we should return the initial
            // empty token even if there are only delimiters after it
            return m_pos == 0 && !m_string.IsEmpty();

        case wxTOKEN_RET_EMPTY_ALL:
            // special hack for wxTOKEN_RET_EMPTY_ALL: we can know if we had
            // already returned the trailing empty token after the last
            // delimiter by examining m_lastDelim: it is set to NUL if we run
            // up to the end of the string in GetNextToken(), but if it is not
            // NUL yet we still have this last token to return even if m_pos is
            // already at m_string.length()
            return m_pos < (size_t) m_string.length() || m_lastDelim != wxT('\0');

        case wxTOKEN_INVALID:
        case wxTOKEN_DEFAULT:
            wxFAIL_MSG( wxT("unexpected tokenizer mode") );
            // fall through

        case wxTOKEN_STRTOK:
            // never return empty delimiters
            break;
    }

    return false;
}

// ----------------------------------------------------------------------------
// token extraction
// ----------------------------------------------------------------------------
// get the next token, will return empty string if !HasMoreTokens()
wxString wxStringTokenizer::GetNextToken()
{
    wxString token;
    do
    {
        if ( !HasMoreTokens() )
        {
            break;
        }

        // find the end of this token
        size_t pos = m_string.find_first_of(m_delims, m_pos);

        // and the start of the next one
        if ( pos == wxString::npos )
        {
            // no more delimiters, the token is everything till the end of
            // string
            token = m_string.SubString(m_pos, wxString::npos);

            // skip the token
            m_pos = m_string.length();

            // it wasn't terminated
            m_lastDelim = wxT('\0');
        }
        else // we found a delimiter at pos
        {
            // in wxTOKEN_RET_DELIMS mode we return the delimiter character
            // with token, otherwise leave it out
            size_t len = pos - m_pos;
            if ( m_mode == wxTOKEN_RET_DELIMS )
                len++;

            token = m_string.SubString(m_pos, len);

            // skip the token and the trailing delimiter
            m_pos = pos + 1;

            m_lastDelim = m_string[static_cast<int>(pos)];
        }
    }
    while ( !AllowEmpty() && token.IsEmpty() );

    return token;
}

// get the delimiter which terminated the token last retrieved by
// GetNextToken() or NUL if there had been no tokens yet or the last
// one wasn't terminated (but ran to the end of the string)
wxChar wxStringTokenizer::GetLastDelimiter() const
{
    return m_lastDelim;
}

// get current tokenizer state
// returns the part of the string which remains to tokenize (*not* the
// initial string)
wxString wxStringTokenizer::GetString() const
{
    return m_string.SubString(m_pos);
}

// returns the current position (i.e. one index after the last
// returned token or 0 if GetNextToken() has never been called) in the
// original string
size_t wxStringTokenizer::GetPosition() const
{
    return m_pos;
}

// misc
// get the current mode - can be different from the one passed to the
// ctor if it was wxTOKEN_DEFAULT
wxStringTokenizerMode wxStringTokenizer::GetMode() const
{
    return m_mode;
}

// do we return empty tokens?
bool wxStringTokenizer::AllowEmpty() const
{
    return m_mode != wxTOKEN_STRTOK;
}

// backwards compatibility section from now on
// -------------------------------------------

// for compatibility only, use GetNextToken() instead
wxString wxStringTokenizer::NextToken()
{
    return GetNextToken();
}

// compatibility only, don't use
void wxStringTokenizer::SetString(const wxString& to_tokenize,const wxString& delims,bool WXUNUSED(ret_delim))
{
    SetString(to_tokenize, delims, wxTOKEN_RET_DELIMS);
}

wxStringTokenizer::wxStringTokenizer(const wxString& to_tokenize,const wxString& delims,bool ret_delim)
{
    SetString(to_tokenize, delims, ret_delim);
}

bool wxStringTokenizer::IsOk() const
{
    return m_mode != wxTOKEN_INVALID;
}
