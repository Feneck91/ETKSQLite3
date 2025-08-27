/////////////////////////////////////////////////////////////////////////////
// Name:        wx/buffer.h
// Purpose:     Array string
// Library:     Copied from wxWidgets, modifyed to be used with QT library
// Author:      Stéphane Château
// Modified by:
// Created:     14/06/2012
// Copyright:   (c) Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_BUFFER_HEADER
#define WX_BUFFER_HEADER

#include "wx.h"
#include <QBuffer>
#include <QVector>

/**
 * Replace wxQVector by QVector.
 */
template<typename TYPE> class wxQVector : public QVector<TYPE>
{
public:
    size_t          GetCount() const                { return QVector<TYPE>::size(); }
    const TYPE &    Item(int _iItemIndex) const     { return QVector<TYPE>::at(_iItemIndex); }
    void            Add(const TYPE &_rItem)         { QVector<TYPE>::append(_rItem); }
    void            Empty()                         { QVector<TYPE>::clear(); }
    size_t          Count() const                   { return QVector<TYPE>::size(); }
};

#define wxMemoryBuffer                          wxQBuffer
#define wxArrayInt                              wxQVector<int>

/**
  * This class manages the actual data buffer pointer and is ref-counted.
  */
class EXPORT_IMPORT wxMemoryBufferData
{
public:
    // the initial size and also the size added by ResizeIfNeeded()
    enum { DefBufSize = 1024 };

    friend class wxMemoryBuffer;

    // everyting is private as it can only be used by wxMemoryBuffer
private:
    wxMemoryBufferData(size_t size = wxMemoryBufferData::DefBufSize) : m_data(size ? malloc(size) : NULL), m_size(size), m_len(0), m_ref(0) {}
    ~wxMemoryBufferData()                                                               { free(m_data); }
    void ResizeIfNeeded(size_t newSize);
    void IncRef()                                                                       { m_ref += 1; }
    void DecRef()                                                                       { m_ref -= 1; if (m_ref == 0) delete this; }

    // the buffer containing the data
    void  *m_data;

    // the size of the buffer
    size_t m_size;

    // the amount of data currently in the buffer
    size_t m_len;

    // the reference count
    size_t m_ref;

    DECLARE_NO_COPY_CLASS(wxMemoryBufferData)
};

class EXPORT_IMPORT wxMemoryBuffer
{
public:
    // ctor and dtor
    wxMemoryBuffer(size_t size = wxMemoryBufferData::DefBufSize)                        { m_bufdata = new wxMemoryBufferData(size); m_bufdata->IncRef(); }
    ~wxMemoryBuffer()                                                                   { m_bufdata->DecRef(); }

    // copy and assignment
    wxMemoryBuffer(const wxMemoryBuffer& src) : m_bufdata(src.m_bufdata)                { m_bufdata->IncRef(); }

    wxMemoryBuffer& operator=(const wxMemoryBuffer& src)                                { m_bufdata->DecRef(); m_bufdata = src.m_bufdata; m_bufdata->IncRef(); return *this; }

    // Accessors
    void  *GetData() const                                                              { return m_bufdata->m_data; }
    size_t GetBufSize() const                                                           { return m_bufdata->m_size; }
    size_t GetDataLen() const                                                           { return m_bufdata->m_len; }

    void SetBufSize(size_t size)                                                        { m_bufdata->ResizeIfNeeded(size); }
    void SetDataLen(size_t len)                                                         { wxASSERT(len <= m_bufdata->m_size); m_bufdata->m_len = len; }

    // Ensure the buffer is big enough and return a pointer to it
    void *GetWriteBuf(size_t sizeNeeded)                                                { m_bufdata->ResizeIfNeeded(sizeNeeded); return m_bufdata->m_data; }

    // Update the length after the write
    void UngetWriteBuf(size_t sizeUsed)                                                 { SetDataLen(sizeUsed); }

    // Like the above, but appends to the buffer
    void *GetAppendBuf(size_t sizeNeeded)                                               { m_bufdata->ResizeIfNeeded(m_bufdata->m_len + sizeNeeded); return (char*)m_bufdata->m_data + m_bufdata->m_len; }

    // Update the length after the append
    void UngetAppendBuf(size_t sizeUsed)                                                { SetDataLen(m_bufdata->m_len + sizeUsed); }

    // Other ways to append to the buffer
    void AppendByte(char data);
    void AppendData(const void *data, size_t len)                                       { memcpy(GetAppendBuf(len), data, len); UngetAppendBuf(len); }

    operator const char *() const                                                       { return (const char*)GetData(); }

private:
    wxMemoryBufferData* m_bufdata;
};

#endif // WX_BUFFER_HEADER
