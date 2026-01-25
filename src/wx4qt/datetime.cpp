/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/datetime.cpp
 * @brief Implementation file for wxDateTime class.
 *
 * Inspired by wxWidgets, modifyed to be used with QT library.
 *
 * @author Stéphane Château
 * @date Created: 2025/09/02
 * @date Modified:
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#include <wx/wx.h>
#include <wx/wxtype.h>
#include <wx/datetime.h>
#include <QLocale>
#include <QTimeZone>

wxQDateTime::wxQDateTime::wxQDateTime()
{
}

wxQDateTime::wxQDateTime(const wxQDateTime &_rDT) : QDateTime(_rDT)
{
}

wxQDateTime::wxQDateTime(const QDateTime &_rDT) : QDateTime(_rDT)
{
}

wxQDateTime::wxQDateTime(wxLongLong &_dt) : QDateTime(QDateTime::fromMSecsSinceEpoch(_dt.GetValue()))
{
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)

wxQDateTime::wxQDateTime(double _dValue) : QDateTime(QDate::fromJulianDay(_dValue).startOfDay())
{
}

wxQDateTime::wxQDateTime(const QDate & _rDate): QDateTime(_rDate.startOfDay())
{
}

#else

wxQDateTime::wxQDateTime(double _dValue) : QDateTime(QDate::fromJulianDay(_dValue))
{
}

wxQDateTime::wxQDateTime(const QDate & _rDate): QDateTime(_rDate)
{
}

#endif

wxQDateTime::wxQDateTime(const struct tm& date)
    : QDateTime(QDate(date.tm_year + 1900, date.tm_mon + 1, date.tm_mday),
                QTime(date.tm_hour, date.tm_min, date.tm_sec, 0),
                QTimeZone::systemTimeZone())
{
}

wxQDateTime::wxQDateTime(const QTime& _rTime)
    : QDateTime(QDate::currentDate(),
                _rTime,
                QTimeZone::systemTimeZone())
{
}

wxQDateTime::operator QDate () const
{
    return date();
}

wxQDateTime::operator QTime () const
{
    return time();
}

void wxQDateTime::SetMillisecond(int _iMillisecond)
{
    setTime(QTime(time().hour(),time().minute(),time().second(),_iMillisecond));
}

bool wxQDateTime::IsValid() const
{
    return isValid();
}

// This function returns the date representation in the ISO 8601 format (YYYY-MM-DD).
wxString wxQDateTime::FormatISODate() const
{
    return date().toString(Qt::ISODate);
}

// This function returns the time representation in the ISO 8601 format (HH:MM:SS).
wxString wxQDateTime::FormatISOTime() const
{
    return time().toString(Qt::ISODate);
}

