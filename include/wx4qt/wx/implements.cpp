/////////////////////////////////////////////////////////////////////////////
// Name:        wx/implements.h
// Purpose:     Implementations of wx classes to be used with QT
// Library:     Copied from wxWidgets, modifyed to be used with QT library
// Author:      Stéphane Château
// Modified by:
// Created:     14/06/2012
// Copyright:   (c) Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wxtype.h"
#include "wx.h"
#include "confbase.h"
#include "tokenzr.h"
#include "string.h"
#include "filename.h"
#include <time.h>

//----------------------------------------------------------------------------------------------------------------
//--                                            wxConfigBase                                                    --
//----------------------------------------------------------------------------------------------------------------
// Static instance of wxConfigBase
wxConfigBase    wxConfigBase::m_sSettings;

wxConfigBase *wxConfigBase::Get()
{
    return &m_sSettings;
}

void wxConfigBase::Set(QSettings *_pSettings)
{   // Replace pointer
    m_sSettings.m_pQSettings = _pSettings;
}

void wxConfigBase::SetPath(const wxChar *_pszPath)
{
    m_strGroupName = _pszPath;
}

wxString wxConfigBase::Read(const wxChar *_pszKey,const wxChar *_pszDefault)
{
    if (m_pQSettings != NULL)
    {
        m_pQSettings->beginGroup(m_strGroupName);
        wxString strRet(m_pQSettings->value(_pszKey).toString());
        m_pQSettings->endGroup();
        return strRet;
    }
    
    wxASSERT_MSG(false,wxT("m_pQSettings is not initialized, CALL wxConfigBase::Set function!"));
    return wxString();
}

void wxConfigBase::Write(const wxChar *_pszKey,const wxChar *_pszValue)
{
    wxASSERT_MSG(m_pQSettings != NULL,wxT("m_pQSettings is not initialized, CALL wxConfigBase::Set function!"));

    if (m_pQSettings != NULL)
    {
        m_pQSettings->beginGroup(m_strGroupName);
        m_pQSettings->setValue(_pszKey,_pszValue);
        m_pQSettings->endGroup();
    }
}

//----------------------------------------------------------------------------------------------------------------
//--                                                Logs                                                        --
//----------------------------------------------------------------------------------------------------------------
// Logs
wx4qtLogger * wx4qtLogger::m_pInstance = NULL;
wx4qtLogger  *wx4qtLogger::Getwx4qtLoggerInstance()
{
    return m_pInstance;
}

wx4qtLogger::wx4qtLogger()
{
    m_pInstance = this;
}

wx4qtLogger::~wx4qtLogger()
{
    m_pInstance = NULL;
}

void wx4qtLogger::LogDebug(QString _strLogString)
{
}

void wx4qtLogger::wxLogDebug(QString _strLogString)
{
}

void wx4qtLogger::wxLogVerbose(QString _strLogString)
{
}

void wx4qtLogger::wxLogError(QString _strLogString)
{
}

void wx4qtLogger::wxLogFatalError(QString _strLogString)
{
}

void wxLogDebug(const wxChar *_pszMessage)
{
    if (wx4qtLogger::Getwx4qtLoggerInstance() != NULL)
    {
        wx4qtLogger::Getwx4qtLoggerInstance()->wxLogDebug(QString(_pszMessage));
    }
}

void wxLogDebug(const wxChar *_pszFormat,const wxChar *_pszMessage)
{
    wxLogDebug(wxString::Format(_pszFormat,_pszMessage));
}

void wxLogVerbose(const wxChar *_pszMessage)
{
    if (wx4qtLogger::Getwx4qtLoggerInstance() != NULL)
    {
        wx4qtLogger::Getwx4qtLoggerInstance()->wxLogVerbose(QString(_pszMessage));
    }
}

void wxLogVerbose(const wxChar *_pszFormat,const wxChar *_pszMessage)
{
    wxLogVerbose(wxString::Format(_pszFormat,_pszMessage));
}

void wxLogError(const wxChar *_pszMessage)
{
    if (wx4qtLogger::Getwx4qtLoggerInstance() != NULL)
    {
        wx4qtLogger::Getwx4qtLoggerInstance()->wxLogError(QString(_pszMessage));
    }
}

void wxLogError(const wxChar *_pszFormat,const wxChar *_pszMessage)
{
    wxLogError(wxString::Format(_pszFormat,_pszMessage));
}

