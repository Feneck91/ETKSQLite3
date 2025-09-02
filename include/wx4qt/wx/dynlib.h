/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/dynarray.cpp
 * @brief Header of wxDynamicLibrary class.
 *
 * wxDynamicLibrary : dynamic library to load.
 *
 * @author Stéphane Château
 * @date Created: 2012/06/14
 * @date Modified: 2025/09/02
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_DYNAMIC_LIBRARY_HEADER
#define WX_DYNAMIC_LIBRARY_HEADER

#include <wx/wxtype.h>
#include <QLibrary>

#define wxDynamicLibrary    wxQDynamicLibrary

/**
 * Not exported, only internal used.
 */
class wxQDynamicLibrary : public QLibrary
{
public:
    wxQDynamicLibrary();
    bool IsLoaded() const;
    bool Load(const char *_pszFileName);
    void * GetSymbol(const char *_pszSymbol);
    bool Unload();
};

#endif // WX_DYNAMIC_LIBRARY_HEADER
