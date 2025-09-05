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

wxDynamicLibrary::wxDynamicLibrary()
    : m_handle(nullptr)
{
}

wxDynamicLibrary::~wxDynamicLibrary()
{
    Unload();
}

bool wxDynamicLibrary::IsLoaded() const
{
    return m_handle != nullptr;
}

#if defined(WIN32)
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                               Windows                                                 //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>

bool wxDynamicLibrary::Load(const char *_pszFileName)
{
    if (IsLoaded())
    {
        Unload();
    }

    m_handle = LoadLibraryA(_pszFileName);
    return m_handle != nullptr;
}

void *wxDynamicLibrary::GetSymbol(const char *_pszSymbol)
{
    if (!IsLoaded())
    {
        return nullptr;
    }

    return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(m_handle), _pszSymbol));
}

bool wxDynamicLibrary::Unload()
{
    if (!IsLoaded())
    {
        return false;
    }

    bool result = FreeLibrary(static_cast<HMODULE>(m_handle)) != 0;
    m_handle = nullptr;
    return result;
}

#else
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                             Linux/macOS                                               //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <dlfcn.h>

#include "wxdynamiclibrary.h"
#include <dlfcn.h>
bool wxDynamicLibrary::Load(const char *_pszFileName)
{
    if (IsLoaded())
    {
        Unload();
    }

    m_handle = dlopen(_pszFileName, RTLD_LAZY);
    return m_handle != nullptr;
}

void *wxDynamicLibrary::GetSymbol(const char *_pszSymbol)
{
    if (!IsLoaded())
    {
        return nullptr;
    }

    return dlsym(m_handle, _pszSymbol);
}

bool wxDynamicLibrary::Unload()
{
    if (!IsLoaded())
    {
        return false;
    }

    bool result = dlclose(m_handle) == 0;
    m_handle = nullptr;
    return result;
}

#endif