void wxLogFatalError(const wxChar *_pszMessage)
{
    if (wx4qtLogger::Getwx4qtLoggerInstance() != NULL)
    {
        wx4qtLogger::Getwx4qtLoggerInstance()->wxLogFatalError(QString(_pszMessage));
    }
}

void wxLogFatalError(const wxChar *_pszFormat,const wxChar *_pszMessage)
{
    wxLogFatalError(wxString::Format(_pszFormat,_pszMessage));
}

//----------------------------------------------------------------------------------------------------------------
//--                                         wxMemoryBufferData                                                 --
//----------------------------------------------------------------------------------------------------------------
void wxMemoryBufferData::ResizeIfNeeded(size_t newSize)
{
    if (newSize > m_size)
    {
        void *dataOld = m_data;
        m_data = realloc(m_data, newSize + wxMemoryBufferData::DefBufSize);
        if ( !m_data )
        {
            free(dataOld);
        }

        m_size = newSize + wxMemoryBufferData::DefBufSize;
    }
}

//----------------------------------------------------------------------------------------------------------------
//--                                           wxMemoryBuffer                                                   --
//----------------------------------------------------------------------------------------------------------------
void wxMemoryBuffer::AppendByte(char data)
{
    wxCHECK_RET( m_bufdata->m_data, wxT("invalid wxMemoryBuffer") );

    m_bufdata->ResizeIfNeeded(m_bufdata->m_len + 1);
    *(((char*)m_bufdata->m_data) + m_bufdata->m_len) = data;
    m_bufdata->m_len += 1;
}

//----------------------------------------------------------------------------------------------------------------
//--                                             wxQDateTime                                                    --
//----------------------------------------------------------------------------------------------------------------
const wxChar * wxQDateTime::ParseDate(const QString &_rString)
{
    QDate dtComputed = QDate::fromString(_rString,Qt::ISODate);
    if (!dtComputed.isValid())
    {
        return NULL;
    }
    else
    {
        setDate(dtComputed);
        setTime(QTime(12,0,0,0));
    }
    m_strParsedDate = toString(Qt::ISODate);
    return m_strParsedDate.c_str();
}

const wxChar * wxQDateTime::ParseTime(const QString &_rString)
{
    QTime dtComputed = QTime::fromString(_rString,Qt::ISODate);
    if (!dtComputed.isValid())
    {
        return NULL;
    }
    else
    {
        setTime(dtComputed);
        setDate(QDate(2000,10,10)); // Don't care about date, just have a valid date
    }
    m_strParsedDate = toString(Qt::ISODate);
    return m_strParsedDate.c_str();
}

const wxChar * wxQDateTime::ParseDateTime(const QString &_rString)
{
    QDateTime dtComputed = QDateTime::fromString(_rString,Qt::ISODate);
    if (!dtComputed.isValid())
    {
        return NULL;
    }
    else
    {
        setTime(dtComputed.time());
        setDate(dtComputed.date());
    }
    m_strParsedDate = toString(Qt::ISODate);
    return m_strParsedDate.c_str();
}


wxString wxQDateTime::FormatISODate() const
{
    return date().toString(Qt::ISODate);
}

wxString wxQDateTime::FormatISOTime() const
{
    return time().toString(Qt::ISODate);
}

wxQDateTime::operator QDate () const
{
    return date();
}

wxQDateTime::operator QTime () const
{
    return time();
}

