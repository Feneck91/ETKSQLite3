/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/buffer.cpp
 * @brief Implementation file for Vector.
 *
 * Vector, simul wxWidget class to be used with other frameworks.
 *
 * @author Stéphane Château
 * @date Created: 2012/06/14
 * @date Modified: 2025/09/01
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#include "wx/buffer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                        wxMemoryBufferData                                             //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
wxMemoryBufferData::wxMemoryBufferData(size_t size)
    : m_data(size ? malloc(size) : nullptr)
    , m_size(size)
    , m_len(0)
    , m_ref(0)
{
}

wxMemoryBufferData::~wxMemoryBufferData()
{
    free(m_data);
}

void wxMemoryBufferData::ResizeIfNeeded(size_t newSize)
{
    if (newSize > m_size)
    {
        void *dataOld = m_data;
        m_data = realloc(m_data, newSize + wxMemoryBufferData::DefBufSize);
        if (m_data == nullptr)
        {
            free(dataOld);
        }

        m_size = newSize + wxMemoryBufferData::DefBufSize;
    }
}

void wxMemoryBufferData::IncRef()
{
    m_ref += 1;
}

void wxMemoryBufferData::DecRef()
{
    m_ref -= 1;
    if (m_ref == 0)
    {
        delete this;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                          wxMemoryBuffer                                               //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ctor and dtor
wxMemoryBuffer::wxMemoryBuffer(size_t size)
{
    m_bufdata = new wxMemoryBufferData(size);
    m_bufdata->IncRef();
}

wxMemoryBuffer::~wxMemoryBuffer()
{
    m_bufdata->DecRef();
}

// copy and assignment
wxMemoryBuffer::wxMemoryBuffer(const wxMemoryBuffer& src)
    : m_bufdata(src.m_bufdata)
{
    m_bufdata->IncRef();
}

wxMemoryBuffer& wxMemoryBuffer::operator=(const wxMemoryBuffer& src)
{
    m_bufdata->DecRef();
    m_bufdata = src.m_bufdata;
    m_bufdata->IncRef();

    return *this;
}

// Accessors
void * wxMemoryBuffer::GetData() const
{
    return m_bufdata->m_data;
}

size_t wxMemoryBuffer::GetBufSize() const
{
    return m_bufdata->m_size;
}

size_t wxMemoryBuffer::GetDataLen() const
{
    return m_bufdata->m_len;
}

void wxMemoryBuffer::SetBufSize(size_t size)
{
    m_bufdata->ResizeIfNeeded(size);
}

void wxMemoryBuffer::SetDataLen(size_t len)
{
    wxASSERT(len <= m_bufdata->m_size);
    m_bufdata->m_len = len;
}

// Ensure the buffer is big enough and return a pointer to it
void *wxMemoryBuffer::GetWriteBuf(size_t sizeNeeded)
{
    m_bufdata->ResizeIfNeeded(sizeNeeded);
    return m_bufdata->m_data;
}

// Update the length after the write
void wxMemoryBuffer::UngetWriteBuf(size_t sizeUsed)
{
    SetDataLen(sizeUsed);
}

// Like the above, but appends to the buffer
void *wxMemoryBuffer::GetAppendBuf(size_t sizeNeeded)
{
    m_bufdata->ResizeIfNeeded(m_bufdata->m_len + sizeNeeded);
    return (char*)m_bufdata->m_data + m_bufdata->m_len;
}

// Update the length after the append
void wxMemoryBuffer::UngetAppendBuf(size_t sizeUsed)
{
    SetDataLen(m_bufdata->m_len + sizeUsed);
}

// Other ways to append to the buffer
void wxMemoryBuffer::AppendByte(char data)
{
    wxCHECK_RET( m_bufdata->m_data, wxT("invalid wxMemoryBuffer") );

    m_bufdata->ResizeIfNeeded(m_bufdata->m_len + 1);
    *(((char*)m_bufdata->m_data) + m_bufdata->m_len) = data;
    m_bufdata->m_len += 1;
}

void wxMemoryBuffer::AppendData(const void *data, size_t len)
{
    memcpy(GetAppendBuf(len), data, len);
    UngetAppendBuf(len);
}


wxMemoryBuffer::operator const char *() const
{
    return (const char*) GetData();
}