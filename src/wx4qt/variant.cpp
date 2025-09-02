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
//                                               wxVariant                                               //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
wxVariant::wxVariant()
    : m_pData(NULL)
{
}

wxVariant::wxVariant(const wxVariant &other)
    : QVariant(other)
    , m_pData(other.m_pData)
{
    if (m_pData != NULL)
    {   // If not NULL increment reference count
        m_pData->IncRef();
    }
}

wxVariant::wxVariant(int i)
    : QVariant(i)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(long i)
    : QVariant(qlonglong(i))
    , m_pData(NULL)
{
}

wxVariant::wxVariant(uint ui)
    : QVariant(ui)
    , m_pData(NULL)
{
}
wxVariant::wxVariant(qlonglong ll)
    : QVariant(ll)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(qulonglong ull)
    : QVariant(ull)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(bool b)
    : QVariant(b)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(double d)
    : QVariant(d)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(float f)
    : QVariant(f)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(const QString &string)
    : QVariant(string)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(const QDate &date)
    : QVariant(date)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(const QTime &time)
    : QVariant(time)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(const QDateTime &datetime)
    : QVariant(datetime)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(const struct tm &date)
    : QVariant(QDateTime(QDate(date.tm_year, date.tm_mon, date.tm_mday), QTime(date.tm_hour, date.tm_min,date.tm_sec, 0),Qt::LocalTime))
    , m_pData(NULL)
{
}

wxVariant::wxVariant(wxVariantData *pdata)
    : QVariant(),m_pData(pdata)
{
    if (m_pData != NULL)
    {
        m_pData->IncRef();
    }
}

wxVariant::~wxVariant()
{
    if (m_pData != NULL)
    {
        m_pData->DecRef();
    }
}

wxString wxVariant::GetString() const
{
    wxString strValue;
    if (m_pData != NULL)
    {
        m_pData->Write(strValue);
    }
    else
    {
        strValue = toString();
    }
    return strValue;
}

double wxVariant::GetDouble() const
{
    return toDouble();
}

wxDateTime wxVariant::GetDateTime() const
{
    return wxDateTime(toDateTime());
}

wxVariantData *wxVariant::GetData() const
{
    if (m_pData != NULL)
    {   // If not NULL return the data
        return m_pData;
    }
    return (wxVariantData *) this;
}

void wxVariant::SetData(wxVariantData *_pData)
{
    if (m_pData != NULL)
    {
        m_pData->DecRef();
    }
    m_pData=_pData;
    if (m_pData != NULL)
    {
        m_pData->IncRef();
    }
}
bool wxVariant::IsNull() const
{
    return isNull();
}

void wxVariant::Clear()
{
    clear();
    if (m_pData != NULL)
    {
        m_pData->IncRef();
        m_pData = NULL;
    }
}

wxString wxVariant::GetType() const
{
    wxString  strType;
    if (m_pData != NULL)
    {
        strType = m_pData->GetType();
    }
    else
    {
        strType = QVariant::typeName();
    }
    return strType;
}

bool wxVariant::Eq(wxVariantData& data) const
{
    wxVariant *pVariant = dynamic_cast<wxVariant *>(&data);
    if (pVariant == NULL)
    {   // The data is not a QVariant, test only if m_pData is not null
        return (m_pData != NULL) && m_pData->Eq(data);
    }
    else
    {
        if (GetType() == data.GetType())
        {   // Only if same type
            if (GetData() != NULL && pVariant->GetData() != NULL)
            {   // If both are wxVariantData type
                return GetData()->Eq(*pVariant->GetData());
            }
            else if (GetData()== NULL && pVariant->GetData() == NULL)
            {   // If both h NOT type
                return QVariant::operator ==(*pVariant);
            }
        }
    }
    return false;
}