wxString wxQDateTime::Format(const char *_pszFormat) const
{
    QDate qdate = date();
    QTime qtime = time();

    wxString strReturnDT(_pszFormat);

    // Replace all %% by a tag that does not exits
    strReturnDT.Replace(wxT("%%"),wxT("¤$£%~ù"));

    // %a: Abbreviated weekday name
    strReturnDT.Replace(wxT("%a"),QDate::shortDayName(qdate.dayOfWeek()).toAscii());

    // %A: Full weekday name
    strReturnDT.Replace(wxT("%A"),QDate::longDayName(qdate.dayOfWeek()).toAscii());

    // %b: Abbreviated month name
    strReturnDT.Replace(wxT("%b"),QDate::shortMonthName(qdate.month()).toAscii());

    // %B: Full month name
    strReturnDT.Replace(wxT("%B"),QDate::longMonthName(qdate.dayOfWeek()).toAscii());

    // %c: Date and time representation appropriate for locale
    // Not supported

    // %d: Day of month as decimal number (01 – 31)
    strReturnDT.Replace(wxT("%d"),wxString::Format(wxT("%02d"),qdate.day()).toAscii());

    // %H: Hour in 24-hour format (00 – 23)
    strReturnDT.Replace(wxT("%H"),wxString::Format(wxT("%02d"),qtime.hour()).toAscii());

    // %I: Hour in 12-hour format (01 – 12)
    strReturnDT.Replace(wxT("%I"),wxString::Format(wxT("%02d"),(qtime.hour() == 0) ? 12 : qtime.hour() % 12).toAscii());

    // %j: Day of year as decimal number (001 – 366)
    strReturnDT.Replace(wxT("%j"),QString("%1").arg(qdate.dayOfYear()).toAscii());

    // %m: Month as decimal number (01 – 12)
    strReturnDT.Replace(wxT("%m"),wxString::Format(wxT("%02d"),qdate.month()).toAscii());

    // %M: Minute as decimal number (00 – 59)
    strReturnDT.Replace(wxT("%M"),wxString::Format(wxT("%02d"),qtime.minute()).toAscii());

    // %p: Current locale's A.M./P.M. indicator for 12-hour clock
    strReturnDT.Replace(wxT("%p"),qtime.hour() > 11 ? "AM" : "PM");

    // %S: Second as decimal number (00 – 59)
    strReturnDT.Replace(wxT("%S"),wxString::Format(wxT("%02d"),qtime.second()).toAscii());

    // %U: Week of year as decimal number, with Sunday as first day of week (00 – 53)
    // Not supported

    // %w: Weekday as decimal number (0 – 6; Sunday is 0)
    strReturnDT.Replace(wxT("%w"),QString("%1").arg(qdate.dayOfWeek() == 7 ? 0 : qdate.dayOfWeek()).toAscii());

    // %W: Week of year as decimal number, with Monday as first day of week (00 – 53)
    int iYear = qdate.year();
    strReturnDT.Replace(wxT("%W"),wxString::Format(wxT("%02d"),qdate.weekNumber(&iYear) - 1).toAscii());

    // %x: Date representation for current locale
    strReturnDT.Replace(wxT("%x"),QString("%1").arg(date().toString(Qt::ISODate)).toAscii());

    // %X: Time representation for current locale
    strReturnDT.Replace(wxT("%x"),QString("%1").arg(time().toString(Qt::ISODate)).toAscii());

    // %y : Year without century, as decimal number (00 – 99)
    strReturnDT.Replace(wxT("%y"),wxString::Format(wxT("%02d"),qdate.year() % 100).toAscii());

    // %Y: Year with century, as decimal number
    strReturnDT.Replace(wxT("%Y"),wxString::Format(wxT("%04d"),qdate.year()).toAscii());

    // %z, %Z: Either the time-zone name or time zone abbreviation, depending on registry settings; no characters if time zone is unknown
    // Not supported

    // Replace all %% by a tag that does not exits
    strReturnDT.Replace(wxT("¤$£%~ù"),wxT("%%"));

    return strReturnDT;
}

//----------------------------------------------------------------------------------------------------------------
//--                                               wxFile                                                      --
//----------------------------------------------------------------------------------------------------------------
bool wxFileExists(const wxChar *_pszFilePath)
{
    QFile file(_pszFilePath);
    return file.exists();
}

bool wxRemoveFile(const wxChar *_pszFilePath)
{
    QFile file(_pszFilePath);
    return file.remove();
}

//----------------------------------------------------------------------------------------------------------------
//--                                              wxString                                                      --
//----------------------------------------------------------------------------------------------------------------
// Static member
const size_t wxString::npos = (size_t) -1;

bool wxIsspace(wxChar _cChar)
{
    return _cChar == wxT(' ');
}

#ifdef UNICODE
    #define wxStrlen_   wcslen
#else
    #define wxStrlen_   strlen
#endif

/* safe version of strlen() (returns 0 if passed NULL pointer) */
size_t wxStrlen(const wxChar *psz)
{
    return psz ? strlen(psz) : 0;
}

//implement our own wmem variants
wxChar* wxTmemchr(const wxChar* s, wxChar c, size_t l)
{
    for(;l && *s != c;--l, ++s);

    if (l)
    {
        return (wxChar*)s;
    }

    return NULL;
}

