/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/dynarray.cpp
 * @brief Header of wxArrayPtrVoid class from std:::vector.
 *
 * wxArrayPtrVoid, simul wxWidget class to be used with other frameworks.
 *
 * @author Stéphane Château
 * @date Created: 14/06/2012
 * @date Modified: 2025/09/02
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_DYNARRAY_HEADER
#define WX_DYNARRAY_HEADER

#include <wx/wx.h>
#include <vector>

class wxArrayPtrVoid : public std::vector<void*>
{
public:
    EXPORT_IMPORT size_t GetCount() const;
    EXPORT_IMPORT size_t Add(void *pItem);
};

#endif // WX_DYNARRAY_HEADER