wxString wxQDateTime::Format(const char *_pszFormat) const
{
    QDate qdate = date();
    QTime qtime = time();

    wxString strReturnDT(_pszFormat);

    // Replace all %% by a tag that does not exits
    strReturnDT.Replace(wxT("%%"),wxT("¤$£%~ù"));

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    // %a: Abbreviated weekday name
    strReturnDT.Replace(wxT("%a"), QLocale::system().dayName(qdate.dayOfWeek(), QLocale::ShortFormat).toUtf8());

    // %A: Full weekday name
    strReturnDT.Replace(wxT("%A"), QLocale::system().dayName(qdate.dayOfWeek(), QLocale::LongFormat).toUtf8());

    // %b: Abbreviated month name
    strReturnDT.Replace(wxT("%b"), QLocale::system().monthName(qdate.month(), QLocale::ShortFormat).toUtf8());

    // %B: Full month name
    strReturnDT.Replace(wxT("%B"), QLocale::system().monthName(qdate.month(), QLocale::LongFormat).toUtf8());
#else
    // %a: Abbreviated weekday name
    strReturnDT.Replace(wxT("%a"), QDate::shortDayName(qdate.dayOfWeek()).toUtf8());

    // %A: Full weekday name
    strReturnDT.Replace(wxT("%A"), QDate::longDayName(qdate.dayOfWeek()).toUtf8());

    // %b: Abbreviated month name
    strReturnDT.Replace(wxT("%b"), QDate::shortMonthName(qdate.month()).toUtf8());

    // %B: Full month name
    strReturnDT.Replace(wxT("%B"), QDate::longMonthName(qdate.month()).toUtf8());
#endif

    // %c: Date and time representation appropriate for locale
    // Not supported

    // %d: Day of month as decimal number (01 – 31)
    strReturnDT.Replace(wxT("%d"),wxString::Format(wxT("%02d"),qdate.day()).toUtf8());

    // %H: Hour in 24-hour format (00 – 23)
    strReturnDT.Replace(wxT("%H"),wxString::Format(wxT("%02d"),qtime.hour()).toUtf8());

    // %I: Hour in 12-hour format (01 – 12)
    strReturnDT.Replace(wxT("%I"),wxString::Format(wxT("%02d"),(qtime.hour() == 0) ? 12 : qtime.hour() % 12).toUtf8());

    // %j: Day of year as decimal number (001 – 366)
    strReturnDT.Replace(wxT("%j"),QString("%1").arg(qdate.dayOfYear()).toUtf8());

    // %m: Month as decimal number (01 – 12)
    strReturnDT.Replace(wxT("%m"),wxString::Format(wxT("%02d"),qdate.month()).toUtf8());

    // %M: Minute as decimal number (00 – 59)
    strReturnDT.Replace(wxT("%M"),wxString::Format(wxT("%02d"),qtime.minute()).toUtf8());

    // %p: Current locale's A.M./P.M. indicator for 12-hour clock
    strReturnDT.Replace(wxT("%p"),qtime.hour() > 11 ? "AM" : "PM");

    // %S: Second as decimal number (00 – 59)
    strReturnDT.Replace(wxT("%S"),wxString::Format(wxT("%02d"),qtime.second()).toUtf8());

    // %U: Week of year as decimal number, with Sunday as first day of week (00 – 53)
    // Not supported

    // %w: Weekday as decimal number (0 – 6; Sunday is 0)
    strReturnDT.Replace(wxT("%w"),QString("%1").arg(qdate.dayOfWeek() == 7 ? 0 : qdate.dayOfWeek()).toUtf8());

    // %W: Week of year as decimal number, with Monday as first day of week (00 – 53)
    int iYear = qdate.year();
    strReturnDT.Replace(wxT("%W"),wxString::Format(wxT("%02d"),qdate.weekNumber(&iYear) - 1).toUtf8());

    // %x: Date representation for current locale
    strReturnDT.Replace(wxT("%x"),QString("%1").arg(date().toString(Qt::ISODate)).toUtf8());

    // %X: Time representation for current locale
    strReturnDT.Replace(wxT("%x"),QString("%1").arg(time().toString(Qt::ISODate)).toUtf8());

    // %y : Year without century, as decimal number (00 – 99)
    strReturnDT.Replace(wxT("%y"),wxString::Format(wxT("%02d"),qdate.year() % 100).toUtf8());

    // %Y: Year with century, as decimal number
    strReturnDT.Replace(wxT("%Y"),wxString::Format(wxT("%04d"),qdate.year()).toUtf8());

    // %z, %Z: Either the time-zone name or time zone abbreviation, depending on registry settings; no characters if time zone is unknown
    // Not supported

    // Replace all %% by a tag that does not exits
    strReturnDT.Replace(wxT("¤$£%~ù"),wxT("%%"));

    return strReturnDT;
}

wxQDateTime wxQDateTime::MakeUTC() const
{
    return toUTC();
}

const wxChar * wxQDateTime::ParseDate(const QString &_rString)
{
    QDate dtComputed = QDate::fromString(_rString,Qt::ISODate);
    if (!dtComputed.isValid())
    {
        return nullptr;
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
        return nullptr;
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
        return nullptr;
    }
    else
    {
        setTime(dtComputed.time());
        setDate(dtComputed.date());
    }
    m_strParsedDate = toString(Qt::ISODate);
    return m_strParsedDate.c_str();
}

// get the internal representation
wxLongLong wxQDateTime::GetValue() const
{
    return QDateTime::toMSecsSinceEpoch();
}

wxLongLong wxQDateTime::GetJulianDayNumber() const
{
    return (qint64) ((QDateTime::toMSecsSinceEpoch() - EPOCH_JDN) / MILLISECONDS_PER_DAY + EPOCH_JDN + 0.5);
}

time_t wxQDateTime::GetTicks() const
{
    return static_cast<time_t>(toSecsSinceEpoch());
}