size_t wxString::find_first_not_of(const wxChar* sz, size_t nStart) const
{
    if ( nStart == npos )
    {
        nStart = length();
    }
    else
    {
        wxASSERT( nStart <= (size_t) length() );
    }

    size_t len = strlen(sz);

    size_t i;
    for(i = nStart; i < (size_t) this->length(); ++i)
    {
        if (!wxTmemchr(sz, *(c_str() + i), len))
            break;
    }

    if(i == (size_t) this->length())
         return npos;
     else
        return i;
}

size_t wxString::find_first_of(const wxChar* sz, size_t nStart) const
{
    wxASSERT(nStart <= (size_t) length());

    size_t len = wxStrlen(sz);

    size_t i;
    for(i = nStart; i < (size_t) this->length(); ++i)
    {
        if (wxTmemchr(sz, *(c_str() + i), len))
            break;
    }

    if(i == (size_t) this->length())
        return npos;
    else
        return i;
}

QString wxString::Printf(const char *_pszFormat,...)
{
    va_list valistArguments;
    va_start(valistArguments,_pszFormat);
    return vsprintf(_pszFormat,valistArguments);
}

wxQString wxString::Format(const char *_pszFormat,...)
{
    QString strReturn;

    va_list valistArguments;
    va_start(valistArguments,_pszFormat);
    return strReturn.vsprintf (_pszFormat,valistArguments);
}

wxString::operator const char *() const
{
    m_convertToChar = toAscii();

    return m_convertToChar;
}

const wxChar *wxString::wx_str() const
{
    m_convertToChar = toAscii();

    return m_convertToChar;
}

const wxChar *wxString::c_str() const
{
    m_convertToChar = toAscii();

    return m_convertToChar;
}

//----------------------------------------------------------------------------------------------------------------
//--                                          wxStringTokenizer                                                 --
//----------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// access to the tokens
// ----------------------------------------------------------------------------
wxStringTokenizer::wxStringTokenizer(const wxString& str,
                                     const wxString& delims,
                                     wxStringTokenizerMode mode)
{
    SetString(str, delims, mode);
}

void wxStringTokenizer::SetString(const wxString& str,
                                  const wxString& delims,
                                  wxStringTokenizerMode mode)
{
    if ( mode == wxTOKEN_DEFAULT )
    {
        // by default, we behave like strtok() if the delimiters are only
        // whitespace characters and as wxTOKEN_RET_EMPTY otherwise (for
        // whitespace delimiters, strtok() behaviour is better because we want
        // to count consecutive spaces as one delimiter)
        const wxChar *p;
        for ( p = delims.c_str(); *p; p++ )
        {
            if ( !wxIsspace(*p) )
                break;
        }

        if ( *p )
        {
            // not whitespace char in delims
            mode = wxTOKEN_RET_EMPTY;
        }
        else
        {
            // only whitespaces
            mode = wxTOKEN_STRTOK;
        }
    }

    m_delims = delims;
    m_mode = mode;

    Reinit(str);
}

void wxStringTokenizer::Reinit(const wxString& str)
{
    wxASSERT( IsOk() ); // you should call SetString() first

    m_string = str;
    m_pos = 0;
    m_lastDelim = wxT('\0');
}

// do we have more of them?
bool wxStringTokenizer::HasMoreTokens() const
{
    Q_ASSERT(IsOk()); // you should call SetString() first

    if ( m_string.find_first_not_of(m_delims, m_pos) != wxString::npos )
    {
        // there are non delimiter characters left, so we do have more tokens
        return true;
    }

    switch ( m_mode )
    {
        case wxTOKEN_RET_EMPTY:
        case wxTOKEN_RET_DELIMS:
            // special hack for wxTOKEN_RET_EMPTY: we should return the initial
            // empty token even if there are only delimiters after it
            return m_pos == 0 && !m_string.IsEmpty();

        case wxTOKEN_RET_EMPTY_ALL:
            // special hack for wxTOKEN_RET_EMPTY_ALL: we can know if we had
            // already returned the trailing empty token after the last
            // delimiter by examining m_lastDelim: it is set to NUL if we run
            // up to the end of the string in GetNextToken(), but if it is not
            // NUL yet we still have this last token to return even if m_pos is
            // already at m_string.length()
            return m_pos < (size_t) m_string.length() || m_lastDelim != wxT('\0');

        case wxTOKEN_INVALID:
        case wxTOKEN_DEFAULT:
            wxFAIL_MSG( wxT("unexpected tokenizer mode") );
            // fall through

        case wxTOKEN_STRTOK:
            // never return empty delimiters
            break;
    }

    return false;
}

