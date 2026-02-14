/////////////////////////////////////////////////////////////////////////////
/**
 * @file ETKSQLite3ValueBind.cpp
 * @brief Implementation file for manage binding datas.
 *
 * Code for binding datas.
 *
 * @author Stéphane Château
 * @date Created: 2011/08/04
 * @date Modified: 2025/09/01
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#include "ETKSQLite3ValueBind.h"
#include <ETKSQLite3VariantDatas.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                        ETKSQLite3ValueBindBase                                        //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
ETKSQLite3ValueBindBase::ETKSQLite3ValueBindBase()
    : m_bIsNull(false)
{
}

ETKSQLite3ValueBindBase::ETKSQLite3ValueBindBase(const ETKSQLite3ValueBindBase &_rValueBindBase)
    : ETKSQLite3::IRefCountImpl<ETKSQLite3ValueBindBase>(_rValueBindBase)
    , m_bIsNull(false)
{
    operator=(_rValueBindBase);
}

ETKSQLite3ValueBindBase::~ETKSQLite3ValueBindBase()
{
}

const ETKSQLite3ValueBindBase &ETKSQLite3ValueBindBase::operator=(const ETKSQLite3ValueBindBase &_rValueBind)
{
    m_bIsNull = _rValueBind.m_bIsNull;
    Copy(_rValueBind);

    return *this;
}

ETKSQLite3ValueBindBase * ETKSQLite3ValueBindBase::Clone() const
{
    ETKSQLite3ValueBindBase *pClone = CreateInstance();
    *pClone = *this;

    return pClone;
}

bool ETKSQLite3ValueBindBase::IsNull() const
{
    return m_bIsNull;
}

void ETKSQLite3ValueBindBase::SetNull(bool _bIsNull) const
{
    m_bIsNull = _bIsNull;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                          ETKSQLite3ValueBind                                          //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
ETKSQLite3ValueBind::ETKSQLite3ValueBind()
    : m_eDataType(eDataTypeNull)
{
    m_eDataPtr.m_pLongValue = nullptr;
}

ETKSQLite3ValueBind::ETKSQLite3ValueBind(etkInt64 *_petkInt64Value)
    : m_eDataType(eDataTypeInt64)
{
    m_eDataPtr.m_petkInt64Value = _petkInt64Value;
}

ETKSQLite3ValueBind::ETKSQLite3ValueBind(long *_pLongValue)
    : m_eDataType(eDataTypeLong)
{
    m_eDataPtr.m_pLongValue = _pLongValue;
}

ETKSQLite3ValueBind::ETKSQLite3ValueBind(unsigned long *_pULongValue)
    : m_eDataType(eDataTypeULong)
{
    m_eDataPtr.m_pULongValue = _pULongValue;
}

ETKSQLite3ValueBind::ETKSQLite3ValueBind(short int *_pShortIntValue)
    : m_eDataType(eDataTypeShortInt)
{
    m_eDataPtr.m_pShortIntValue = _pShortIntValue;
}

ETKSQLite3ValueBind::ETKSQLite3ValueBind(unsigned short int *_pUShortIntValue)
    : m_eDataType(eDataTypeUShortInt)
{
    m_eDataPtr.m_pUShortIntValue = _pUShortIntValue;
}

ETKSQLite3ValueBind::ETKSQLite3ValueBind(char *_pCharValue)
    : m_eDataType(eDataTypeChar)
{
    m_eDataPtr.m_pCharValue = _pCharValue;
}

ETKSQLite3ValueBind::ETKSQLite3ValueBind(unsigned char *_pUCharValue)
    : m_eDataType(eDataTypeUChar)
{
    m_eDataPtr.m_pUCharValue = _pUCharValue;
}

ETKSQLite3ValueBind::ETKSQLite3ValueBind(double *_pDoubleValue)
    : m_eDataType(eDataTypeDouble)
{
    m_eDataPtr.m_pDoubleValue = _pDoubleValue;
}

ETKSQLite3ValueBind::ETKSQLite3ValueBind(etkString *_petkStringValue)
    : m_eDataType(eDataTypeString)
{
    m_eDataPtr.m_petkStringValue = _petkStringValue;
}

ETKSQLite3ValueBind::ETKSQLite3ValueBind(etkDateTime *_petkDateTimeValue)
    : m_eDataType(eDataTypeDateTime)
{
    m_eDataPtr.m_petkDateTimeValue = _petkDateTimeValue;
}

#if defined(QT_ETKSQLITE3) || defined(STL_ETKSQLITE3)

ETKSQLite3ValueBind::ETKSQLite3ValueBind(etkDate *_petkDateValue)
    : m_eDataType(eDataTypeDate)
{
    m_eDataPtr.m_petkDateValue = _petkDateValue;
}

ETKSQLite3ValueBind::ETKSQLite3ValueBind(etkTime *_petkTimeValue)
    : m_eDataType(eDataTypeTime)
{
    m_eDataPtr.m_petkTimeValue = _petkTimeValue;
}

#endif

ETKSQLite3ValueBind::ETKSQLite3ValueBind(bool *_pBoolValue)
    : m_eDataType(eDataTypeBool)
{
    m_eDataPtr.m_pBool = _pBoolValue;
}

ETKSQLite3ValueBind::ETKSQLite3ValueBind(const ETKSQLite3ValueBind &_rValueBind)
    : ETKSQLite3ValueBindBase(_rValueBind)
{
    operator=(_rValueBind);
}

ETKSQLite3ValueBind::~ETKSQLite3ValueBind()
{
}

void ETKSQLite3ValueBind::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const
{
    if ((!_bAddForInsertRequest && _bForInsertRequest) || IsNull())
    {   // Bind null if this field should not be add for insert request else it doesn't work
        // It is probably a primary key with integer autoincrement field
        // Bind null if this field is mark as NULL field, ignore binded data and set it to null
        _rstmt.BindNull(_iIndex);
    }
    else
    {
        switch(m_eDataType)
        {
            case eDataTypeNull :
            {
                _rstmt.BindNull(_iIndex);
                break;
            }
            case eDataTypeInt64 :
            {
                _rstmt.Bind(_iIndex,*m_eDataPtr.m_petkInt64Value);
                break;
            }
            case eDataTypeLong :
            {
                _rstmt.Bind(_iIndex,etkInt64(*m_eDataPtr.m_pLongValue));
                break;
            }
            case eDataTypeULong :
            {
                _rstmt.Bind(_iIndex,etkInt64(*m_eDataPtr.m_pULongValue));
                break;
            }
            case eDataTypeShortInt :
            {
                _rstmt.Bind(_iIndex,etkInt64((long) *m_eDataPtr.m_pShortIntValue));
                break;
            }
            case eDataTypeUShortInt :
            {
                _rstmt.Bind(_iIndex,etkInt64((long) *m_eDataPtr.m_pUShortIntValue));
                break;
            }
            case eDataTypeChar :
            {
                _rstmt.Bind(_iIndex,etkInt64((long) *m_eDataPtr.m_pCharValue));
                break;
            }
            case eDataTypeUChar :
            {
                _rstmt.Bind(_iIndex,etkInt64((long) *m_eDataPtr.m_pUCharValue));
                break;
            }
            case eDataTypeDouble :
            {
                _rstmt.Bind(_iIndex,*m_eDataPtr.m_pDoubleValue);
                break;
            }
            case eDataTypeString :
            {
                _rstmt.Bind(_iIndex,*m_eDataPtr.m_petkStringValue);
                break;
            }
            case eDataTypeDateTime :
            {
                _rstmt.BindDateTime(_iIndex,*m_eDataPtr.m_petkDateTimeValue);
                break;
            }
#if defined(QT_ETKSQLITE3) || defined(STL_ETKSQLITE3)
            case eDataTypeDate:
            {
                _rstmt.BindDate(_iIndex,*m_eDataPtr.m_petkDateValue);
                break;
            }
            case eDataTypeTime :
            {
                _rstmt.BindTime(_iIndex,*m_eDataPtr.m_petkTimeValue);
                break;
            }
#endif
            case eDataTypeBool :
            {
                _rstmt.BindBool(_iIndex,*m_eDataPtr.m_pBool);
                break;
            }
        }
    }
}

void ETKSQLite3ValueBind::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
{
    if (_rResultSet.IsNull(_iIndex))
    {
        SetNull();
    }
    else
    {
        SetNull(false); // This field is not null
        switch(m_eDataType)
        {
            case eDataTypeNull :
            {
                wxFAIL_MSG(wxT("Error, binding on null data type!"));
                break;
            }
            case eDataTypeInt64 :
            {
                *m_eDataPtr.m_petkInt64Value = etkInt64(_rResultSet.GetInt64(_iIndex).GetValue());
                break;
            }
            case eDataTypeLong :
            {
                *m_eDataPtr.m_pLongValue = (long) _rResultSet.GetInt64(_iIndex).GetValue();
                break;
            }
            case eDataTypeULong :
            {
                *m_eDataPtr.m_pULongValue = (unsigned long) _rResultSet.GetInt64(_iIndex).GetValue();
                break;
            }
            case eDataTypeShortInt :
            {
                *m_eDataPtr.m_pShortIntValue = (short int) _rResultSet.GetInt(_iIndex);
                break;
            }
            case eDataTypeUShortInt :
            {
                *m_eDataPtr.m_pUShortIntValue = (unsigned short int) _rResultSet.GetInt(_iIndex);
                break;
            }
            case eDataTypeChar :
            {
                *m_eDataPtr.m_pCharValue = (char) _rResultSet.GetInt(_iIndex);
                break;
            }
            case eDataTypeUChar :
            {
                *m_eDataPtr.m_pUCharValue = (unsigned char) _rResultSet.GetInt(_iIndex);
                break;
            }
            case eDataTypeDouble :
            {
                *m_eDataPtr.m_pDoubleValue = _rResultSet.GetDouble(_iIndex);
                break;
            }
            case eDataTypeString :
            {
                *m_eDataPtr.m_petkStringValue = _rResultSet.GetAsString(_iIndex);
                break;
            }
            case eDataTypeDateTime :
            {
                *m_eDataPtr.m_petkDateTimeValue = _rResultSet.GetDateTime(_iIndex);
                break;
            }
            case eDataTypeBool :
            {
                *m_eDataPtr.m_pBool = _rResultSet.GetBool(_iIndex);
                break;
            }
#if defined(QT_ETKSQLITE3) || defined(STL_ETKSQLITE3)
            case eDataTypeDate:
            {
                *m_eDataPtr.m_petkDateValue = _rResultSet.GetDate(_iIndex);
                break;
            }
            case eDataTypeTime:
            {
                *m_eDataPtr.m_petkTimeValue = _rResultSet.GetTime(_iIndex);
                break;
            }
#endif
        }
    }
}

const ETKSQLite3ValueBind & ETKSQLite3ValueBind::operator=(const ETKSQLite3ValueBind &_rValueBind)
{
    ETKSQLite3ValueBindBase::operator=(_rValueBind);
    return *this;
}

const ETKSQLite3ValueBindBase &ETKSQLite3ValueBind::Copy(const ETKSQLite3ValueBindBase &_rValueBind)
{
    m_eDataPtr.m_pLongValue = nullptr;
    m_eDataType             = eDataTypeNull;

    const ETKSQLite3ValueBind *pValueToBind = dynamic_cast<const ETKSQLite3ValueBind *>(&_rValueBind);
    if (pValueToBind  != nullptr)
    {   // Correct type, can assign
        m_eDataType = pValueToBind->m_eDataType;
        m_eDataPtr  = pValueToBind->m_eDataPtr;
    }
    else
    {   // Not a class derived from ETKSQLite3ValueBind, reset types
        m_eDataType             = eDataTypeNull;
        m_eDataPtr.m_pLongValue = nullptr;
        wxFAIL_MSG(wxT("ETKSQLite3ValueBind::Copy: Bad value type to copy"));
    }

    return *this;
}

ETKSQLite3ValueBindBase * ETKSQLite3ValueBind::CreateInstance() const
{
    return new ETKSQLite3ValueBind();
}
