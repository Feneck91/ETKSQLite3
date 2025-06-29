/////////////////////////////////////////////////////////////////////////////
// Name:        wxETKSQLite3VariantDataBlob.cpp
// Library:     wxETKSQLite3
// Purpose:     Code for variants used into wxETKSQLite3 library
// Author:      Stéphane Château (Feneck91@free.fr)
// Modified by:
// Created:     2011/08/05
// Copyright:   © Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_WX_ETK_SQLITE3_WX_VARIANTS_H
#define INCLUDE_WX_ETK_SQLITE3_WX_VARIANTS_H

#include "wxETKSQLite3Types.h"
#include "wxETKSQLite3Column.h"
#include "wxETKSQLite3ValueBind.h"
#include "wxETKSQLite3RequestSelector.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                     wxETKSQLite3VariantDataBlob                                       //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Class used to ge/set blob from/to sqlite3
 *
 * @author Stéphane Château
 * @version Name : wxETKSQLite3<br>
 *          Revision : <b>1.0</b>
 */
class EXPORT_IMPORT wxETKSQLite3VariantDataBlob  : public wxVariantData
{
public:
    /**
     * Type name for this variant.
     */
    static const wxString               STR_VARIANT_BLOB_TYPE;

    /**
     * Internal blob buffer.
     */
    unsigned char *                     m_pBuff;

    /**
     * Size of internal blob buffer.
     */
    size_t                              m_stLenBuff;

public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Default constructor.
     *
     * The blob is empty.
     */
    wxETKSQLite3VariantDataBlob();

    /**
     * Constructor.
     *
     * Is used to create blog from a buffer.
     *
     * @param _pBuff Pointer on buffer.
     * @param _stLenBuff Buffer size.
     */
    wxETKSQLite3VariantDataBlob(void *_pBuff,size_t _stLenBuff);

    /**
     * Copy constructor.
     *
     * @param _rOther Blob to copy into this.
     */
    wxETKSQLite3VariantDataBlob(const wxETKSQLite3VariantDataBlob &_rOther);

    /**
     * Destructor.
     */
    ~wxETKSQLite3VariantDataBlob();
    //@}

    /// @name Operators.
    //@{
    /**
     * Assignment operator.
     *
     * @param _rblobValue Blob to copy into this.
     * @return this.
     */
    const wxETKSQLite3VariantDataBlob & operator=(const wxETKSQLite3VariantDataBlob & _rblobValue);

    /**
     * Assignment operator.
     *
     * If param is not a Blob, this is clear and so, this is Null.
     *
     * @param _rData Variant of type Blob to copy into this (else the content of this is clear).
     * @return this.
     */
    const wxETKSQLite3VariantDataBlob & operator=(const wxVariant & _rData);

    /**
     * Test the equality of blobs.
     *
     * @param _rblobValue Blob content to test equality.
     * @return true if both blobs are same, false else.
     */
    bool                                operator==(const wxETKSQLite3VariantDataBlob & _rblobValue) const;

    /**
     * Operator used to get one element of the internal blob by index.
     *
     * @param _iIndex Index.
     * @return Value of this element as unsigned char.
     */
    unsigned char                       operator[](size_t _iIndex) const;
    //@}

    /**
     * Clear blob contents.
     */
    void                                Clear();

    /**
     * Get the content size of the blob.
     */
    size_t                              Length() const;

    /**
     * Test if blob is null or not.
     */
    bool                                IsNull() const;

    /**
     * Get the pointer of internal blob datas.
     */
    char *                              GetData();

    /**
     * Get the pointer of internal blob datas.
     */
    const char *                        GetData() const;

    /**
     * Fill the internal blob from blob variant.
     *
     * Is same function as operator = but on = operator this is always clear, in this function
     * if the type is not correct, 'this' is not modified.
     *
     * @return false if the assignment failed (bad type of input data, only Null variant and Blob are allowed).
     *         If the function returns false, this is not modified.
     * @param _rData Variant that contains the blob.
     * @see PutBinaryToVariant
     */
    bool                                GetBinaryFromVariant(const wxVariant &_rData);

    /**
     * Fill variant from the internal blob.
     *
     * @see GetBinaryFromVariant
     * @param _rData Variant to init.
     * @return true if the blob is not null.
     */
    bool                                PutBinaryToVariant(wxVariant &_rData) const;

    /**
     * Get a string corresponding to the blob.
     *
     * Each byte is converted to hexadecimal.
     *
     * @return The blob as hexa form, one byte after one other.
     * @see PutBinaryToVariant,GetBinaryFromVariant
     */
    wxString                            GetAsString() const;

    /**
     * Initialize a blob from a memory buffer.
     *
     * @param _pBuff Pointer on buffer.
     * @param _stLenBuff Length buffer.
     */
    void                                PutBytes(const char *_pBuff,size_t _stLenBuff);

