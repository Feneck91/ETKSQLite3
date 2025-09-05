/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/variant.cpp
 * @brief Implementation file for wxVariant class.
 *
 * Variant implementation.
 *
 * @author Stéphane Château
 * @date Created: 2025/09/02
 * @date Modified:
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
//
// Internal include, simul wxWidgets
//
#include <wx/variant.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                             wxVariantData                                             //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

wxVariantData::wxVariantData()
    : m_count(0)
{
}

bool wxVariantData::Write(wxString & WXUNUSED(str)) const
{
    return false;
}

bool wxVariantData::Read(wxString & WXUNUSED(str))
{
    return false;
}

void wxVariantData::IncRef()
{
    m_count++;
}

void wxVariantData::DecRef()
{
    if ( --m_count == 0 )
    {
        delete this;
    }
}

int wxVariantData::GetRefCount() const
{
    return m_count;
}

wxVariantData::~wxVariantData()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                        Concrete wxVariantData                                         //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class wxVariantDataInt : public wxVariantData
{
public:
    wxVariantDataInt(int value) : m_value(value)
    {
    }

    bool Eq(wxVariantData& data) const override
    {
        wxVariantDataInt* pOther = dynamic_cast<wxVariantDataInt*>(&data);
        if (pOther == nullptr)
        {
            return false;
        }
        return m_value == pOther->m_value;
    }

    bool Write(wxString& str) const override
    {
        str = wxString::FromUTF8(std::to_string(m_value).c_str());
        return true;
    }

    wxString GetType() const override
    {
        return wxString("int");
    }

private:
    int m_value;
};

class wxVariantDataLong : public wxVariantData
{
public:
    wxVariantDataLong(long value) : m_value(value) {}

    bool Eq(wxVariantData& data) const override
    {
        wxVariantDataLong* pOther = dynamic_cast<wxVariantDataLong*>(&data);
        if (pOther == nullptr)
        {
            return false;
        }
        return m_value == pOther->m_value;
    }

    bool Write(wxString& str) const override
    {
        str = wxString::FromUTF8(std::to_string(m_value).c_str());
        return true;
    }

    wxString GetType() const override
    {
        return wxString("long");
    }

private:
    long m_value;
};

class wxVariantDataUInt : public wxVariantData
{
public:
    wxVariantDataUInt(unsigned int value) : m_value(value) {}

    bool Eq(wxVariantData& data) const override
    {
        wxVariantDataUInt* pOther = dynamic_cast<wxVariantDataUInt*>(&data);
        if (pOther == nullptr)
        {
            return false;
        }
        return m_value == pOther->m_value;
    }

    bool Write(wxString& str) const override
    {
        str = wxString::FromUTF8(std::to_string(m_value).c_str());
        return true;
    }

    wxString GetType() const override
    {
        return wxString("uint");
    }

private:
    unsigned int m_value;
};

class wxVariantDataLongLong : public wxVariantData
{
public:
    wxVariantDataLongLong(long long value) : m_value(value) {}

    bool Eq(wxVariantData& data) const override
    {
        wxVariantDataLongLong* pOther = dynamic_cast<wxVariantDataLongLong*>(&data);
        if (pOther == nullptr)
        {
            return false;
        }
        return m_value == pOther->m_value;
    }

    bool Write(wxString& str) const override
    {
        str = wxString::FromUTF8(std::to_string(m_value).c_str());
        return true;
    }

    wxString GetType() const override
    {
        return wxString("longlong");
    }

private:
    long long m_value;
};

class wxVariantDataULongLong : public wxVariantData
{
public:
    wxVariantDataULongLong(unsigned long long value) : m_value(value) {}

    bool Eq(wxVariantData& data) const override
    {
        wxVariantDataULongLong* pOther = dynamic_cast<wxVariantDataULongLong*>(&data);
        if (pOther == nullptr)
        {
            return false;
        }
        return m_value == pOther->m_value;
    }

