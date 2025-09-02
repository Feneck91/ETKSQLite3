/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/dynarray.cpp
 * @brief Implementation of wxArrayPtrVoid class from std:::vector.
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
#include <wx/dynarray.h>

size_t wxArrayPtrVoid::GetCount() const
{
    return size();
}

size_t wxArrayPtrVoid::Add(void *pItem)
{
    this->push_back(pItem);
    return size() - 1;
}