// count the number of (remaining) tokens in the string
size_t wxStringTokenizer::CountTokens() const
{
    Q_ASSERT(IsOk()); // you should call SetString() first

    // VZ: this function is IMHO not very useful, so it's probably not very
    //     important if its implementation here is not as efficient as it
    //     could be -- but OTOH like this we're sure to get the correct answer
    //     in all modes
    wxStringTokenizer tkz(m_string.c_str() + m_pos, m_delims, m_mode);

    size_t count = 0;
    while ( tkz.HasMoreTokens() )
    {
        count++;

        (void)tkz.GetNextToken();
    }

    return count;
}

// ----------------------------------------------------------------------------
// token extraction
// ----------------------------------------------------------------------------

wxString wxStringTokenizer::GetNextToken()
{
    wxString token;
    do
    {
        if ( !HasMoreTokens() )
        {
            break;
        }

        // find the end of this token
        size_t pos = m_string.find_first_of(m_delims, m_pos);

        // and the start of the next one
        if ( pos == wxString::npos )
        {
            // no more delimiters, the token is everything till the end of
            // string
            token = m_string.SubString(m_pos, wxString::npos);

            // skip the token
            m_pos = m_string.length();

            // it wasn't terminated
            m_lastDelim = wxT('\0');
        }
        else // we found a delimiter at pos
        {
            // in wxTOKEN_RET_DELIMS mode we return the delimiter character
            // with token, otherwise leave it out
            size_t len = pos - m_pos;
            if ( m_mode == wxTOKEN_RET_DELIMS )
                len++;

            token = m_string.SubString(m_pos, len);

            // skip the token and the trailing delimiter
            m_pos = pos + 1;

            m_lastDelim = m_string[pos];
        }
    }
    while ( !AllowEmpty() && token.IsEmpty() );

    return token;
}

//----------------------------------------------------------------------------------------------------------------
//--                                             wxVariant                                                      --
//----------------------------------------------------------------------------------------------------------------
wxVariant::wxVariant()
    : m_pData(NULL)
{
}

wxVariant::wxVariant(const wxVariant &other)
    : QVariant(other)
    , m_pData(other.m_pData)
{
    if (m_pData != NULL)
    {   // If not NULL increment reference count
        m_pData->IncRef();
    }
}

wxVariant::wxVariant(int i)
    : QVariant(i)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(long i)
    : QVariant(qlonglong(i))
    , m_pData(NULL)
{
}

wxVariant::wxVariant(uint ui)
    : QVariant(ui)
    , m_pData(NULL)
{
}
wxVariant::wxVariant(qlonglong ll)
    : QVariant(ll)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(qulonglong ull)
    : QVariant(ull)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(bool b)
    : QVariant(b)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(double d)
    : QVariant(d)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(float f)
    : QVariant(f)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(const QString &string)
    : QVariant(string)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(const QDate &date)
    : QVariant(date)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(const QTime &time)
    : QVariant(time)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(const QDateTime &datetime)
    : QVariant(datetime)
    , m_pData(NULL)
{
}

wxVariant::wxVariant(const struct tm &date)
    : QVariant(QDateTime(QDate(date.tm_year, date.tm_mon, date.tm_mday), QTime(date.tm_hour, date.tm_min,date.tm_sec, 0),Qt::LocalTime))
    , m_pData(NULL)
{
}

wxVariant::wxVariant(wxVariantData *pdata)
    : QVariant(),m_pData(pdata)
{
    if (m_pData != NULL)
    {
        m_pData->IncRef();
    }
}

wxVariant::~wxVariant()
{
    if (m_pData != NULL)
    {
        m_pData->DecRef();
    }
}

wxString wxVariant::GetString() const
{
    wxString strValue;
    if (m_pData != NULL)
    {
        m_pData->Write(strValue);
    }
    else
    { 
        strValue = toString();
    } 
    return strValue;
}

double wxVariant::GetDouble() const
{
    return toDouble();
}

wxDateTime wxVariant::GetDateTime() const
{
    return wxDateTime(toDateTime());
}

