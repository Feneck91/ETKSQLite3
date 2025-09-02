/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/dynarray.cpp
 * @brief Implementation of wxDynamicLibrary class.
 *
 * wxDynamicLibrary : dynamic library to load.
 *
 * @author Stéphane Château
 * @date Created: 2025/09/02
 * @date Modified:
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#include <wx/dynlib.h>

wxQDynamicLibrary::wxQDynamicLibrary()
{
}

bool wxQDynamicLibrary::IsLoaded() const
{
    return isLoaded ();
}

bool wxQDynamicLibrary::Load(const char *_pszFileName)
{
    setFileName(_pszFileName); return load();
}

void * wxQDynamicLibrary::GetSymbol(const char *_pszSymbol)
{
    return resolve(_pszSymbol);
}

bool wxQDynamicLibrary::Unload()
{
    return unload();
}
