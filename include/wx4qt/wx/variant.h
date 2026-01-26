/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/variant.cpp
 * @brief Header file for wxVariant class.
 *
 * Variant implementation.
 *
 * @author Stéphane Château
 * @date Created: 2014/06/02
 * @date Modified: 2025/09/02
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_VARIANT_HEADER
#define WX_VARIANT_HEADER

//
// Internal include, simul wxWidgets
//
#include <wx/wx.h>
#include <wx/wxtype.h>
#include <wx/datetime.h>

//
// wxVariant Class
//
#include <QVariant>
#include <QDateTime>

class EXPORT_IMPORT wxVariantData
{
    friend class wxVariant;
public:
    wxVariantData();

    // Override these to provide common functionality
    virtual bool Eq(wxVariantData& data) const = 0;
    virtual bool Write(wxString & WXUNUSED(str)) const;
    virtual bool Read(wxString & WXUNUSED(str));
    // What type is it? Return a string name.
    virtual wxString GetType() const = 0;

    void IncRef();
    void DecRef();
    int GetRefCount() const;

protected:
    // Protected dtor should make some incompatible code
    // break more louder. That is, they should do data->DecRef()
    // instead of delete data.
    virtual ~wxVariantData();

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
    wxVariant & operator=(const wxVariant &) = default;
    // To be used with wxVariantData
    virtual bool Eq(wxVariantData& data) const;
    // What type is it? Return a string name.
    virtual wxString GetType() const;
};

#endif // WX_VARIANT_HEADER
