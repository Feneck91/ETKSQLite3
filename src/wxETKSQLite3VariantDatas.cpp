/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3VariantDataBlob.h
// Library:     wxETKSQLite3
// Purpose:     Code for variants used into wxETKSQLite3 library
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/05
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "wxETKSQLite3VariantDatas.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                     wxETKSQLite3VariantDataBlob                                       //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

const wxString wxETKSQLite3VariantDataBlob::STR_VARIANT_BLOB_TYPE(_T("blob"));

wxETKSQLite3VariantDataBlob::wxETKSQLite3VariantDataBlob()
{
    m_pBuff      = NULL;
    m_stLenBuff  = 0;
}

wxETKSQLite3VariantDataBlob::wxETKSQLite3VariantDataBlob(const wxETKSQLite3VariantDataBlob &_rOther)
{
    if (this != &_rOther)
    {
        m_pBuff     = NULL;
        m_stLenBuff  = 0;
        operator=(_rOther);
    }
}

wxETKSQLite3VariantDataBlob::wxETKSQLite3VariantDataBlob(void *_pBuff,size_t _stLenBuff)
{
    m_stLenBuff = _stLenBuff;
    if (_pBuff != NULL)
    {   // Copy data
        m_pBuff = new unsigned char[m_stLenBuff];
        memcpy(m_pBuff,_pBuff,m_stLenBuff);
    }
    else
    {
        m_pBuff = NULL;
    }
}

wxETKSQLite3VariantDataBlob::~wxETKSQLite3VariantDataBlob()
{
    Clear();
}

void wxETKSQLite3VariantDataBlob::Clear()
{
    if (m_pBuff != NULL)
    {
        delete []m_pBuff;
        m_stLenBuff = 0;
        m_pBuff     = NULL;
    }
}

size_t wxETKSQLite3VariantDataBlob::Length() const
{
    return m_stLenBuff;
}

bool wxETKSQLite3VariantDataBlob::IsNull() const
{
    return m_stLenBuff == 0;
}

bool wxETKSQLite3VariantDataBlob::operator==(const wxETKSQLite3VariantDataBlob & _rblobValue) const
{
    if (Length() != _rblobValue.Length())
    {   // If size are not same, the elements are differents;
    }

    if (IsNull())
    {   // Both are null (already test the size are same, so a blob with 0 size is null, so don't need
        // to test both.
        return true;
    }
    return memcmp(m_pBuff,_rblobValue.m_pBuff,Length()) == 0;
}

const wxETKSQLite3VariantDataBlob & wxETKSQLite3VariantDataBlob::operator=(const wxETKSQLite3VariantDataBlob & _rblobValue)
{
    Clear();
    m_stLenBuff = _rblobValue.m_stLenBuff;
    if (_rblobValue.m_pBuff != NULL)
    {   // Copy data
        m_pBuff = new unsigned char[m_stLenBuff];
        memcpy(m_pBuff,_rblobValue.m_pBuff,m_stLenBuff);
    }

    return *this;
}

const wxETKSQLite3VariantDataBlob & wxETKSQLite3VariantDataBlob::operator=(const wxVariant &_rData)
{
    Clear(); // always clear this
    if (_rData.GetType() == STR_VARIANT_BLOB_TYPE)
    {
        operator=(dynamic_cast<wxETKSQLite3VariantDataBlob &>(*_rData.GetData()));
    }

    return *this;
}

unsigned char wxETKSQLite3VariantDataBlob::operator[](size_t _iIndex) const
{
    wxASSERT(_iIndex<m_stLenBuff);
    return m_pBuff[_iIndex];
}

bool wxETKSQLite3VariantDataBlob::GetBinaryFromVariant(const wxVariant &_rData)
{
    bool bRet = false;

    //Binary data is stored in the variant as an array of unsigned char
    if (_rData.GetType() == STR_VARIANT_BLOB_TYPE)
    {
        operator=(_rData);
        bRet = true;
    }

    return bRet;
}

bool wxETKSQLite3VariantDataBlob::PutBinaryToVariant(wxVariant &_rData) const
{
    _rData.SetData(new wxETKSQLite3VariantDataBlob(*this));
    return IsNull();
}

wxString wxETKSQLite3VariantDataBlob::GetAsString() const
{
    wxString strBlobAsString;

    for (size_t iBlc = 0;iBlc < Length();++iBlc)
    {
        strBlobAsString+=wxString::Format(_T("%02X"),(int) operator[](iBlc));
    }

    return strBlobAsString;
}

char * wxETKSQLite3VariantDataBlob::GetData()
{
    return (char *) m_pBuff;
}

