/////////////////////////////////////////////////////////////////////////////
// Name:        wx/datetime.h
// Purpose:     wxDateTime class
// Library:     Copied from wxWidgets, modifyed to be used with QT library
// Author:      Stéphane Château
// Modified by:
// Created:     14/06/2012
// Copyright:   (c) Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_DYNARRAY_HEADER
#define WX_DYNARRAY_HEADER

#include "wxETKSQLite3Types.h"
#include <iostream>
#include <vector>

class EXPORT_IMPORT wxArrayPtrVoid : public std::vector<void*>
{
public:
    size_t GetCount() const
    {
        return size();
    }

    size_t Add(void *pItem)
    {
        this->push_back(pItem);
        return size() - 1;
    }
};

#endif // WX_DYNARRAY_HEADER