    /**
     * Get the size of blob variant type.
     *
     * @param _rData Data that must be a blob variant.
     * @return Blob size, -1 if error (bad type).
     */
    static size_t                       GetSizeOfVariantBlob(const wxVariant &_rData);

    /** @name Operators.
     *
     * Override mandatory base classes (pure virtual).
     */
    //@{
    /**
     * What type is it?
     *
     * @return A string name.
     */
    virtual wxString                    GetType() const;

    /**
     * Override these to provide common functionality.
     *
     * @param _rData Variant to test if is equal to this.
     * @return true if is same.
     */
    virtual bool                        Eq(wxVariantData& _rData) const;

    /**
     * Used to convert to wxString.
     *
     * Use GetAsString() function.
     *
     * @param _rstrValue String value to set with blob content.
     * @return Always true.
     */
    virtual bool                        Write(wxString & _rstrValue) const;
    //@}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                             wxETKSQLite3VariantDataColumnAttributes                                   //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Class used to record columns attribute
 *
 * @author Stéphane Château
 * @version Name : wxETKSQLite3<br>
 *          Revision : <b>1.0</b>
 */
class EXPORT_IMPORT wxETKSQLite3VariantDataColumnAttributes  : public wxVariantData
                                                             , public wxETKSQLite3ColumnAttributes
{
public:
    /**
     * Type name for this variant.
     */
    static const wxString               STR_VARIANT_COLUM_ATTRIBUTE_TYPE;

public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Default constructor.
     *
     * All columns attributes are empty.
     */
    wxETKSQLite3VariantDataColumnAttributes();

    /**
     * Constructor used to record column attributes.
     *
     * @param _rColumnAttributes Column attributes to assign to this, all will be copied into this.
     */
    wxETKSQLite3VariantDataColumnAttributes(const wxETKSQLite3ColumnAttributes &_rColumnAttributes);

    /**
     * Copy constructor.
     *
     * @param _rOther Column attributes to copy into this.
     */
    wxETKSQLite3VariantDataColumnAttributes(const wxETKSQLite3VariantDataColumnAttributes &_rOther);

    /**
     * Destructor.
     */
    ~wxETKSQLite3VariantDataColumnAttributes();
    //@}

    /// @name Operators.
    //@{
    /**
     * Assignment operator.
     *
     * @param _rColumnAttributeValue Column attribute to copy into this.
     * @return this.
     */
    const wxETKSQLite3VariantDataColumnAttributes & operator=(const wxETKSQLite3VariantDataColumnAttributes & _rColumnAttributeValue);

    /**
     * Assignment operator.
     *
     * If param is not a Blob, this function does nothing.
     *
     * @param _rData Variant of type Column attribute to copy into this (else the function does nothing).
     * @return this.
     */
    const wxETKSQLite3VariantDataColumnAttributes & operator=(const wxVariant & _rData);
    //@}

    /** @name Operators.
     *
     * Override mandatory base classes (pure virtual).
     */
    //@{
    /**
     * What type is it?
     *
     * @return A string name.
     */
    virtual wxString                    GetType() const;

    /**
     * Override these to provide common functionality.
     *
     * @param _rData Variant to test if is equal to this.
     * @return always return false, never test all columns equality.
     */
    virtual bool                        Eq(wxVariantData& _rData) const;

    /**
     * Used to convert to wxString.
     *
     * Always return "<b>?</b>" because it is used into SQL request creation with columns data (in assignation only).
     *
     * @param _rstrValue String value to set with this content. Always returns "?".
     * @return Always true.
     */
    virtual bool                        Write(wxString & _rstrValue) const;
    //@}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                 wxETKSQLite3VariantDataValueBind                                      //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Class used to record value binding attribute
 *
 * @author Stéphane Château
 * @version Name : wxETKSQLite3<br>
 *          Revision : <b>1.0</b>
 */
class EXPORT_IMPORT wxETKSQLite3VariantDataValueBind : public wxVariantData
                                                     , public wxETKSQLite3ValueBind
{
public:
    /**
     * Type name for this variant.
     */
    static const wxString               STR_VARIANT_VARIANT_BIND_TYPE;
public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Default constructor.
     *
     * All columns attributes are empty.
     */
    wxETKSQLite3VariantDataValueBind();

    /**
     * Constructor used to record column attributes.
     *
     * @param _rValueBind  Contains value data and class that record how to bind it.
     */
    wxETKSQLite3VariantDataValueBind(const wxETKSQLite3ValueBind &_rValueBind);

    /**
     * Copy constructor.
     *
     * @param _rOther Value bind to copy into this.
     */
    wxETKSQLite3VariantDataValueBind(const wxETKSQLite3VariantDataValueBind &_rOther);

    /**
     * Destructor.
     */
    ~wxETKSQLite3VariantDataValueBind();
    //@}

    /// @name Operators.
    //@{
    /**
     * Assignment operator.
     *
     * @param _rValueBind Value binding reference.
     * @return this.
     */
    const wxETKSQLite3VariantDataValueBind & operator=(const wxETKSQLite3VariantDataValueBind & _rValueBind);

    /**
     * Assignment operator.
     *
     * If param is not a Blob, this function does nothing.
     *
     * @param _rData Variant of type Column attribute to copy into this (else the function does nothing).
     * @return this.
     */
    const wxETKSQLite3VariantDataValueBind & operator=(const wxVariant & _rData);
    //@}

    /** @name Operators.
     *
     * Override mandatory base classes (pure virtual).
     */
    //@{
    /**
     * What type is it?
     *
     * @return A string name.
     */
    virtual wxString                    GetType() const;

    /**
     * Override these to provide common functionality.
     *
     * @param _rData Variant to test if is equal to this.
     * @return always return false, never test all columns equality.
     */
    virtual bool                        Eq(wxVariantData& _rData) const;

    /**
     * Used to convert to wxString.
     *
     * Always return "<b>?</b>" because it is used into SQL request creation with columns data (in assignation only).
     *
     * @param _rstrValue String value to set with this content. Always returns "?".
     * @return Always true.
     */
    virtual bool                        Write(wxString & _rstrValue) const;
    //@}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                wxETKSQLite3VariantDataRequestSelector                                 //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Class used to record request selector.
 *
 * @author Stéphane Château
 * @version Name : wxETKSQLite3<br>
 *          Revision : <b>1.0</b>
 */
class EXPORT_IMPORT wxETKSQLite3VariantDataRequestSelector  : public wxVariantData
                                                            , public wxETKSQLite3RequestSelector
{
public:
    /**
     * Type name for this variant.
     */
    static const wxString               STR_VARIANT_REQUEST_SELECTOR_TYPE;

public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Constructor used to record column attributes.
     *
     * @param _rRequestSelector Selector to copy into this.
     */
    wxETKSQLite3VariantDataRequestSelector(const wxETKSQLite3RequestSelector &_rRequestSelector);

    /**
     * Copy constructor.
     *
     * @param _rOther Column attributes to copy into this.
     */
    wxETKSQLite3VariantDataRequestSelector(const wxETKSQLite3VariantDataRequestSelector &_rOther);

    /**
     * Destructor.
     */
    ~wxETKSQLite3VariantDataRequestSelector();
    //@}

    /** @name Operators.
     *
     * Override mandatory base classes (pure virtual).
     */
    //@{
    /**
     * What type is it?
     *
     * @return A string name.
     */
    virtual wxString                    GetType() const;

    /**
     * Override these to provide common functionality.
     *
     * @param _rData Variant to test if is equal to this.
     * @return always return false, never test all columns equality.
     */
    virtual bool                        Eq(wxVariantData& _rData) const;

    /**
     * Used to convert to wxString.
     *
     * Always return "<b>?</b>" because it is used into SQL request creation with columns data (in assignation only).
     *
     * @param _rstrValue String value to set with this content. Always returns "?".
     * @return Always true.
     */
    virtual bool                        Write(wxString & _rstrValue) const;
    //@}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                    wxETKSQLite3VariantDataLongLong                                    //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Class used to record a long long value.
 *
 * @author Stéphane Château
 * @version Name : wxETKSQLite3<br>
 *          Revision : <b>1.0</b>
 */
class EXPORT_IMPORT wxETKSQLite3VariantDataLongLong : public wxVariantData
{
public:
    /**
     * Type name for this variant.
     */
    static const wxString               STR_VARIANT_LONG_LONG_TYPE;
private:
    /**
     * Long long value.
     */
    wxLongLong                          m_llValue;

public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Constructor used to record column attributes.
     *
     * @param _rllValue Value to copy into this.
     */
    wxETKSQLite3VariantDataLongLong(const wxLongLong &_rllValue);

    /**
     * Copy constructor.
     *
     * @param _rOther Variant LongLong value to copy into this.
     */
    wxETKSQLite3VariantDataLongLong(const wxETKSQLite3VariantDataLongLong &_rOther);

    /**
     * Destructor.
     */
    ~wxETKSQLite3VariantDataLongLong();
    //@}

    /**
     * Get the value.
     *
     * @return The long long value.
     */
    const wxLongLong &                  GetValue() const;

    /** @name Operators.
     *
     * Override mandatory base classes (pure virtual).
     */
    //@{
    /**
     * What type is it?
     *
     * @return A string name.
     */
    virtual wxString                    GetType() const;

    /**
     * Override these to provide common functionality.
     *
     * @param _rData Variant to test if is equal to this.
     * @return always return false, never test all columns equality.
     */
    virtual bool                        Eq(wxVariantData& _rData) const;

    /**
     * Used to convert to wxString.
     *
     * Return the long long value as string.
     *
     * @param _rstrValue String value to set with this content.
     * @return Always true.
     */
    virtual bool                        Write(wxString & _rstrValue) const;
    //@}
};

#endif // INCLUDE_WX_ETK_SQLITE3_WX_VARIANTS_H