const char * wxETKSQLite3VariantDataBlob::GetData() const
{
    return (const char *) m_pBuff;
}

void wxETKSQLite3VariantDataBlob::PutBytes(const char *_pBuff,size_t _stLenBuff)
{
    Clear();

    m_stLenBuff = _stLenBuff;
    if (_pBuff != NULL)
    {   // Copy data
        m_pBuff = new unsigned char[m_stLenBuff];
        memcpy(m_pBuff,_pBuff,m_stLenBuff);
    }
}

size_t wxETKSQLite3VariantDataBlob::GetSizeOfVariantBlob(const wxVariant &_rData)
{
    if (_rData.GetType() == STR_VARIANT_BLOB_TYPE)
    {
        return (dynamic_cast<wxETKSQLite3VariantDataBlob &>(*_rData.GetData())).Length();
    }

    return -1; // Erreur
}

wxString wxETKSQLite3VariantDataBlob::GetType() const
{
    return STR_VARIANT_BLOB_TYPE;
}

bool wxETKSQLite3VariantDataBlob::Eq(wxVariantData& _rData) const
{
    bool bRet = false;

    if (_rData.GetType() == STR_VARIANT_BLOB_TYPE)
    {
        wxETKSQLite3VariantDataBlob &rvariantBlob = dynamic_cast<wxETKSQLite3VariantDataBlob &>(_rData);
        bRet = operator==(rvariantBlob);
    }
    return bRet;
}

