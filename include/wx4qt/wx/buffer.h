/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/buffer.h
 * @brief Header file for Vector.
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
#ifndef WX_BUFFER_HEADER
#define WX_BUFFER_HEADER

#include <wx/wx.h>
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

#define wxArrayInt                              wxQVector<int>

/**
 * This class manages the actual data buffer pointer and is ref-counted.
 */
class EXPORT_IMPORT wxMemoryBufferData
{
public:
    // the initial size and also the size added by ResizeIfNeeded()
    enum
    {
        DefBufSize = 1024
    };

    friend class wxMemoryBuffer;

    // everyting is private as it can only be used by wxMemoryBuffer
private:
    wxMemoryBufferData(size_t size = wxMemoryBufferData::DefBufSize);
    ~wxMemoryBufferData();
    void ResizeIfNeeded(size_t newSize);

    void IncRef();
    void DecRef();

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
    wxMemoryBuffer(size_t size = wxMemoryBufferData::DefBufSize);
    ~wxMemoryBuffer();

    // copy and assignment
    wxMemoryBuffer(const wxMemoryBuffer& src);

    wxMemoryBuffer& operator=(const wxMemoryBuffer& src);

    // Accessors
    void  *GetData() const;
    size_t GetBufSize() const;
    size_t GetDataLen() const;

    void SetBufSize(size_t size);
    void SetDataLen(size_t len);

    // Ensure the buffer is big enough and return a pointer to it
    void *GetWriteBuf(size_t sizeNeeded);

    // Update the length after the write
    void UngetWriteBuf(size_t sizeUsed);

    // Like the above, but appends to the buffer
    void *GetAppendBuf(size_t sizeNeeded);

    // Update the length after the append
    void UngetAppendBuf(size_t sizeUsed);

    // Other ways to append to the buffer
    void AppendByte(char data);

    void AppendData(const void *data, size_t len);

    operator const char *() const;

private:
    wxMemoryBufferData* m_bufdata;
};

#endif // WX_BUFFER_HEADER