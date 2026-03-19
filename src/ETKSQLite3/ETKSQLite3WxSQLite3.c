/////////////////////////////////////////////////////////////////////////////
// Name:        ETKSQLite3WxSQLite3.h
// Library:     ETKSQLite3
// Purpose:     Code to compile SQLite3 into library
// Author:      Stéphane Château
// Modified by:
// Created:     2011/07/31
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER // Only for MSVC compiler
    #pragma warning(push) // Backup actual warning parameters
    #pragma warning(disable : 4996)
    #pragma warning(disable : 4457)
    #pragma warning(disable : 4013)
    #pragma warning(disable : 4251)
#elif defined(__GNUC__) || defined(__clang__) // GCC/Clang (Linux/macOS)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wconversion"
    #pragma GCC diagnostic ignored "-Wunused-variable"
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    #pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
    #pragma GCC diagnostic ignored "-Wtype-limits"
    #pragma GCC diagnostic ignored "-Wsign-compare"
    #pragma GCC diagnostic ignored "-Wunused-function"
    #pragma GCC diagnostic ignored "-Wimplicit-fallthrough="
    #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
    #pragma GCC diagnostic ignored "-Wstringop-overread"
    #ifdef MAKING_ETK_SQLITE3_SHARED
        #pragma GCC diagnostic ignored "-Wattributes"
    #endif
#endif

// Duplicate define with windows.h
#if defined(_WIN32) || defined(WIN32)
    #include <windows.h>
    #ifdef FORCEINLINE
        #undef FORCEINLINE
    #endif
#endif

#include "sqlite3mc_amalgamation.c"

#ifdef _MSC_VER // Only for MSVC compiler
    #pragma warning(pop) // Restaure warning parameters
#elif defined(__GNUC__) || defined(__clang__) // GCC/Clang (Linux/macOS)
    #pragma GCC diagnostic pop
#endif
