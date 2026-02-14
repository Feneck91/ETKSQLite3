/////////////////////////////////////////////////////////////////////////////
/**
 * @file ETKSQLite3VariantDatas.cpp
 * @brief Implementation file for variants datas.
 *
 * Code for variants used into ETKSQLite3 library.
 *
 * @author Stéphane Château
 * @date Created: 2011/08/05
 * @date Modified: 2025/09/01
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#include "ETKSQLite3VariantDatas.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                      ETKSQLite3VariantDataBlob                                        //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

const etkString ETKSQLite3VariantDataBlob::STR_VARIANT_BLOB_TYPE(_T("blob"));

ETKSQLite3VariantDataBlob::ETKSQLite3VariantDataBlob()
{
    m_pBuff      = NULL;
    m_stLenBuff  = 0;
}

ETKSQLite3VariantDataBlob::ETKSQLite3VariantDataBlob(const ETKSQLite3VariantDataBlob &_rOther)
{
    if (this != &_rOther)
    {
        m_pBuff     = NULL;
        m_stLenBuff  = 0;
        operator=(_rOther);
    }
}

ETKSQLite3VariantDataBlob::ETKSQLite3VariantDataBlob(void *_pBuff,size_t _stLenBuff)
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

ETKSQLite3VariantDataBlob::~ETKSQLite3VariantDataBlob()
{
    Clear();
}

void ETKSQLite3VariantDataBlob::Clear()
{
    if (m_pBuff != NULL)
    {
        delete []m_pBuff;
        m_stLenBuff = 0;
        m_pBuff     = NULL;
    }
}

size_t ETKSQLite3VariantDataBlob::Length() const
{
    return m_stLenBuff;
}

bool ETKSQLite3VariantDataBlob::IsNull() const
{
    return m_stLenBuff == 0;
}

bool ETKSQLite3VariantDataBlob::operator==(const ETKSQLite3VariantDataBlob & _rblobValue) const
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

const ETKSQLite3VariantDataBlob & ETKSQLite3VariantDataBlob::operator=(const ETKSQLite3VariantDataBlob & _rblobValue)
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

const ETKSQLite3VariantDataBlob & ETKSQLite3VariantDataBlob::operator=(const wxVariant &_rData)
{
    Clear(); // always clear this
    if (_rData.GetType() == STR_VARIANT_BLOB_TYPE)
    {
        operator=(dynamic_cast<ETKSQLite3VariantDataBlob &>(*_rData.GetData()));
    }

    return *this;
}

unsigned char ETKSQLite3VariantDataBlob::operator[](size_t _iIndex) const
{
    wxASSERT_MSG(_iIndex < m_stLenBuff, wxT("Index out of range"));
    return m_pBuff[_iIndex];
}

bool ETKSQLite3VariantDataBlob::GetBinaryFromVariant(const wxVariant &_rData)
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

bool ETKSQLite3VariantDataBlob::PutBinaryToVariant(wxVariant &_rData) const
{
    _rData.SetData(new ETKSQLite3VariantDataBlob(*this));
    return IsNull();
}

etkString ETKSQLite3VariantDataBlob::GetAsString() const
{
    etkString strBlobAsString;

    for (size_t iBlc = 0;iBlc < Length();++iBlc)
    {
        strBlobAsString+=etkString::Format(_T("%02X"),(int) operator[](iBlc));
    }

    return strBlobAsString;
}

char * ETKSQLite3VariantDataBlob::GetData()
{
    return (char *) m_pBuff;
}

const char * ETKSQLite3VariantDataBlob::GetData() const
{
    return (const char *) m_pBuff;
}

void ETKSQLite3VariantDataBlob::PutBytes(const char *_pBuff,size_t _stLenBuff)
{
    Clear();

    m_stLenBuff = _stLenBuff;
    if (_pBuff != NULL)
    {   // Copy data
        m_pBuff = new unsigned char[m_stLenBuff];
        memcpy(m_pBuff,_pBuff,m_stLenBuff);
    }
}

size_t ETKSQLite3VariantDataBlob::GetSizeOfVariantBlob(const wxVariant &_rData)
{
    if (_rData.GetType() == STR_VARIANT_BLOB_TYPE)
    {
        return (dynamic_cast<ETKSQLite3VariantDataBlob &>(*_rData.GetData())).Length();
    }

    return std::numeric_limits<size_t>::max(); // Erreur
}

etkString ETKSQLite3VariantDataBlob::GetType() const
{
    return STR_VARIANT_BLOB_TYPE;
}

bool ETKSQLite3VariantDataBlob::Eq(wxVariantData& _rData) const
{
    bool bRet = false;

    if (_rData.GetType() == STR_VARIANT_BLOB_TYPE)
    {
        ETKSQLite3VariantDataBlob &rvariantBlob = dynamic_cast<ETKSQLite3VariantDataBlob &>(_rData);
        bRet = operator==(rvariantBlob);
    }
    return bRet;
}

bool ETKSQLite3VariantDataBlob::Write(etkString & _rstrValue) const
{
    _rstrValue = GetAsString();
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                              ETKSQLite3VariantDataColumnAttributes                                    //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const etkString ETKSQLite3VariantDataColumnAttributes::STR_VARIANT_COLUM_ATTRIBUTE_TYPE(_T("columnattributes"));

ETKSQLite3VariantDataColumnAttributes::ETKSQLite3VariantDataColumnAttributes()
{
}

ETKSQLite3VariantDataColumnAttributes::ETKSQLite3VariantDataColumnAttributes(const ETKSQLite3ColumnAttributes &_rColumnAttributes)
    : wxVariantData()
    , ETKSQLite3ColumnAttributes(_rColumnAttributes)
{
}

ETKSQLite3VariantDataColumnAttributes::ETKSQLite3VariantDataColumnAttributes(const ETKSQLite3VariantDataColumnAttributes &_rOther)
    : wxVariantData()
    , ETKSQLite3ColumnAttributes(_rOther)
{
    if (this != &_rOther)
    {   // Only call operator= on this
        operator=(_rOther);
    }
}

ETKSQLite3VariantDataColumnAttributes::~ETKSQLite3VariantDataColumnAttributes()
{
}

const ETKSQLite3VariantDataColumnAttributes & ETKSQLite3VariantDataColumnAttributes::operator=(const ETKSQLite3VariantDataColumnAttributes & _rColumnAttributeValue)
{
    // wxVariantData::operator=(_rValueBind); Pb With count ref ?
    ETKSQLite3ColumnAttributes::operator=(_rColumnAttributeValue);

    return *this;
}

const ETKSQLite3VariantDataColumnAttributes & ETKSQLite3VariantDataColumnAttributes::operator=(const wxVariant & _rData)
{
    if (_rData.GetType() == STR_VARIANT_COLUM_ATTRIBUTE_TYPE)
    {
        ETKSQLite3VariantDataColumnAttributes &rvariantColumnAttribute = dynamic_cast<ETKSQLite3VariantDataColumnAttributes &>(*_rData.GetData());
        operator=(rvariantColumnAttribute);
    }

    return *this;
}

etkString ETKSQLite3VariantDataColumnAttributes::GetType() const
{
    return STR_VARIANT_COLUM_ATTRIBUTE_TYPE;
}

bool ETKSQLite3VariantDataColumnAttributes::Eq(wxVariantData& _rData) const
{
    wxUnusedVar(_rData);
    return false; // No test is done, not used !
}

bool ETKSQLite3VariantDataColumnAttributes::Write(etkString & _rstrValue) const
{
    _rstrValue = _T("?");
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                  ETKSQLite3VariantDataValueBind                                       //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const etkString ETKSQLite3VariantDataValueBind::STR_VARIANT_VARIANT_BIND_TYPE(_T("valuebind"));

ETKSQLite3VariantDataValueBind::ETKSQLite3VariantDataValueBind()
{
}

ETKSQLite3VariantDataValueBind::ETKSQLite3VariantDataValueBind(const ETKSQLite3ValueBind &_rValueBind)
    : wxVariantData()
    , ETKSQLite3ValueBind(_rValueBind)
{
}

ETKSQLite3VariantDataValueBind::ETKSQLite3VariantDataValueBind(const ETKSQLite3VariantDataValueBind &_rOther)
    : wxVariantData()
    , ETKSQLite3ValueBind(_rOther)
{
    if (this != &_rOther)
    {   // Only call operator= on this
        operator=(_rOther);
    }
}

ETKSQLite3VariantDataValueBind::~ETKSQLite3VariantDataValueBind()
{
}

const ETKSQLite3VariantDataValueBind & ETKSQLite3VariantDataValueBind::operator=(const ETKSQLite3VariantDataValueBind & _rValueBind)
{
    // wxVariantData::operator=(_rValueBind); Pb With count ref ?
    ETKSQLite3ValueBind::operator=(_rValueBind);
    return *this;
}

const ETKSQLite3VariantDataValueBind & ETKSQLite3VariantDataValueBind::operator=(const wxVariant & _rData)
{
    if (_rData.GetType() == STR_VARIANT_VARIANT_BIND_TYPE)
    {
        ETKSQLite3VariantDataValueBind &rvariantValueBind = dynamic_cast<ETKSQLite3VariantDataValueBind &>(*_rData.GetData());
        operator=(rvariantValueBind);
    }

    return *this;
}

etkString ETKSQLite3VariantDataValueBind::GetType() const
{
    return STR_VARIANT_VARIANT_BIND_TYPE;
}

bool ETKSQLite3VariantDataValueBind::Eq(wxVariantData& _rData) const
{
    wxUnusedVar(_rData);
    return false; // No test is done, not used !
}

bool ETKSQLite3VariantDataValueBind::Write(etkString & _rstrValue) const
{
    _rstrValue = _T("?");
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                               ETKSQLite3VariantDataRequestSelector                                    //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const etkString ETKSQLite3VariantDataRequestSelector::STR_VARIANT_REQUEST_SELECTOR_TYPE(_T("requestselector"));

ETKSQLite3VariantDataRequestSelector::ETKSQLite3VariantDataRequestSelector(const ETKSQLite3RequestSelector &_rRequestSelector)
    : wxVariantData()
    , ETKSQLite3RequestSelector(_rRequestSelector)
{
}

ETKSQLite3VariantDataRequestSelector::ETKSQLite3VariantDataRequestSelector(const ETKSQLite3VariantDataRequestSelector &_rOther)
    : ETKSQLite3RequestSelector(_rOther)
{
}

ETKSQLite3VariantDataRequestSelector::~ETKSQLite3VariantDataRequestSelector()
{
}

etkString ETKSQLite3VariantDataRequestSelector::GetType() const
{
    return STR_VARIANT_REQUEST_SELECTOR_TYPE;
}

bool ETKSQLite3VariantDataRequestSelector::Eq(wxVariantData& _rData) const
{
    wxUnusedVar(_rData);
    return false; // No test is done, not used !
}

bool ETKSQLite3VariantDataRequestSelector::Write(etkString & _rstrValue) const
{
    _rstrValue = GetCriterionRequest().GetSQL();
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                   ETKSQLite3VariantDataLongLong                                       //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const etkString ETKSQLite3VariantDataLongLong::STR_VARIANT_LONG_LONG_TYPE(_T("wxLongLong"));

ETKSQLite3VariantDataLongLong::ETKSQLite3VariantDataLongLong(const wxLongLong &_rllValue)
    : wxVariantData()
    , m_llValue(_rllValue)
{
}

ETKSQLite3VariantDataLongLong::ETKSQLite3VariantDataLongLong(const ETKSQLite3VariantDataLongLong &_rOther)
    : m_llValue(_rOther.m_llValue)
{
}

ETKSQLite3VariantDataLongLong::~ETKSQLite3VariantDataLongLong()
{
}

const wxLongLong & ETKSQLite3VariantDataLongLong::GetValue() const
{
    return m_llValue;
}

etkString ETKSQLite3VariantDataLongLong::GetType() const
{
    return STR_VARIANT_LONG_LONG_TYPE;
}

bool ETKSQLite3VariantDataLongLong::Eq(wxVariantData& _rData) const
{
    bool bRet = false;

    if (_rData.GetType() == STR_VARIANT_LONG_LONG_TYPE)
    {
        bRet = (m_llValue == dynamic_cast<ETKSQLite3VariantDataLongLong &>(_rData).m_llValue);
    }
    return bRet;
}

bool ETKSQLite3VariantDataLongLong::Write(etkString & _rstrValue) const
{
    _rstrValue = m_llValue.ToString();
    return true;
}