bool wxETKSQLite3VariantDataBlob::Write(wxString & _rstrValue) const
{
    _rstrValue = GetAsString();
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                             wxETKSQLite3VariantDataColumnAttributes                                   //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const wxString wxETKSQLite3VariantDataColumnAttributes::STR_VARIANT_COLUM_ATTRIBUTE_TYPE(_T("columnattributes"));

wxETKSQLite3VariantDataColumnAttributes::wxETKSQLite3VariantDataColumnAttributes()
{
}

wxETKSQLite3VariantDataColumnAttributes::wxETKSQLite3VariantDataColumnAttributes(const wxETKSQLite3ColumnAttributes &_rColumnAttributes)
    : wxVariantData()
    , wxETKSQLite3ColumnAttributes(_rColumnAttributes)
{
}

wxETKSQLite3VariantDataColumnAttributes::wxETKSQLite3VariantDataColumnAttributes(const wxETKSQLite3VariantDataColumnAttributes &_rOther)
{
    if (this != &_rOther)
    {   // Only call operator= on this
        operator=(_rOther);
    }
}

wxETKSQLite3VariantDataColumnAttributes::~wxETKSQLite3VariantDataColumnAttributes()
{
}

const wxETKSQLite3VariantDataColumnAttributes & wxETKSQLite3VariantDataColumnAttributes::operator=(const wxETKSQLite3VariantDataColumnAttributes & _rColumnAttributeValue)
{
    // wxVariantData::operator=(_rValueBind); Pb With count ref ?
    wxETKSQLite3ColumnAttributes::operator=(_rColumnAttributeValue);

    return *this;
}

const wxETKSQLite3VariantDataColumnAttributes & wxETKSQLite3VariantDataColumnAttributes::operator=(const wxVariant & _rData)
{
    if (_rData.GetType() == STR_VARIANT_COLUM_ATTRIBUTE_TYPE)
    {
        wxETKSQLite3VariantDataColumnAttributes &rvariantColumnAttribute = dynamic_cast<wxETKSQLite3VariantDataColumnAttributes &>(*_rData.GetData());
        operator=(rvariantColumnAttribute);
    }

    return *this;
}

wxString wxETKSQLite3VariantDataColumnAttributes::GetType() const
{
    return STR_VARIANT_COLUM_ATTRIBUTE_TYPE;
}

bool wxETKSQLite3VariantDataColumnAttributes::Eq(wxVariantData& _rData) const
{
    return false; // No test is done, not used !
}

bool wxETKSQLite3VariantDataColumnAttributes::Write(wxString & _rstrValue) const
{
    _rstrValue = _T("?");
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                 wxETKSQLite3VariantDataValueBind                                      //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const wxString wxETKSQLite3VariantDataValueBind::STR_VARIANT_VARIANT_BIND_TYPE(_T("valuebind"));

wxETKSQLite3VariantDataValueBind::wxETKSQLite3VariantDataValueBind()
{
}

wxETKSQLite3VariantDataValueBind::wxETKSQLite3VariantDataValueBind(const wxETKSQLite3ValueBind &_rValueBind)
    : wxVariantData()
    , wxETKSQLite3ValueBind(_rValueBind)
{
}

wxETKSQLite3VariantDataValueBind::wxETKSQLite3VariantDataValueBind(const wxETKSQLite3VariantDataValueBind &_rOther)
{
    if (this != &_rOther)
    {   // Only call operator= on this
        operator=(_rOther);
    }
}

wxETKSQLite3VariantDataValueBind::~wxETKSQLite3VariantDataValueBind()
{
}

const wxETKSQLite3VariantDataValueBind & wxETKSQLite3VariantDataValueBind::operator=(const wxETKSQLite3VariantDataValueBind & _rValueBind)
{
    // wxVariantData::operator=(_rValueBind); Pb With count ref ?
    wxETKSQLite3ValueBind::operator=(_rValueBind);
    return *this;
}

const wxETKSQLite3VariantDataValueBind & wxETKSQLite3VariantDataValueBind::operator=(const wxVariant & _rData)
{
    if (_rData.GetType() == STR_VARIANT_VARIANT_BIND_TYPE)
    {
        wxETKSQLite3VariantDataValueBind &rvariantValueBind = dynamic_cast<wxETKSQLite3VariantDataValueBind &>(*_rData.GetData());
        operator=(rvariantValueBind);
    }

    return *this;
}

wxString wxETKSQLite3VariantDataValueBind::GetType() const
{
    return STR_VARIANT_VARIANT_BIND_TYPE;
}

bool wxETKSQLite3VariantDataValueBind::Eq(wxVariantData& _rData) const
{
    return false; // No test is done, not used !
}

bool wxETKSQLite3VariantDataValueBind::Write(wxString & _rstrValue) const
{
    _rstrValue = _T("?");
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                              wxETKSQLite3VariantDataRequestSelector                                   //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const wxString wxETKSQLite3VariantDataRequestSelector::STR_VARIANT_REQUEST_SELECTOR_TYPE(_T("requestselector"));

wxETKSQLite3VariantDataRequestSelector::wxETKSQLite3VariantDataRequestSelector(const wxETKSQLite3RequestSelector &_rRequestSelector)
    : wxVariantData()
    , wxETKSQLite3RequestSelector(_rRequestSelector)
{
}

wxETKSQLite3VariantDataRequestSelector::wxETKSQLite3VariantDataRequestSelector(const wxETKSQLite3VariantDataRequestSelector &_rOther)
    : wxETKSQLite3RequestSelector(_rOther)
{
}

wxETKSQLite3VariantDataRequestSelector::~wxETKSQLite3VariantDataRequestSelector()
{
}

wxString wxETKSQLite3VariantDataRequestSelector::GetType() const
{
    return STR_VARIANT_REQUEST_SELECTOR_TYPE;
}

bool wxETKSQLite3VariantDataRequestSelector::Eq(wxVariantData& _rData) const
{
    return false; // No test is done, not used !
}

bool wxETKSQLite3VariantDataRequestSelector::Write(wxString & _rstrValue) const
{
    _rstrValue = GetCriterionRequest().GetSQL();
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                  wxETKSQLite3VariantDataLongLong                                      //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const wxString wxETKSQLite3VariantDataLongLong::STR_VARIANT_LONG_LONG_TYPE(_T("wxLongLong"));

wxETKSQLite3VariantDataLongLong::wxETKSQLite3VariantDataLongLong(const wxLongLong &_rllValue)
    : wxVariantData()
    , m_llValue(_rllValue)
{
}

wxETKSQLite3VariantDataLongLong::wxETKSQLite3VariantDataLongLong(const wxETKSQLite3VariantDataLongLong &_rOther)
    : m_llValue(_rOther.m_llValue)
{
}

wxETKSQLite3VariantDataLongLong::~wxETKSQLite3VariantDataLongLong()
{
}

const wxLongLong & wxETKSQLite3VariantDataLongLong::GetValue() const
{
    return m_llValue;
}

wxString wxETKSQLite3VariantDataLongLong::GetType() const
{
    return STR_VARIANT_LONG_LONG_TYPE;
}

bool wxETKSQLite3VariantDataLongLong::Eq(wxVariantData& _rData) const
{
    bool bRet = false;

    if (_rData.GetType() == STR_VARIANT_LONG_LONG_TYPE)
    {
        bRet = (m_llValue == dynamic_cast<wxETKSQLite3VariantDataLongLong &>(_rData).m_llValue);
    }
    return bRet;
}

bool wxETKSQLite3VariantDataLongLong::Write(wxString & _rstrValue) const
{
    _rstrValue = m_llValue.ToString();
    return true;
}
