/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3ValueBind.cpp
// Library:     wxETKSQLite3
// Purpose:     Code for binding datas
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/04
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "wxETKSQLite3ValueBind.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                       wxETKSQLite3ValueBindBase                                       //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
wxETKSQLite3ValueBindBase::wxETKSQLite3ValueBindBase()
    : m_bIsNull(false)
{
}

wxETKSQLite3ValueBindBase::~wxETKSQLite3ValueBindBase()
{
}

const wxETKSQLite3ValueBindBase &wxETKSQLite3ValueBindBase::operator=(const wxETKSQLite3ValueBindBase &_rValueBind)
{
    m_bIsNull = _rValueBind.m_bIsNull;
    Copy(_rValueBind);

    return *this;
}

wxETKSQLite3ValueBindBase * wxETKSQLite3ValueBindBase::Clone() const
{
    wxETKSQLite3ValueBindBase *pClone = CreateInstance();
    *pClone = *this;

    return pClone;
}

bool wxETKSQLite3ValueBindBase::IsNull() const
{
    return m_bIsNull;
}

void wxETKSQLite3ValueBindBase::SetNull(bool _bIsNull) const
{
    m_bIsNull = _bIsNull;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                         wxETKSQLite3ValueBind                                         //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
wxETKSQLite3ValueBind::wxETKSQLite3ValueBind()
    : m_eDataType(eDataTypeNull)
{
    m_eDataPtr.m_pLongValue = NULL;
}

wxETKSQLite3ValueBind::wxETKSQLite3ValueBind(wxLongLong *_pLongLongValue)
    : m_eDataType(eDataTypeLongLong)
{
    m_eDataPtr.m_pLongLongValue = _pLongLongValue;
}

wxETKSQLite3ValueBind::wxETKSQLite3ValueBind(long *_pLongValue)
    : m_eDataType(eDataTypeLong)
{
    m_eDataPtr.m_pLongValue = _pLongValue;
}

wxETKSQLite3ValueBind::wxETKSQLite3ValueBind(unsigned long *_pULongValue)
    : m_eDataType(eDataTypeULong)
{
    m_eDataPtr.m_pULongValue = _pULongValue;
}

wxETKSQLite3ValueBind::wxETKSQLite3ValueBind(short int *_pShortIntValue)
    : m_eDataType(eDataTypeShortInt)
{
    m_eDataPtr.m_pShortIntValue = _pShortIntValue;
}

wxETKSQLite3ValueBind::wxETKSQLite3ValueBind(unsigned short int *_pUShortIntValue)
    : m_eDataType(eDataTypeUShortInt)
{
    m_eDataPtr.m_pUShortIntValue = _pUShortIntValue;
}

wxETKSQLite3ValueBind::wxETKSQLite3ValueBind(char *_pCharValue)
    : m_eDataType(eDataTypeChar)
{
    m_eDataPtr.m_pCharValue = _pCharValue;
}

wxETKSQLite3ValueBind::wxETKSQLite3ValueBind(unsigned char *_pUCharValue)
    : m_eDataType(eDataTypeUChar)
{
    m_eDataPtr.m_pUCharValue = _pUCharValue;
}

wxETKSQLite3ValueBind::wxETKSQLite3ValueBind(double *_pDoubleValue)
    : m_eDataType(eDataTypeDouble)
{
    m_eDataPtr.m_pDoubleValue = _pDoubleValue;
}

wxETKSQLite3ValueBind::wxETKSQLite3ValueBind(wxString *_pwxStringValue)
    : m_eDataType(eDataTypewxString)
{
    m_eDataPtr.m_pwxStringValue = _pwxStringValue;
}

wxETKSQLite3ValueBind::wxETKSQLite3ValueBind(wxDateTime *_pwxDateTimeValue)
    : m_eDataType(eDataTypewxDateTime)
{
    m_eDataPtr.m_pwxDateTimeValue = _pwxDateTimeValue;
}

wxETKSQLite3ValueBind::wxETKSQLite3ValueBind(bool *_pBoolValue)
    : m_eDataType(eDataTypeBool)
{
    m_eDataPtr.m_pBool = _pBoolValue;
}

wxETKSQLite3ValueBind::wxETKSQLite3ValueBind(const wxETKSQLite3ValueBind &_rValueBind)
{
    operator=(_rValueBind);
}

wxETKSQLite3ValueBind::~wxETKSQLite3ValueBind()
{
}

void wxETKSQLite3ValueBind::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const
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
            case eDataTypeLongLong :
            {
                _rstmt.Bind(_iIndex,*m_eDataPtr.m_pLongLongValue);
                break;
            }
            case eDataTypeLong :
            {
                _rstmt.Bind(_iIndex,wxLongLong(*m_eDataPtr.m_pLongValue));
                break;
            }
            case eDataTypeULong :
            {
                _rstmt.Bind(_iIndex,wxLongLong(*m_eDataPtr.m_pULongValue));
                break;
            }
            case eDataTypeShortInt :
            {
                _rstmt.Bind(_iIndex,wxLongLong((long) *m_eDataPtr.m_pShortIntValue));
                break;
            }
            case eDataTypeUShortInt :
            {
                _rstmt.Bind(_iIndex,wxLongLong((long) *m_eDataPtr.m_pUShortIntValue));
                break;
            }
            case eDataTypeChar :
            {
                _rstmt.Bind(_iIndex,wxLongLong((long) *m_eDataPtr.m_pCharValue));
                break;
            }
            case eDataTypeUChar :
            {
                _rstmt.Bind(_iIndex,wxLongLong((long) *m_eDataPtr.m_pUCharValue));
                break;
            }
            case eDataTypeDouble :
            {
                _rstmt.Bind(_iIndex,*m_eDataPtr.m_pDoubleValue);
                break;
            }
            case eDataTypewxString :
            {
                _rstmt.Bind(_iIndex,*m_eDataPtr.m_pwxStringValue);
                break;
            }
            case eDataTypewxDateTime :
            {
                _rstmt.BindDateTime(_iIndex,*m_eDataPtr.m_pwxDateTimeValue);
                break;
            }
            case eDataTypeBool :
            {
                _rstmt.BindBool(_iIndex,*m_eDataPtr.m_pBool);
                break;
            }
        }
    }
}

