/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/regex.h
 * @brief Header of regex.
 *
 * Header class for regex class, used when wxUSE_REGEX is set.
 *
 * @author Stéphane Château
 * @date Created: 2025/09/02
 * @date Modified:
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_REGEX_HEADER
#define WX_REGEX_HEADER

#include <regex>
#include <wx/string.h>
#include <vector>

enum
{
    // use extended regex syntax
    wxRE_EXTENDED = 0,

    // use advanced RE syntax (built-in regex only)
    wxRE_ADVANCED = 1,

    // use basic RE syntax
    wxRE_BASIC    = 2,

    // ignore case in match
    wxRE_ICASE    = 4,

    // only check match, don't set back references
    wxRE_NOSUB    = 8,

    // if not set, treat '\n' as an ordinary character, otherwise it is
    // special: it is not matched by '.' and '^' and '$' always match
    // after/before it regardless of the setting of wxRE_NOT[BE]OL
    wxRE_NEWLINE  = 16,

    // default flags
    wxRE_DEFAULT  = wxRE_EXTENDED
};

class wxRegEx
{
public:
    // Constructeur par défaut
    wxRegEx() = default;

    // Constructeur avec une expression régulière et des options
    wxRegEx(const wxQString & pattern, int flags = wxRE_ADVANCED);

    // Compile l'expression régulière
    bool Compile(const wxQString & pattern, int flags = wxRE_ADVANCED);

    // Vérifie si l'expression régulière est valide
    bool IsValid() const;

    // Vérifie si la chaîne correspond entièrement à l'expression régulière
    bool Matches(const wxQString & text) const;

private:
    std::regex m_regex;
    bool m_isValid{false};
};

#endif // WX_REGEX_HEADER