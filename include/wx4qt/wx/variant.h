/////////////////////////////////////////////////////////////////////////////
// Name:        wx/tokenzr.h
// Purpose:     String tokenizer - a C++ replacement for strtok(3)
// Library:     Copied from wxWidgets, modifyed to be used with QT library
// Author:      Guilhem Lavaux
// Modified by:
// Created:     14/06/2012
// Copyright:   (c) Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_VARIANT_HEADER
#define WX_VARIANT_HEADER

#include "wxtype.h"

//
// wxVariant Class
//
#include <QVariant>
#include <QDateTime>
#include "datetime.h"
#include "string.h"

class EXPORT_IMPORT wxVariantData
{
    friend class wxVariant;
public:
    wxVariantData() : m_count(0)                                                        { }

    // Override these to provide common functionality
    virtual bool Eq(wxVariantData& data) const = 0;
    virtual bool Write(wxString & WXUNUSED(str)) const                                  { return false; }
    virtual bool Read(wxString & WXUNUSED(str))                                         { return false; }
    // What type is it? Return a string name.
    virtual wxString GetType() const = 0;

    void IncRef()                                                                       { m_count++; }
    void DecRef()                                                                       { if ( --m_count == 0 ) delete this; }
    int GetRefCount() const                                                             { return m_count; }

protected:
    // Protected dtor should make some incompatible code
    // break more louder. That is, they should do data->DecRef()
    // instead of delete data.
    virtual ~wxVariantData()                                                            { }

private:
    int     m_count;
};

class EXPORT_IMPORT wxVariant : public wxVariantData, public QVariant
{
    wxVariantData *m_pData;
public:
    wxVariant();
    wxVariant(const wxVariant &other);
    wxVariant(int i);
    wxVariant(long i);
    wxVariant(uint ui);
    wxVariant(qlonglong ll);
    wxVariant(qulonglong ull);
    wxVariant(bool b);
    wxVariant(double d);
    wxVariant(float f);
    wxVariant(const QString &string);
    wxVariant(const QDate &date);
    wxVariant(const QTime &time);
    wxVariant(const QDateTime &datetime);
    wxVariant(const struct tm &date);
    wxVariant(wxVariantData *pdata);
    virtual ~wxVariant();
    wxString GetString() const;
    double GetDouble() const;
    wxDateTime GetDateTime() const;
    wxVariantData *GetData() const;
    void SetData(wxVariantData *_pData);
    bool IsNull() const;
    void Clear();

    // To be used with wxVariantData
    virtual bool Eq(wxVariantData& data) const;
    // What type is it? Return a string name.
    virtual wxString GetType() const;
};

#endif // WX_VARIANT_HEADER
