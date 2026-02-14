/////////////////////////////////////////////////////////////////////////////
/**
 * @file ETKSQLite3ValueBind.h
 * @brief Header file for manage binding datas.
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
#ifndef INCLUDE_ETK_SQLITE3_VALUE_BIND_H
#define INCLUDE_ETK_SQLITE3_VALUE_BIND_H

#include "ETKSQLite3Types.h"
#include "ETKSQLite3SharedPtr.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                        ETKSQLite3ValueBindBase                                        //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Class used to to record value pointer to bind.
 *
 * It ccntains value pointer and type and is used to bind value to SQL.
 *
 * @author Stéphane Château
 * @version Name : ETKSQLite3<br>
 *          Revision : <b>1.0</b>
 */
class EXPORT_IMPORT ETKSQLite3ValueBindBase : public ETKSQLite3::IRefCountImpl<ETKSQLite3ValueBindBase>
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
    ETKSQLite3ValueBindBase();

    /**
     * Copy constructor.
     *
     * @param _rValueBind Value to copy into this.
     */
    ETKSQLite3ValueBindBase(const ETKSQLite3ValueBindBase &_rValueBindBase);

    /**
     * Destructor.
     */
    virtual ~ETKSQLite3ValueBindBase();
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
     const ETKSQLite3ValueBindBase &    operator=(const ETKSQLite3ValueBindBase &_rValueBind);
    //@}

    /**
     * Clone.
     *
     * Clone this instance.
     *
     * @return Clone of this.
     */
    ETKSQLite3ValueBindBase *         Clone() const;

protected:
    /**
     * Copy function.
     *
     * <b>Must be overwrite by derived classes</b>.
     *
     * @param _rValueBind Bind value to copy into this.
     * @return This.
     */
    virtual const ETKSQLite3ValueBindBase & Copy(const ETKSQLite3ValueBindBase &_rValueBind) = 0;

    /**
     * Create instance of the derived class.
     *
     * <b>Must be overwrite by derived classes</b>.
     *
     * @return A new instance of this.
     */
    virtual ETKSQLite3ValueBindBase * CreateInstance() const = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                          ETKSQLite3ValueBind                                          //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Class used to to record value pointer to bind.
 *
 * It ccntains value pointer and type and is used to bind value to SQL.
 *
 * @author Stéphane Château
 * @version Name : ETKSQLite3<br>
 *          Revision : <b>1.0</b>
 */
class EXPORT_IMPORT ETKSQLite3ValueBind : public ETKSQLite3ValueBindBase
{
private:
    /**
     * Type of pointer stored.
     */
    enum eDataType
    {
        eDataTypeNull,      // Not defined
        eDataTypeInt64,
        eDataTypeLong,
        eDataTypeULong,
        eDataTypeShortInt,
        eDataTypeUShortInt,
        eDataTypeChar,
        eDataTypeUChar,
        eDataTypeDouble,
        eDataTypeString,
        eDataTypeDateTime,
        eDataTypeDate,
        eDataTypeTime,
        eDataTypeBool
    }                                   m_eDataType;

    /**
     * Type of pointer stored.
     */
    union eDataPtr
    {
        etkInt64 *              m_petkInt64Value;
        long *                  m_pLongValue;
        unsigned long *         m_pULongValue;
        short int *             m_pShortIntValue;
        unsigned short int *    m_pUShortIntValue;
        char *                  m_pCharValue;
        unsigned char *         m_pUCharValue;
        double *                m_pDoubleValue;
        etkString *             m_petkStringValue;
        etkDateTime *           m_petkDateTimeValue;
#if defined(QT_ETKSQLITE3) || defined(STL_ETKSQLITE3)
        etkDate *               m_petkDateValue;
        etkTime *               m_petkTimeValue;
#endif
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
    ETKSQLite3ValueBind();

    /**
     * Constructor.
     *
     * @param _petkInt64Value Value's pointer of type long 64 bits to bind.
     */
    ETKSQLite3ValueBind(etkInt64 *_petkInt64Value);

    /**
     * Constructor.
     *
     * @param _pLongValue Value's pointer of type long to bind.
     */
    ETKSQLite3ValueBind(long *_pLongValue);

    /**
     * Constructor.
     *
     * @param _pULongValue Value's pointer of type unsigned long to bind.
     */
    ETKSQLite3ValueBind(unsigned long *_pULongValue);

    /**
     * Constructor.
     *
     * @param _pShortIntValue Value's pointer of type short int to bind.
     */
    ETKSQLite3ValueBind(short int *_pShortIntValue);

    /**
     * Constructor.
     *
     * @param _pShortIntValue Value's pointer of type unsigned short int to bind.
     */
    ETKSQLite3ValueBind(unsigned short int *_pUShortIntValue);

    /**
     * Constructor.
     *
     * @param _pCharValue Value's pointer of type char to bind.
     */
    ETKSQLite3ValueBind(char *_pCharValue);

    /**
     * Constructor.
     *
     * @param _pUCharValue Value's pointer of type unsigned char to bind.
     */
    ETKSQLite3ValueBind(unsigned char *_pUCharValue);

    /**
     * Constructor.
     *
     * @param _pDoubleValue Value's pointer of type double to bind.
     */
    ETKSQLite3ValueBind(double *_pDoubleValue);

