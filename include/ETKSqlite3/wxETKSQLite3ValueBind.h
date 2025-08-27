/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3ValueBind.h
// Library:     wxETKSQLite3
// Purpose:     Code for binding datas
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/04
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_WX_ETK_SQLITE3_VALUE_BIND_H
#define INCLUDE_WX_ETK_SQLITE3_VALUE_BIND_H

#include "wxETKSQLite3Types.h"
#include "wxETKSQLite3SharedPtr.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                       wxETKSQLite3ValueBindBase                                       //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Class used to to record value pointer to bind.
 *
 * It ccntains value pointer and type and is used to bind value to SQL.
 *
 * @author Stéphane Château
 * @version Name : wxETKSQLite3<br>
 *          Revision : <b>1.0</b>
 */
class EXPORT_IMPORT wxETKSQLite3ValueBindBase : public wxETKSQLite3::IRefCountImpl<wxETKSQLite3ValueBindBase>
{
    /**
     * Indicate if the field is null or not.
     */
    mutable bool                        m_bIsNull;

public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Default constructor.
     */
    wxETKSQLite3ValueBindBase();

    /**
     * Destructor.
     */
    virtual ~wxETKSQLite3ValueBindBase();
    //@}

    /**
     * Indicate if the field is null or not.
     *
     * @return true if the field is null, false else.
     */
    bool                                IsNull() const;

    /**
     * Set field to null or not null.
     *
     * Could be called on const instance. Protect other members.
     *
     * @param _bIsNull true to put this field to null, false to put it to not null, in this case the
     *                 bind value will be use the value of the binded data.
     */
    void                                SetNull(bool _bIsNull = true) const;

    /**
     * Bind a data to the statement.
     *
     * <b>Must be overwrite by derived classes</b>.
     *
     * @param _rstmt Statement to bind to.
     * @param _iIndex Index of item to bind.
     * @param _bAddForInsertRequest Indicate if this column should be set to NULL for insert request.
     * @param _bForInsertRequest If is bind for an INSERT request. In this case, some field must be set to NULL value (for INSERT request, the primary key is computed).
     */
    virtual void                        BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const = 0;

    /**
     * Bind datas from the structure.
     *
     * <b>Must be overwrite by derived classes</b>.
     *
     * @param _rResultSet Reocrd set where binf data from.
     * @param _iIndex Index of column into _rResultSet recordset.
     */
    virtual void                        BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const = 0;

    /// @name Operators.
    //@{
    /**
     * Assignment operator.
     *
     * @param _rValueBind Bind value to copy into this.
     * @return This.
     */
    virtual const wxETKSQLite3ValueBindBase &operator=(const wxETKSQLite3ValueBindBase &_rValueBind);
    //@}

    /**
     * Clone.
     *
     * Clone this instance.
     *
     * @return Clone of this.
     */
    wxETKSQLite3ValueBindBase *         Clone() const;

protected:
    /**
     * Copy function.
     *
     * <b>Must be overwrite by derived classes</b>.
     *
     * @param _rValueBind Bind value to copy into this.
     * @return This.
     */
    virtual const wxETKSQLite3ValueBindBase &Copy(const wxETKSQLite3ValueBindBase &_rValueBind) = 0;

    /**
     * Create instance of the derived class.
     *
     * <b>Must be overwrite by derived classes</b>.
     *
     * @return A new instance of this.
     */
    virtual wxETKSQLite3ValueBindBase * CreateInstance() const = 0;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                         wxETKSQLite3ValueBind                                         //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Class used to to record value pointer to bind.
 *
 * It ccntains value pointer and type and is used to bind value to SQL.
 *
 * @author Stéphane Château
 * @version Name : wxETKSQLite3<br>
 *          Revision : <b>1.0</b>
 */
class EXPORT_IMPORT wxETKSQLite3ValueBind : public wxETKSQLite3ValueBindBase
{
private:
    /**
     * Type of pointer stored.
     */
    enum eDataType
    {
        eDataTypeNull,      // Not defined
        eDataTypeLongLong,
        eDataTypeLong,
        eDataTypeULong,
        eDataTypeShortInt,
        eDataTypeUShortInt,
        eDataTypeChar,
        eDataTypeUChar,
        eDataTypeDouble,
        eDataTypewxString,
        eDataTypewxDateTime,
        eDataTypeBool
    }                                   m_eDataType;