wxVariantData *wxVariant::GetData() const
{
    if (m_pData != NULL)
    {   // If not NULL return the data
        return m_pData;
    }
    return (wxVariantData *) this;
}

void wxVariant::SetData(wxVariantData *_pData)
{
    if (m_pData != NULL)
    {
        m_pData->DecRef();
    }
    m_pData=_pData;
    if (m_pData != NULL)
    {
        m_pData->IncRef();
    }
}
bool wxVariant::IsNull() const
{
    return isNull();
}

void wxVariant::Clear()
{
    clear();
    if (m_pData != NULL)
    {
        m_pData->IncRef();
        m_pData = NULL;
    }
}

wxString wxVariant::GetType() const
{
    wxString  strType;
    if (m_pData != NULL)
    {
        strType = m_pData->GetType();
    }
    else
    {
        strType = QVariant::typeName();
    }
    return strType;
}

bool wxVariant::Eq(wxVariantData& data) const
{
    wxVariant *pVariant = dynamic_cast<wxVariant *>(&data);
    if (pVariant == NULL)
    {   // The data is not a QVariant, test only if m_pData is not null
        return (m_pData != NULL) && m_pData->Eq(data);
    }
    else
    {
        if (GetType() == data.GetType())
        {   // Only if same type
            if (GetData() != NULL && pVariant->GetData() != NULL)
            {   // If both are wxVariantData type
                return GetData()->Eq(*pVariant->GetData());
            }
            else if (GetData()== NULL && pVariant->GetData() == NULL)
            {   // If both h NOT type
                return QVariant::operator ==(*pVariant);
            }
        }
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------
//--                                        Binding QT Classes                                                  --
//----------------------------------------------------------------------------------------------------------------

#include "wxETKSQLite3ValueBind.h"
#include "wx/wxSQLite3.h"

// Bind QT types : export all full template specialization
template<> void wxETKSQLite3ValueBindOther<qint64>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const
{
    _rstmt.Bind(_iIndex,wxLongLong(*m_pDataPtr));
}

template<> void wxETKSQLite3ValueBindOther<qint64>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
{
    wxASSERT_MSG(!_rResultSet.IsNull(_iIndex),wxT("BindFrom must not be called on null column!"));
    *m_pDataPtr = _rResultSet.GetInt64(_iIndex).GetValue();
}

template<> void wxETKSQLite3ValueBindOther<QString>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const
{
    _rstmt.Bind(_iIndex,wxString(*m_pDataPtr));
}

template<> void wxETKSQLite3ValueBindOther<QString>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
{
    wxASSERT_MSG(!_rResultSet.IsNull(_iIndex),wxT("BindFrom must not be called on null column!"));
    *m_pDataPtr = _rResultSet.GetAsString(_iIndex);
}

template<> void wxETKSQLite3ValueBindOther<QDate>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const
{
    _rstmt.BindDate(_iIndex,wxDateTime(*m_pDataPtr));
}

template<> void wxETKSQLite3ValueBindOther<QDate>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
{
    wxASSERT_MSG(!_rResultSet.IsNull(_iIndex),wxT("BindFrom must not be called on null column!"));
    *m_pDataPtr = _rResultSet.GetDate(_iIndex).date();
}

template<> void wxETKSQLite3ValueBindOther<QTime>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const
{
    _rstmt.BindTime(_iIndex,wxDateTime(*m_pDataPtr));
}

template<> void wxETKSQLite3ValueBindOther<QTime>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
{
    wxASSERT_MSG(!_rResultSet.IsNull(_iIndex),wxT("BindFrom must not be called on null column!"));
    *m_pDataPtr = _rResultSet.GetTime(_iIndex).time();
}

template<> void wxETKSQLite3ValueBindOther<QDateTime>::BindTo(wxSQLite3Statement &_rstmt,int _iIndex,bool _bAddForInsertRequest,bool _bForInsertRequest) const
{
    _rstmt.BindDateTime(_iIndex,wxDateTime(*m_pDataPtr));
}

template<> void wxETKSQLite3ValueBindOther<QDateTime>::BindFrom(wxSQLite3ResultSet &_rResultSet,int _iIndex) const
{
    wxASSERT_MSG(!_rResultSet.IsNull(_iIndex),wxT("BindFrom must not be called on null column!"));
    *m_pDataPtr = _rResultSet.GetDateTime(_iIndex);
}
