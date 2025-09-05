/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/regex.cpp
 * @brief Implementation of regex.
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
#include <wx/regex.h>

// Constructeur avec une expression régulière et des options
wxRegEx::wxRegEx(const wxString & pattern, int flags)
{
    Compile(pattern, flags);
}

// Compile l'expression régulière
bool wxRegEx::Compile(const wxString & pattern, int flags)
{
    std::regex_constants::syntax_option_type stdFlags = std::regex_constants::ECMAScript;

    if (flags & wxRE_ICASE)
    {
        stdFlags |= std::regex_constants::icase;
    }

    try
    {
        m_regex.assign(pattern, stdFlags);
        m_isValid = true;
    }
    catch (const std::regex_error&)
    {
        m_isValid = false;
    }

    return m_isValid;
}

// Vérifie si l'expression régulière est valide
bool wxRegEx::IsValid() const
{
    return m_isValid;
}

// Vérifie si la chaîne correspond entièrement à l'expression régulière
bool wxRegEx::Matches(const wxString & text) const
{
    if (!m_isValid)
    {
        return false;
    }
    return std::regex_match(text.toStdString(), m_regex);
}
