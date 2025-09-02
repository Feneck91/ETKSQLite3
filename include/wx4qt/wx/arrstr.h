/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/arrstr.h
 * @brief Header file for wxArrayString.
 *
 * String array, simul wxWidget class to be used with other frameworks.
 *
 * @author Stéphane Château
 * @date Created: 2012/06/14
 * @date Modified: 2025/09/01
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_ARRSTRING_HEADER
#define WX_ARRSTRING_HEADER

#include "buffer.h"

// String array is just a wxQVector of wxString
#define wxArrayString                           wxQVector<wxString>

#endif // WX_ARRSTRING_HEADER