    /**
     * Type of pointer stored.
     */
    union eDataPtr
    {
        wxLongLong *            m_pLongLongValue;
        long *                  m_pLongValue;
        unsigned long *         m_pULongValue;
        short int *             m_pShortIntValue;
        unsigned short int *    m_pUShortIntValue;
        char *                  m_pCharValue;
        unsigned char *         m_pUCharValue;
        double *                m_pDoubleValue;
        wxString *              m_pwxStringValue;
        wxDateTime *            m_pwxDateTimeValue;
        bool *                  m_pBool;
    }                                   m_eDataPtr;
public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Default constructor.
     *
     * Set data type pointer to null.
     */
    wxETKSQLite3ValueBind();

    /**
     * Constructor.
     *
     * @param _pLongLongValue Value's pointer of type long 64 bits to bind.
     */
    wxETKSQLite3ValueBind(wxLongLong *_pLongLongValue);

    /**
     * Constructor.
     *
     * @param _pLongValue Value's pointer of type long to bind.
     */
    wxETKSQLite3ValueBind(long *_pLongValue);

    /**
     * Constructor.
     *
     * @param _pULongValue Value's pointer of type unsigned long to bind.
     */
    wxETKSQLite3ValueBind(unsigned long *_pULongValue);

    /**
     * Constructor.
     *
     * @param _pShortIntValue Value's pointer of type short int to bind.
     */
    wxETKSQLite3ValueBind(short int *_pShortIntValue);

    /**
     * Constructor.
     *
     * @param _pShortIntValue Value's pointer of type unsigned short int to bind.
     */
    wxETKSQLite3ValueBind(unsigned short int *_pUShortIntValue);

    /**
     * Constructor.
     *
     * @param _pCharValue Value's pointer of type char to bind.
     */
    wxETKSQLite3ValueBind(char *_pCharValue);

    /**
     * Constructor.
     *
     * @param _pUCharValue Value's pointer of type unsigned char to bind.
     */
    wxETKSQLite3ValueBind(unsigned char *_pUCharValue);

    /**
     * Constructor.
     *
     * @param _pDoubleValue Value's pointer of type double to bind.
     */
    wxETKSQLite3ValueBind(double *_pDoubleValue);

    /**
     * Constructor.
     *
     * @param _pwxStringValue Value's pointer of type wxString to bind.
     */
    wxETKSQLite3ValueBind(wxString *_pwxStringValue);

    /**
     * Constructor.
     *
     * @param _pwxDateTimeValue Value's pointer of type wxDateTime to bind.
     */
    wxETKSQLite3ValueBind(wxDateTime *_pwxDateTimeValue);

    /**
     * Constructor.
     *
     * @param _pBoolValue Value's pointer of type bool to bind.
     */
    wxETKSQLite3ValueBind(bool *_pBoolValue);

    /**
     * Copy constructor.
     *
     * @param _rValueBind Column to copy into this.
     */
    wxETKSQLite3ValueBind(const wxETKSQLite3ValueBind &_rValueBind);

    /**
     * Destructor.
     */
    virtual ~wxETKSQLite3ValueBind();
    //@}
    /**
     * Bind a data to the statement.
     *
     * @param _rstmt Statement to bind to.
     * @param _iIndex Index of item to bind.
     * @param _bAddForInsertRequest Indicate if this column should be set to NULL for insert request.
     * @param _bForInsertRequest If is bind for an INSERT request. In this case, some field must be set to NULL value (for INSERT request, the primary key is computed).
     */
    virtual void                        BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;

    /**
     * Bind datas from the structure.
     *
     * @param _rResultSet Reocrd set where binf data from.
     * @param _iIndex Index of column into _rResultSet recordset.
     */
    virtual void                        BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;

    /// @name Operators.
    //@{
    /**
     * Assignment operator.
     *
     * @param _rValueBind Bind value to copy into this.
     * @return This.
     */
    virtual const wxETKSQLite3ValueBind &operator=(const wxETKSQLite3ValueBind &_rValueBind);
    //@}

protected:
    /**
     * Copy function.
     *
     * @param _rValueBind Bind value to copy into this.
     * @return This.
     */
    virtual const wxETKSQLite3ValueBindBase &Copy(const wxETKSQLite3ValueBindBase &_rValueBind);