void wxETKSQLite3ValueBind::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
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
            case eDataTypeLongLong :
            {
                *m_eDataPtr.m_pLongLongValue = _rResultSet.GetInt64(_iIndex).GetValue();
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
            case eDataTypewxString :
            {
                *m_eDataPtr.m_pwxStringValue = _rResultSet.GetAsString(_iIndex);
                break;
            }
            case eDataTypewxDateTime :
            {
                *m_eDataPtr.m_pwxDateTimeValue = _rResultSet.GetDateTime(_iIndex);
                break;
            }
            case eDataTypeBool :
            {
                *m_eDataPtr.m_pBool = _rResultSet.GetBool(_iIndex);
                break;
            }
        }
    }
}

const wxETKSQLite3ValueBind & wxETKSQLite3ValueBind::operator=(const wxETKSQLite3ValueBind &_rValueBind)
{
    wxETKSQLite3ValueBindBase::operator=(_rValueBind);
    return *this;
}

const wxETKSQLite3ValueBindBase &wxETKSQLite3ValueBind::Copy(const wxETKSQLite3ValueBindBase &_rValueBind)
{
    m_eDataPtr.m_pLongValue = NULL;
    m_eDataType             = eDataTypeNull;

    const wxETKSQLite3ValueBind *pValueToBind = dynamic_cast<const wxETKSQLite3ValueBind *>(&_rValueBind);
    if (pValueToBind  != NULL)
    {   // Correct type, can assign
        m_eDataType = pValueToBind->m_eDataType;
        m_eDataPtr  = pValueToBind->m_eDataPtr;
    }
    else
    {   // Not a class derived from wxETKSQLite3ValueBind, reset types
        m_eDataType             = eDataTypeNull;
        m_eDataPtr.m_pLongValue = NULL;
        wxFAIL_MSG(wxT("wxETKSQLite3ValueBind::Copy: Bad value type to copy"));
    }

    return *this;
}

wxETKSQLite3ValueBindBase * wxETKSQLite3ValueBind::CreateInstance() const
{
    return new wxETKSQLite3ValueBind();
}
