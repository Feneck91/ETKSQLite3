/////////////////////////////////////////////////////////////////////////////
// Name:        wx/wx.h
// Purpose:     Contains general constants of wxWidgets
// Library:     Copied from wxWidgets, modifyed to be used with QT library
// Author:      Stéphane Château
// Modified by:
// Created:     14/06/2012
// Copyright:   (c) Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_HEADER
#define WX_HEADER

#include <iostream>
#define wxCHECK_VERSION(major,minor,build)      0

#define wxLongLong                              wxQLongLong
#define wxT(x)                                  x
#define _(x)                                    x
#define wxTRANSLATE(x)                          x
#define wxGetTranslation(x)                     x
#define __TFILE__                               __FILE__
#define STL_ASSERT_X(cond, context, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "Assertion failed: " << #cond << ", " \
                      << (context).ToUTF8().data() << ", " \
                      << (msg) << std::endl; \
            std::abort(); \
        } \
    } while (false)
#define STL_ASSERT(cond) \
    do { \
        if (!(cond)) { \
            std::abort(); \
        } \
    } while (false)
#define wxOnAssert(file,line,fn,cond,msg)       STL_ASSERT_X((cond),(wxString::Format("%s in line %d", fn, line)), msg)
#define wxMax(a,b)                              (((a) > (b)) ? (a) : (b))
#define WXUNUSED(x)
#define DWORD                                   unsigned long
#define LPCWSTR                                 wchar_t *

/*  sometimes the value of a variable is *really* not used, to suppress  the */
/*  resulting warning you may pass it to this function */
#ifdef __cplusplus
#   ifdef __BORLANDC__
#       define wxUnusedVar(identifier) identifier
#   else
        #define wxUnusedVar(identifier) identifier
        //template <class T> inline void wxUnusedVar(const T& WXUNUSED(t)) { }
#   endif
#endif

// NB: the following macros also work in release mode!

// These macros must be used only in invalid situation: for example, an
// invalid parameter (e.g. a NULL pointer) is passed to a function. Instead of
// dereferencing it and causing core dump the function might try using
// CHECK( p != NULL ) or CHECK( p != NULL, return LogError("p is NULL!!") )

// check that expression is true, "return" if not (also FAILs in debug mode)
#define wxCHECK(cond, rc)            wxCHECK_MSG(cond, rc, NULL)

// as wxCHECK but with a message explaining why we fail
#define wxCHECK_MSG(cond, rc, msg)   wxCHECK2_MSG(cond, return rc, msg)

// check that expression is true, perform op if not */
#define wxCHECK2(cond, op)           wxCHECK2_MSG(cond, op, NULL)

// as wxCHECK2 but with a message explaining why we fail

// see comment near the definition of wxASSERT_MSG for the # if/else reason
#if defined(__MWERKS__)
    #define wxCHECK2_MSG(cond, op, msg)                                       \
        if ( cond )                                                           \
        {}                                                                    \
        else                                                                  \
        {                                                                     \
            wxFAIL_COND_MSG(#cond, msg);                                      \
            op;                                                               \
        }                                                                     \
        struct wxDummyCheckStruct /* just to force a semicolon */
#else
    #define wxCHECK2_MSG(cond, op, msg)                                       \
        if ( cond )                                                           \
            ;                                                                 \
        else                                                                  \
        {                                                                     \
            wxFAIL_COND_MSG(#cond, msg);                                      \
            op;                                                               \
        }                                                                     \
        struct wxDummyCheckStruct /* just to force a semicolon */
#endif

/*  special form of assert: always triggers it (in debug mode) */
#define wxFAIL wxFAIL_MSG(NULL)

/*  FAIL with some message */
#define wxFAIL_MSG(msg) wxFAIL_COND_MSG("wxAssertFailure", msg)

/*  FAIL with some message and a condition */
#define wxFAIL_COND_MSG(cond, msg)                                          \
    wxOnAssert(__TFILE__, __LINE__,  __WXFUNCTION__, wxT(cond), msg)

// special form of wxCHECK2: as wxCHECK, but for use in void functions
//
// NB: there is only one form (with msg parameter) and it's intentional:
// there is no other way to tell the caller what exactly went wrong
// from the void function (of course, the function shouldn't be void
// to begin with...)
#define wxCHECK_RET(cond, msg)       wxCHECK2_MSG(cond, return, msg)

#define DECLARE_NO_COPY_CLASS(className) \
    private: \
        className(const className &);

#ifndef __WXFUNCTION__
    /* TODO: add more compilers supporting __FUNCTION__ */
    #if defined(__DMC__)
        /*
           __FUNCTION__ happens to be not defined within class members
           http://www.digitalmars.com/drn-bin/wwwnews?c%2B%2B.beta/485
        */
        #define __WXFUNCTION__ (NULL)
    #elif defined(__GNUC__) || \
          (defined(_MSC_VER) && _MSC_VER >= 1300) || \
          defined(__FUNCTION__)
        #define __WXFUNCTION__ __FUNCTION__
    #else
        /* still define __WXFUNCTION__ to avoid #ifdefs elsewhere */
        #define __WXFUNCTION__ (NULL)
    #endif
#endif /* __WXFUNCTION__ already defined */

//
// wxASSERT
//
#if defined(_DEBUG) && !defined(__WXDEBUG__)
    #define __WXDEBUG__
#endif

#ifdef DEBUG
    #define wxASSERT                STL_ASSERT
    #define wxASSERT_MSG(cond,msg)  STL_ASSERT_X(cond, "", msg);
#else
    #define wxASSERT
    #define wxASSERT_MSG(cond,msg)
#endif // DEBUG


//
// wx Macros
//
#define wxDELETE(x)      { if (x) { delete x; x=NULL; }}

#include <wx/wxtype.h>
#include <wx/string.h>
#include <wx/logger.h>
//#include <QVector>
//#include <QtGlobal>
#include <assert.h>

/*
#ifndef INCLUDE_WX_ETK_SQLITE3_VALUE_BIND_H
class wxSQLite3Statement;
class wxSQLite3ResultSet;
#include "ETKSQLite3ValueBind.h"
// Bind QT types : export all full template specialization: used to bind QT type directly without using wxWidgets wrappers out of the library
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<qint64>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<qint64>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QString>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QString>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QDate>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QDate>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QTime>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QTime>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QDateTime>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;
template<> EXPORT_IMPORT void ETKSQLite3ValueBindOther<QDateTime>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;
#endif //INCLUDE_WX_ETK_SQLITE3_VALUE_BIND_H
*/

#endif // WX_HEADER