    /**
     * Constructor.
     *
     * @param _petkStringValue Value's pointer of type etkString to bind.
     */
    ETKSQLite3ValueBind(etkString *_petkStringValue);

    /**
     * Constructor.
     *
     * @param _petkDateTimeValue Value's pointer of type etkDateTime to bind.
     */
    ETKSQLite3ValueBind(etkDateTime *_petkDateTimeValue);

#if defined(QT_ETKSQLITE3) || defined(STL_ETKSQLITE3)
    /**
     * Constructor.
     *
     * @param _petkDateValue Value's pointer of type etkDate to bind.
     */
    ETKSQLite3ValueBind(etkDate *_petkDateValue);

    /**
     * Constructor.
     *
     * @param _petkTimeValue Value's pointer of type etkTime to bind.
     */
    ETKSQLite3ValueBind(etkTime *_petkTimeValue);
#endif

    /**
     * Constructor.
     *
     * @param _pBoolValue Value's pointer of type bool to bind.
     */
    ETKSQLite3ValueBind(bool *_pBoolValue);

    /**
     * Copy constructor.
     *
     * @param _rValueBind Value bind to copy into this.
     */
    ETKSQLite3ValueBind(const ETKSQLite3ValueBind &_rValueBind);

    /**
     * Destructor.
     */
    virtual ~ETKSQLite3ValueBind();
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
     const ETKSQLite3ValueBind &        operator=(const ETKSQLite3ValueBind &_rValueBind);
    //@}

protected:
    /**
     * Copy function.
     *
     * @param _rValueBind Bind value to copy into this.
     * @return This.
     */
    virtual const ETKSQLite3ValueBindBase & Copy(const ETKSQLite3ValueBindBase &_rValueBind);

    /**
     * Create instance of the derived class.
     *
     * <b>Must be overwrite by derived classes</b>.
     *
     * @return A new instance of this.
     */
    virtual ETKSQLite3ValueBindBase * CreateInstance() const;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                       ETKSQLite3ValueBindOther                                        //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Class used to to record value pointer to bind.
 *
 * It ccntains value pointer and type and is used to bind value to SQL.
 *
 * @author Stéphane Château
 * @version Name : ETKSQLite3<br>
 *          Revision : <b>1.0</b>
 */
template<class TYPE> class ETKSQLite3ValueBindOther : public ETKSQLite3ValueBind
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
    ETKSQLite3ValueBindOther();

    /**
     * Constructor.
     *
     * @param _pDataPtr Value's pointer of typed value to bind.
     */
    ETKSQLite3ValueBindOther(TYPE *_pDataPtr);

    /**
     * Destructor.
     */
    virtual ~ETKSQLite3ValueBindOther();
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
     const ETKSQLite3ValueBindOther<TYPE> &operator=(const ETKSQLite3ValueBindOther<TYPE> &_rValueBind);
    //@}

protected:
    /**
     * Copy function.
     *
     * @param _rValueBind Bind value to copy into this.
     * @return This.
     */
    virtual const ETKSQLite3ValueBindBase &Copy(const ETKSQLite3ValueBindBase &_rValueBind);

protected:
    /**
     * Create instance of the derived class.
     *
     * <b>Must be overwrite by derived classes</b>.
     *
     * @return A new instance of this.
     */
    virtual ETKSQLite3ValueBindBase * CreateInstance() const;
};

template<class TYPE> ETKSQLite3ValueBindOther<TYPE>::ETKSQLite3ValueBindOther()
{
    m_pDataPtr = nullptr;
}

template<class TYPE> ETKSQLite3ValueBindOther<TYPE>::ETKSQLite3ValueBindOther(TYPE *_pDataPtr)
    : m_pDataPtr(_pDataPtr)
{
}

template<class TYPE> ETKSQLite3ValueBindOther<TYPE>::~ETKSQLite3ValueBindOther()
{
}

template<class TYPE> const ETKSQLite3ValueBindOther<TYPE> &ETKSQLite3ValueBindOther<TYPE>::operator=(const ETKSQLite3ValueBindOther<TYPE> &_rValueBind)
{
    ETKSQLite3ValueBindBase::operator=(_rValueBind);
    return *this;
}

template<class TYPE> const ETKSQLite3ValueBindBase &ETKSQLite3ValueBindOther<TYPE>::Copy(const ETKSQLite3ValueBindBase &_rValueBind)
{
    const ETKSQLite3ValueBindOther<TYPE> *pValueToBind = dynamic_cast<const ETKSQLite3ValueBindOther<TYPE> *>(&_rValueBind);
    if (pValueToBind  != nullptr)
    {   // Correct type, can assign
        m_pDataPtr = pValueToBind->m_pDataPtr;
    }
    else
    {   // Not a class derived from ETKSQLite3ValueBind, reset types
        m_pDataPtr = nullptr;
        wxFAIL_MSG(wxT("ETKSQLite3ValueBindOther::Copy: Bad value type to copy"));
    }

    return *this;
}

template<class TYPE> ETKSQLite3ValueBindBase * ETKSQLite3ValueBindOther<TYPE>::CreateInstance() const
{
    return new ETKSQLite3ValueBindOther<TYPE>();
}

#endif // INCLUDE_ETK_SQLITE3_VALUE_BIND_H