    /**
     * Create instance of the derived class.
     *
     * <b>Must be overwrite by derived classes</b>.
     *
     * @return A new instance of this.
     */
    virtual wxETKSQLite3ValueBindBase * CreateInstance() const;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                      wxETKSQLite3ValueBindOther                                       //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Class used to to record value pointer to bind.
 *
 * It ccntains value pointer and type and is used to bind value to SQL.
 *
 * @author Stéphane Château
 * @version Name : wxETKSQLite3<br>
 *          Revision : <b>1.0</b>
 */
template<class TYPE> class wxETKSQLite3ValueBindOther : public wxETKSQLite3ValueBind
{
private:
    TYPE *                              m_pDataPtr;

public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Default constructor.
     *
     * Set data type pointer to null.
     */
    wxETKSQLite3ValueBindOther();

    /**
     * Constructor.
     *
     * @param _pDataPtr Value's pointer of typed value to bind.
     */
    wxETKSQLite3ValueBindOther(TYPE *_pDataPtr);

    /**
     * Destructor.
     */
    virtual ~wxETKSQLite3ValueBindOther();
    //@}
    /**
     * Bind a data to the statement.
     *
     * @param _rstmt Statement to bind to.
     * @param _iIndex Index of item to bind.
     * @param _bAddForInsertRequest Indicate if this column should be set to NULL for insert request.
     * @param _bForInsertRequest If is bind for an INSERT request. In this case, some field must be set to NULL value (for INSERT request, the primary key is computed).
     */
    virtual void                        BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const;

    /**
     * Bind datas from the structure.
     *
     * @param _rResultSet Reocrd set where binf data from.
     * @param _iIndex Index of column into _rResultSet recordset.
     */
    virtual void                        BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const;

    /// @name Operators.
    //@{
    /**
     * Assignment operator.
     *
     * @param _rValueBind Bind value to copy into this.
     * @return This.
     */
    virtual const wxETKSQLite3ValueBindOther<TYPE> &operator=(const wxETKSQLite3ValueBindOther<TYPE> &_rValueBind);
    //@}

protected:
    /**
     * Copy function.
     *
     * @param _rValueBind Bind value to copy into this.
     * @return This.
     */
    virtual const wxETKSQLite3ValueBindBase &Copy(const wxETKSQLite3ValueBindBase &_rValueBind);

protected:
    /**
     * Create instance of the derived class.
     *
     * <b>Must be overwrite by derived classes</b>.
     *
     * @return A new instance of this.
     */
    virtual wxETKSQLite3ValueBindBase * CreateInstance() const;
};

template<class TYPE> wxETKSQLite3ValueBindOther<TYPE>::wxETKSQLite3ValueBindOther()
{
    m_pDataPtr = NULL;
}

template<class TYPE> wxETKSQLite3ValueBindOther<TYPE>::wxETKSQLite3ValueBindOther(TYPE *_pDataPtr)
    : m_pDataPtr(_pDataPtr)
{
}

template<class TYPE> wxETKSQLite3ValueBindOther<TYPE>::~wxETKSQLite3ValueBindOther()
{
}

template<class TYPE> const wxETKSQLite3ValueBindOther<TYPE> &wxETKSQLite3ValueBindOther<TYPE>::operator=(const wxETKSQLite3ValueBindOther<TYPE> &_rValueBind)
{
    wxETKSQLite3ValueBindBase::operator=(_rValueBind);
    return *this;
}

template<class TYPE> const wxETKSQLite3ValueBindBase &wxETKSQLite3ValueBindOther<TYPE>::Copy(const wxETKSQLite3ValueBindBase &_rValueBind)
{
    const wxETKSQLite3ValueBindOther<TYPE> *pValueToBind = dynamic_cast<const wxETKSQLite3ValueBindOther<TYPE> *>(&_rValueBind);
    if (pValueToBind  != NULL)
    {   // Correct type, can assign
        m_pDataPtr = pValueToBind->m_pDataPtr;
    }
    else
    {   // Not a class derived from wxETKSQLite3ValueBind, reset types
        m_pDataPtr = NULL;
        wxFAIL_MSG(wxT("wxETKSQLite3ValueBindOther::Copy: Bad value type to copy"));
    }

    return *this;
}

template<class TYPE> wxETKSQLite3ValueBindBase * wxETKSQLite3ValueBindOther<TYPE>::CreateInstance() const
{
    return new wxETKSQLite3ValueBindOther<TYPE>();
}

#endif // INCLUDE_WX_ETK_SQLITE3_VALUE_BIND_H