    bool Write(wxString& str) const override
    {
        str = wxString::FromUTF8(std::to_string(m_value).c_str());
        return true;
    }

    wxString GetType() const override
    {
        return wxString("ulonglong");
    }

private:
    unsigned long long m_value;
};

class wxVariantDataBool : public wxVariantData
{
public:
    wxVariantDataBool(bool value) : m_value(value) {}

    bool Eq(wxVariantData& data) const override
    {
        wxVariantDataBool* pOther = dynamic_cast<wxVariantDataBool*>(&data);
        if (pOther == nullptr)
        {
            return false;
        }
        return m_value == pOther->m_value;
    }

    bool Write(wxString& str) const override
    {
        str = m_value ? wxString("true") : wxString("false");
        return true;
    }

    wxString GetType() const override
    {
        return wxString("bool");
    }

private:
    bool m_value;
};

class wxVariantDataDouble : public wxVariantData
{
public:
    wxVariantDataDouble(double value) : m_value(value) {}

    bool Eq(wxVariantData& data) const override
    {
        wxVariantDataDouble* pOther = dynamic_cast<wxVariantDataDouble*>(&data);
        if (pOther == nullptr)
        {
            return false;
        }
        return m_value == pOther->m_value;
    }

    bool Write(wxString& str) const override
    {
        str = wxString::FromUTF8(std::to_string(m_value).c_str());
        return true;
    }

    wxString GetType() const override
    {
        return wxString("double");
    }

private:
    double m_value;
};

class wxVariantDataFloat : public wxVariantData
{
public:
    wxVariantDataFloat(float value) : m_value(value) {}

    bool Eq(wxVariantData& data) const override
    {
        wxVariantDataFloat* pOther = dynamic_cast<wxVariantDataFloat*>(&data);
        if (pOther == nullptr)
        {
            return false;
        }
        return m_value == pOther->m_value;
    }

    bool Write(wxString& str) const override
    {
        str = wxString::FromUTF8(std::to_string(m_value).c_str());
        return true;
    }

    wxString GetType() const override
    {
        return wxString("float");
    }

private:
    float m_value;
};

class wxVariantDataString : public wxVariantData
{
public:
    wxVariantDataString(const std::string& value) : m_value(value) {}

    bool Eq(wxVariantData& data) const override
    {
        wxVariantDataString* pOther = dynamic_cast<wxVariantDataString*>(&data);
        if (pOther == nullptr)
        {
            return false;
        }
        return m_value == pOther->m_value;
    }

    bool Write(wxString& str) const override
    {
        str = wxString::FromUTF8(m_value.c_str());
        return true;
    }

    wxString GetType() const override
    {
        return wxString("string");
    }

private:
    std::string m_value;
};

class wxVariantDataDateTime : public wxVariantData
{
public:
    wxVariantDataDateTime(const wxDateTime& value) : m_value(value) {}

    bool Eq(wxVariantData& data) const override
    {
        wxVariantDataDateTime* pOther = dynamic_cast<wxVariantDataDateTime*>(&data);
        if (pOther == nullptr)
        {
            return false;
        }
        return m_value == pOther->m_value;
    }

    bool Write(wxString& str) const override
    {
        // Convert wxDateTime to string
        str = m_value.Format();
        return true;
    }

    wxString GetType() const override
    {
        return wxString("datetime");
    }

private:
    wxDateTime m_value;
};

class wxVariantDataTm : public wxVariantData
{
public:
    wxVariantDataTm(const struct tm& value) : m_value(value) {}

    bool Eq(wxVariantData& data) const override
    {
        wxVariantDataTm* pOther = dynamic_cast<wxVariantDataTm*>(&data);
        if (pOther == nullptr)
        {
            return false;
        }
        return difftime(mktime(const_cast<struct tm*>(&m_value)), mktime(const_cast<struct tm*>(&pOther->m_value))) == 0;
    }

