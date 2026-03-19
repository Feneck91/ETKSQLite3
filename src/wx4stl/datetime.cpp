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

static const wxChar * ISODate       = "%Y-%m-%d";
static const wxChar * ISOTime       = "%H:%M:%S";
static const wxChar * ISODateTime   = "%Y-%m-%dT%H:%M:%S";

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                                wxDate                                                 //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
wxDate::wxDate(int day, int month, int year)
    : m_day(day), m_month(month), m_year(year)
{
}

int wxDate::GetDay() const
{
    return m_day;
}

int wxDate::GetMonth() const
{
    return m_month;
}

int wxDate::GetYear() const
{
    return m_year;
}

void wxDate::operator=(const wxDateTime &_rDateTime)
{
    m_day   = _rDateTime.GetDay();
    m_month = _rDateTime.GetMonth();
    m_year  = _rDateTime.GetYear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                                wxTime                                                 //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
wxTime::wxTime(int hour, int minute, int second)
    : m_hour(hour), m_minute(minute), m_second(second)
{
}

int wxTime::GetHour() const
{
    return m_hour;
}

int wxTime::GetMinute() const
{
    return m_minute;
}

int wxTime::GetSecond() const
{
    return m_second;
}

void wxTime::operator=(const wxDateTime &_rDateTime)
{
    m_hour   = _rDateTime.GetHour();
    m_minute = _rDateTime.GetMinute();
    m_second = _rDateTime.GetSecond();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                              wxDateTime                                               //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
wxDateTime::wxDateTime(const TimePoint& timePoint)
    : m_timePoint(timePoint)
{
}

wxDateTime::wxDateTime()
    : m_timePoint(Clock::now())
{
}

wxDateTime::wxDateTime(time_t timeT)
    : m_timePoint(Clock::from_time_t(timeT))
{
}

wxDateTime::wxDateTime(int year, wxDateTimeMonth month, int day, int hour, int minute, int second)
{
    std::tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = static_cast<int>(month);
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    tm.tm_isdst = -1; // Laisse mktime déterminer l'heure d'été

    std::time_t timeT = std::mktime(&tm);
    m_timePoint = (timeT == -1)
        ? InvalidTimePoint  // Invalid
        : Clock::from_time_t(timeT);
}

wxDateTime::wxDateTime(const struct tm &_rtmValue)
{
    std::tm tmCopy = _rtmValue; // Copie de la structure tm pour éviter les modifications
    std::time_t timeT = std::mktime(&tmCopy);
    m_timePoint = (timeT == -1)
        ? InvalidTimePoint          // Invalid
        : Clock::from_time_t(timeT);
}

// Constructor from wxDate (hour by default to midnight)
wxDateTime::wxDateTime(const wxDate& date)
{
    struct tm tm = {};
    tm.tm_year = date.GetYear() - 1900;
    tm.tm_mon = date.GetMonth() - 1;
    tm.tm_mday = date.GetDay();
    tm.tm_hour = 0;   // Default hour is 0h00
    tm.tm_min = 0;
    tm.tm_sec = 0;
    tm.tm_isdst = -1; // Let mktime choose summer time

    std::time_t timeT = std::mktime(&tm);
    m_timePoint = (timeT == -1)
        ? InvalidTimePoint  // Invalid
        : Clock::from_time_t(timeT);
}

// Constructor from wxTime (date by défaut is 01/01/1970)
wxDateTime::wxDateTime(const wxTime& time)
{
    std::time_t now = Clock::to_time_t(Clock::now());
    std::tm tm = *std::localtime(&now);

    tm.tm_hour = time.GetHour();
    tm.tm_min = time.GetMinute();
    tm.tm_sec = time.GetSecond();
    tm.tm_year = 1970;
    tm.tm_mon = 0;
    tm.tm_mday = 1;
    tm.tm_isdst = -1; // Let mktime choose summer time

    std::time_t timeT = std::mktime(&tm);
    m_timePoint = (timeT == -1)
        ? InvalidTimePoint  // Invalid
        : Clock::from_time_t(timeT);
}

wxDateTime::wxDateTime(double _dValue)
{
    m_timePoint = Clock::time_point() + std::chrono::duration_cast<Clock::duration>(std::chrono::duration<double>(_dValue));
}

time_t wxDateTime::GetTicks() const
{
    return Clock::to_time_t(m_timePoint);
}

int wxDateTime::GetYear() const
{
    std::time_t timeT = Clock::to_time_t(m_timePoint);
    std::tm tm = *std::localtime(&timeT);
    return tm.tm_year + 1900;
}

wxDateTime::wxDateTimeMonth wxDateTime::GetMonth() const
{
    std::time_t timeT = Clock::to_time_t(m_timePoint);
    std::tm tm = *std::localtime(&timeT);
    return static_cast<wxDateTimeMonth>(tm.tm_mon);
}

int wxDateTime::GetDay() const
{
    std::time_t timeT = Clock::to_time_t(m_timePoint);
    std::tm tm = *std::localtime(&timeT);
    return tm.tm_mday;
}

// Récupère le jour de la semaine
wxDateTime::wxDateTimeWeekDay wxDateTime::GetWeekDay() const
{
    std::time_t timeT = Clock::to_time_t(m_timePoint);
    std::tm tm = *std::localtime(&timeT);
    return static_cast<wxDateTimeWeekDay>(tm.tm_wday);
}

int wxDateTime::GetHour() const
{
    std::time_t timeT = Clock::to_time_t(m_timePoint);
    std::tm tm = *std::localtime(&timeT);
    return tm.tm_hour;
}

int wxDateTime::GetMinute() const
{
    std::time_t timeT = Clock::to_time_t(m_timePoint);
    std::tm tm = *std::localtime(&timeT);
    return tm.tm_min;
}

int wxDateTime::GetSecond() const
{
    std::time_t timeT = Clock::to_time_t(m_timePoint);
    std::tm tm = *std::localtime(&timeT);
    return tm.tm_sec;
}

bool wxDateTime::IsValid() const
{
    return m_timePoint != TimePoint();
}

wxDateTime wxDateTime::Now()
{
    return wxDateTime(Clock::now());
}

wxDateTime wxDateTime::UTCNow()
{
    return wxDateTime(Clock::now());
}

// Get into UTC format
wxDateTime wxDateTime::toUTC() const
{
    return *this;
}

wxDateTime wxDateTime::MakeUTC() const
{
    return toUTC();
}

wxDateTime wxDateTime::ToLocalTime() const
{
    auto in_time_t = Clock::to_time_t(m_timePoint);

    // Convert into struct tm local
    std::tm tmLocal = *std::localtime(&in_time_t);

    // Back to time_point
    return wxDateTime(Clock::from_time_t(std::mktime(&tmLocal)));
}

// Format date / time into string
wxString wxDateTime::Format(const std::string& format) const
{
    std::time_t timeT = Clock::to_time_t(m_timePoint);
    std::tm tm = *std::localtime(&timeT);

    char buffer[256];
    std::strftime(buffer, sizeof(buffer), format.c_str(), &tm);
    return wxString(buffer);
}

// Format date / time into string
wxString wxDateTime::toString(const std::string& format, bool _bUTC) const
{
    auto in_time_t = Clock::to_time_t(m_timePoint);
    std::tm tm{};
    if (_bUTC)
    {
        tm = *std::gmtime(&in_time_t);
    }
    else
    {
        tm = *std::localtime(&in_time_t);
    }

    char buffer[256];
    return std::strftime(buffer, sizeof(buffer), format.c_str(), &tm)
        ? buffer
        : wxEmptyString;
}

// Parse string to create wxDateTime (portable version)
wxDateTime wxDateTime::ParseFormat(const std::string& dateTimeStr, const std::string& format)
{
    int year = 1900, month = 0, day = 1, hour = 0, minute = 0, second = 0;

    // Get simple approch to parse simple formats
    if (format == "%Y-%m-%d %H:%M:%S")
    {
        if (sscanf(dateTimeStr.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) != 6)
        {
            return InvalidTimePoint;
        }
        --month; // Months are 0-based
    }
    else if (format == "%d/%m/%Y %H:%M:%S")
    {
        if (sscanf(dateTimeStr.c_str(), "%d/%d/%d %d:%d:%d", &day, &month, &year, &hour, &minute, &second) != 6)
        {
            return InvalidTimePoint;
        }
        --month; // Months are 0-based
    }
    else if (format == "%Y-%m-%d")
    {
        if (sscanf(dateTimeStr.c_str(), "%d-%d-%d", &year, &month, &day) != 3)
        {
            return InvalidTimePoint;
        }
        --month; // Months are 0-based
    }
    else
    {
        return InvalidTimePoint;
    }

    std::tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = month;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    tm.tm_isdst = -1; // let mktime check if daily time

    std::time_t timeT = std::mktime(&tm);
    if (timeT == -1)
    {
        return InvalidTimePoint;
    }

    return wxDateTime(Clock::from_time_t(timeT));
}

// Parse string to create wxDateTime (portable version)
const wxChar * wxDateTime::ParseDate(const std::string& dateStr)
{
    wxDateTime dt = ParseFormat(dateStr, wxT("%Y-%m-%d"));
    return (dt.IsValid())
        ? toString(ISODate)
        : nullptr;
}

const wxChar * wxDateTime::ParseTime(const std::string& timeStr)
{
    wxDateTime dt = ParseFormat(timeStr, wxT("%H:%M:%S"));
    return (dt.IsValid())
        ? toString(ISOTime)
        : nullptr;
}

const wxChar * wxDateTime::ParseDateTime(const std::string& dateTimeStr)
{
    wxDateTime dt = ParseFormat(dateTimeStr, wxT("%H:%M:%S"));
    return (dt.IsValid())
        ? toString(ISODateTime)
        : nullptr;
}

// This function returns the date representation in the ISO 8601 format (YYYY-MM-DD).
wxString wxDateTime::FormatISODate() const
{
    return toString(ISODate);
}

// This function returns the time representation in the ISO 8601 format (HH:MM:SS).
wxString wxDateTime::FormatISOTime() const
{
    return toString(ISOTime);
}

wxDateTime wxDateTime::Add(const Duration& duration) const
{
    return wxDateTime(m_timePoint + duration);
}

wxDateTime wxDateTime::Subtract(const Duration& duration) const
{
    return wxDateTime(m_timePoint - duration);
}

wxDateTime::Duration wxDateTime::Subtract(const wxDateTime& other) const
{
    return m_timePoint - other.m_timePoint;
}

bool wxDateTime::operator==(const wxDateTime& other) const
{
    return m_timePoint == other.m_timePoint;
}

bool wxDateTime::operator!=(const wxDateTime& other) const
{
    return m_timePoint != other.m_timePoint;
}

bool wxDateTime::operator<(const wxDateTime& other) const
{
    return m_timePoint < other.m_timePoint;
}

bool wxDateTime::operator<=(const wxDateTime& other) const
{
    return m_timePoint <= other.m_timePoint;
}

bool wxDateTime::operator>(const wxDateTime& other) const
{
    return m_timePoint > other.m_timePoint;
}

bool wxDateTime::operator>=(const wxDateTime& other) const
{
    return m_timePoint >= other.m_timePoint;
}

// Vérifie si l'année est bissextile
bool wxDateTime::IsLeapYear(int year)
{
    return (year % 4 == 0) && (year % 100 != 0) && (year % 400 == 0);
}

// Constantes pour les durées
namespace wxTimeSpan
{
    const wxDateTime::Duration Seconds(int seconds)
    {
        return std::chrono::seconds(seconds);
    }

    const wxDateTime::Duration Minutes(int minutes)
    {
        return std::chrono::minutes(minutes);
    }

    const wxDateTime::Duration Hours(int hours)
    {
        return std::chrono::hours(hours);
    }

    const wxDateTime::Duration Days(int days)
    {
        return std::chrono::hours(24 * days);
    }

    const wxDateTime::Duration Weeks(int weeks)
    {
        return Days(7 * weeks);
    }
}

wxDate wxDateTime::date() const
{
    std::time_t timeT = Clock::to_time_t(m_timePoint);
    std::tm tm = *std::localtime(&timeT);

    return wxDate(tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

wxTime wxDateTime::time() const
{
    std::time_t timeT = Clock::to_time_t(m_timePoint);
    std::tm tm = *std::localtime(&timeT);

    return wxTime(tm.tm_hour, tm.tm_min, tm.tm_sec);
}

wxLongLong wxDateTime::GetJulianDayNumber() const
{

    return wxLongLong(static_cast<int64_t>((GetValue().GetValue() - EPOCH_JDN) / MILLISECONDS_PER_DAY + EPOCH_JDN + 0.5));
}

// get the internal representation
wxLongLong wxDateTime::GetValue() const
{
    return wxLongLong(std::chrono::duration_cast<std::chrono::milliseconds>(m_timePoint.time_since_epoch()).count());
}
