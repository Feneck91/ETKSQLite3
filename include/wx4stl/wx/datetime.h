/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/datetime.h
 * @brief Header file for wxDateTime class.
 *
 * Inspired by wxWidgets, modifyed to be used with QT library.
 *
 * @author Stéphane Château
 * @date Created: 2012/06/14
 * @date Modified: 2025/09/01
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_DATETIME_HEADER
#define WX_DATETIME_HEADER

#include <wx/wxtype.h>
#include <wx/longlong.h>
#include <chrono>
#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#define wxInvalidDateTime   wxDateTime()

#define MILLISECONDS_PER_DAY 86400000l
// this is the integral part of JDN of the midnight of Jan 1, 1970
// (i.e. JDN(Jan 1, 1970) = 2440587.5)
#define EPOCH_JDN           2440587l
#define InvalidTimePoint    TimePoint()

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                                wxDate                                                 //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class wxDateTime;
class EXPORT_IMPORT wxDate
{
public:
    wxDate(int day = 1, int month = 1, int year = 1900);

    int GetDay() const;
    int GetMonth() const;
    int GetYear() const;

    void operator=(const wxDateTime &_rDateTime);

private:
    int m_day;
    int m_month;
    int m_year;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                                wxTime                                                 //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class EXPORT_IMPORT wxTime
{
public:
    wxTime(int hour = 0, int minute = 0, int second = 0);

    int GetHour() const;
    int GetMinute() const;
    int GetSecond() const;

    void operator=(const wxDateTime &_rDateTime);

private:
    int m_hour;
    int m_minute;
    int m_second;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                              wxDateTime                                               //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class wxDateTime
{
public:
    // Types for duration and time points
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;
    using Duration = Clock::duration;

    // Enum for week days
    enum wxDateTimeWeekDay
    {
        wxDateTime_Sun = 0,
        wxDateTime_Mon,
        wxDateTime_Tue,
        wxDateTime_Wed,
        wxDateTime_Thu,
        wxDateTime_Fri,
        wxDateTime_Sat,
        wxDateTime_Inv_WeekDay
    };

    // Enum for months
    enum wxDateTimeMonth {
        wxDateTime_Jan = 0,
        wxDateTime_Feb,
        wxDateTime_Mar,
        wxDateTime_Apr,
        wxDateTime_May,
        wxDateTime_Jun,
        wxDateTime_Jul,
        wxDateTime_Aug,
        wxDateTime_Sep,
        wxDateTime_Oct,
        wxDateTime_Nov,
        wxDateTime_Dec,
        wxDateTime_Inv_Month
    };

protected:
    // Constructor from time_point
    EXPORT_IMPORT wxDateTime(const TimePoint& timePoint);

public:
    // Constructors
    EXPORT_IMPORT wxDateTime();
    EXPORT_IMPORT wxDateTime(time_t timeT);
    EXPORT_IMPORT wxDateTime(int year, wxDateTimeMonth month, int day, int hour = 0, int minute = 0, int second = 0);
    EXPORT_IMPORT wxDateTime(const struct tm &_rtmValue);
    EXPORT_IMPORT wxDateTime(const wxDate &date);
    EXPORT_IMPORT wxDateTime(const wxTime &date);
    EXPORT_IMPORT wxDateTime(double _dValue); // Constructs the object from timet value holding the number of seconds since Jan 1, 1970 UTC. (wxWidgets documentation)

    // Getters
    EXPORT_IMPORT time_t GetTicks() const;
    EXPORT_IMPORT int GetYear() const;
    EXPORT_IMPORT wxDateTimeMonth GetMonth() const;
    EXPORT_IMPORT int GetDay() const;
    EXPORT_IMPORT wxDateTimeWeekDay GetWeekDay() const;
    EXPORT_IMPORT int GetHour() const;
    EXPORT_IMPORT int GetMinute() const;
    EXPORT_IMPORT int GetSecond() const;
    EXPORT_IMPORT void SetMillisecond(int) {} // Not needed

    // Check
    EXPORT_IMPORT bool IsValid() const;
    // Get current date/time
    EXPORT_IMPORT static wxDateTime Now();

    // Get actual date / time in UTC
    EXPORT_IMPORT static wxDateTime UTCNow();

    // Get internal date en UTC
    EXPORT_IMPORT wxDateTime toUTC() const;
    EXPORT_IMPORT wxDateTime MakeUTC() const;
    EXPORT_IMPORT wxDateTime ToLocalTime() const; // To local time (not UTC)

    // Format date
    EXPORT_IMPORT wxString Format(const std::string& format = "%Y-%m-%d %H:%M:%S") const;
    EXPORT_IMPORT wxString toString(const std::string& format, bool _bUTC = false) const;

    // Parse string to create a wxDateTime
    EXPORT_IMPORT static wxDateTime ParseFormat(const std::string& dateTimeStr, const std::string& format = "%Y-%m-%d %H:%M:%S");
    EXPORT_IMPORT const wxChar * ParseDate(const std::string &dateStr);
    EXPORT_IMPORT const wxChar * ParseTime(const std::string &timeStr);
    EXPORT_IMPORT const wxChar * ParseDateTime(const std::string &dateTimeStr);

    // This function returns the date representation in the ISO 8601 format (YYYY-MM-DD).
    EXPORT_IMPORT wxString FormatISODate() const;
    // This function returns the time representation in the ISO 8601 format (HH:MM:SS).
    EXPORT_IMPORT wxString FormatISOTime() const;

    // Add / Substract / Diff
    EXPORT_IMPORT wxDateTime Add(const Duration& duration) const;
    EXPORT_IMPORT wxDateTime Subtract(const Duration& duration) const;
    EXPORT_IMPORT Duration Subtract(const wxDateTime& other) const;

    // Operators
    EXPORT_IMPORT bool operator==(const wxDateTime& other) const;
    EXPORT_IMPORT bool operator!=(const wxDateTime& other) const;
    EXPORT_IMPORT bool operator<(const wxDateTime& other) const;
    EXPORT_IMPORT bool operator<=(const wxDateTime& other) const;
    EXPORT_IMPORT bool operator>(const wxDateTime& other) const;
    EXPORT_IMPORT bool operator>=(const wxDateTime& other) const;
    // Check if bissextil year
    EXPORT_IMPORT static bool IsLeapYear(int year);

    // Function not implemented by base class
    EXPORT_IMPORT wxDate date() const;
    EXPORT_IMPORT wxTime time() const;

    // get the internal representation
    EXPORT_IMPORT wxLongLong GetValue() const;
    EXPORT_IMPORT wxLongLong GetJulianDayNumber() const;

private:
    TimePoint m_timePoint; // Internal stockage in UTC
};

// Constantes pour les durées
namespace wxTimeSpan
{
    const wxDateTime::Duration Seconds(int seconds);
    const wxDateTime::Duration Minutes(int minutes);
    const wxDateTime::Duration Hours(int hours);
    const wxDateTime::Duration Days(int days);
    const wxDateTime::Duration Weeks(int weeks);
}

#endif // WX_DATETIME_HEADER