    bool Write(wxString& str) const override
    {
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &m_value);
        str = wxString::FromUTF8(buffer);
        return true;
    }

    wxString GetType() const override
    {
        return wxString("tm");
    }

private:
    struct tm m_value;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                               wxVariant                                               //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
wxVariant::wxVariant()
    : m_pData(nullptr)
{
}

wxVariant::wxVariant(const wxVariant &other)
    : m_pData(other.m_pData)
{
    if (m_pData != nullptr)
    {   // If not nullptr increment reference count
        m_pData->IncRef();
    }
}

wxVariant::wxVariant(wxVariantData *pdata)
    : m_pData(pdata)
{
    if (m_pData != nullptr)
    {
        m_pData->IncRef();
    }
}

// Specific type constructors
wxVariant::wxVariant(int i)                         : m_pData(new wxVariantDataInt(i)) {}
wxVariant::wxVariant(long i)                        : m_pData(new wxVariantDataLong(i)) {}
wxVariant::wxVariant(unsigned int ui)               : m_pData(new wxVariantDataUInt(ui)) {}
wxVariant::wxVariant(int64_t ll)                    : m_pData(new wxVariantDataLongLong(ll)) {}
wxVariant::wxVariant(uint64_t ull)                  : m_pData(new wxVariantDataULongLong(ull)) {}
wxVariant::wxVariant(bool b)                        : m_pData(new wxVariantDataBool(b)) {}
wxVariant::wxVariant(double d)                      : m_pData(new wxVariantDataDouble(d)) {}
wxVariant::wxVariant(float f)                       : m_pData(new wxVariantDataFloat(f)) {}
wxVariant::wxVariant(const std::string& string)     : m_pData(new wxVariantDataString(string)) {}
wxVariant::wxVariant(const wxDateTime& datetime)    : m_pData(new wxVariantDataDateTime(datetime)) {}
wxVariant::wxVariant(const struct tm& date)         : m_pData(new wxVariantDataTm(date)) {}

wxVariant::~wxVariant()
{
    if (m_pData != nullptr)
    {
        m_pData->DecRef();
    }
}

wxString wxVariant::GetString() const
{
    wxString strValue = wxEmptyString;
    if (m_pData != nullptr)
    {
        m_pData->Write(strValue);
    }
    return strValue;
}

double wxVariant::GetDouble() const
{
    if (!m_pData)
    {
        return 0.0;
    }

    wxString str;
    if (m_pData->Write(str))
    {
        return std::stod(str.ToUTF8().data());
    }
    return 0.0;
}

wxDateTime wxVariant::GetDateTime() const
{
    if (!m_pData || m_pData->GetType() != "datetime")
    {
        return wxDateTime();
    }

    wxString str;
    if (m_pData->Write(str))
    {
        // Here you would parse the string to create a wxDateTime object
        // This is a placeholder; you'll need to implement the parsing logic
        return wxDateTime();
    }
    return wxDateTime();
}

wxVariantData *wxVariant::GetData() const
{
    if (m_pData != nullptr)
    {   // If not nullptr return the data
        return m_pData;
    }
    return (wxVariantData *) this;
}

void wxVariant::SetData(wxVariantData *_pData)
{
    if (m_pData != nullptr)
    {
        m_pData->DecRef();
    }
    m_pData=_pData;
    if (m_pData != nullptr)
    {
        m_pData->IncRef();
    }
}
bool wxVariant::IsNull() const
{
    return m_pData == nullptr;
}

void wxVariant::Clear()
{
    if (m_pData != nullptr)
    {
        m_pData->IncRef();
        m_pData = nullptr;
    }
}

wxString wxVariant::GetType() const
{
    return m_pData == nullptr
        ? "null"
        : m_pData->GetType();
}

bool wxVariant::Eq(const wxVariant& other) const
{
    if (IsNull() && other.IsNull())
    {
        return true;
    }
    if (IsNull() || other.IsNull())
    {
        return false;
    }
    return m_pData->Eq(*other.m_pData);
}