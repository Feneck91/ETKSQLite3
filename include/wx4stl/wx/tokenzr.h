/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/tokenzr.h
 * @brief Header file for wxStringTokenizer class.
 *
 * String tokenizer - a C++ replacement for strtok(3).
 *
 * @author Stéphane Château
 * @date Created: 2014/06/14
 * @date Modified: 2025/09/02
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_TOKENZRH_HEADER
#define WX_TOKENZRH_HEADER

//#include <wx/wxtype.h>
#include <wx/string.h>
#include <wx/arrstr.h>

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// default: delimiters are usual white space characters
#define wxDEFAULT_DELIMITERS (wxT(" \t\r\n"))

// wxStringTokenizer mode flags which determine its behaviour
enum wxStringTokenizerMode
{
    wxTOKEN_INVALID = -1,   // set by def ctor until SetString() is called
    wxTOKEN_DEFAULT,        // strtok() for whitespace delims, RET_EMPTY else
    wxTOKEN_RET_EMPTY,      // return empty token in the middle of the string
    wxTOKEN_RET_EMPTY_ALL,  // return trailing empty tokens too
    wxTOKEN_RET_DELIMS,     // return the delim with token (implies RET_EMPTY)
    wxTOKEN_STRTOK          // behave exactly like strtok(3)
};

// ----------------------------------------------------------------------------
// wxStringTokenizer: replaces infamous strtok() and has some other features
// ----------------------------------------------------------------------------

class wxStringTokenizer
{
public:
    // ctors and initializers
        // default ctor, call SetString() later
    wxStringTokenizer();
        // ctor which gives us the string
    wxStringTokenizer(const wxString& str,const wxString& delims = wxDEFAULT_DELIMITERS,wxStringTokenizerMode mode = wxTOKEN_DEFAULT);

        // args are same as for the non default ctor above
    void SetString(const wxString& str,const wxString& delims = wxDEFAULT_DELIMITERS,wxStringTokenizerMode mode = wxTOKEN_DEFAULT);

        // reinitialize the tokenizer with the same delimiters/mode
    void Reinit(const wxString& str);

    // tokens access
        // return the number of remaining tokens
    size_t CountTokens() const;
        // did we reach the end of the string?
    bool HasMoreTokens() const;
        // get the next token, will return empty string if !HasMoreTokens()
    wxString GetNextToken();
        // get the delimiter which terminated the token last retrieved by
        // GetNextToken() or NUL if there had been no tokens yet or the last
        // one wasn't terminated (but ran to the end of the string)
    wxChar GetLastDelimiter() const;

    // get current tokenizer state
        // returns the part of the string which remains to tokenize (*not* the
        // initial string)
    wxString GetString() const;

        // returns the current position (i.e. one index after the last
        // returned token or 0 if GetNextToken() has never been called) in the
        // original string
    size_t GetPosition() const;

    // misc
        // get the current mode - can be different from the one passed to the
        // ctor if it was wxTOKEN_DEFAULT
    wxStringTokenizerMode GetMode() const;
        // do we return empty tokens?
    bool AllowEmpty() const;

    // backwards compatibility section from now on
    // -------------------------------------------

    // for compatibility only, use GetNextToken() instead
    wxString NextToken();

    // compatibility only, don't use
    void SetString(const wxString& to_tokenize,const wxString& delims,bool WXUNUSED(ret_delim));

    wxStringTokenizer(const wxString& to_tokenize,const wxString& delims,bool ret_delim);

protected:
    bool IsOk() const;

    wxString m_string,              // the string we tokenize
             m_delims;              // all possible delimiters

    size_t   m_pos;                 // the current position in m_string

    wxStringTokenizerMode m_mode;   // see wxTOKEN_XXX values

    wxChar   m_lastDelim;           // delimiter after last token or '\0'
};

#endif // WX_TOKENZRH_HEADER