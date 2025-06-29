/////////////////////////////////////////////////////////////////////////////
// Name:        wx/datetime.h
// Purpose:     wxDateTime class
// Library:     Copied from wxWidgets, modifyed to be used with QT library
// Author:      Stéphane Château
// Modified by:
// Created:     14/06/2012
// Copyright:   (c) Stéphane Château
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_DATETIME_HEADER
#define WX_DATETIME_HEADER

#include "longlong.h"
#include "string.h"
#include <QDateTime>

#define wxDateTime          wxQDateTime
#define wxInvalidDateTime   QDateTime(QDate(-1,-1,-1))

#define MILLISECONDS_PER_DAY 86400000l
// this is the integral part of JDN of the midnight of Jan 1, 1970
// (i.e. JDN(Jan 1, 1970) = 2440587.5)
#define EPOCH_JDN           2440587l

class EXPORT_IMPORT wxQDateTime : public QDateTime
{
private:
    mutable wxString m_strParsedDate;
public :
    wxQDateTime() {}
    wxQDateTime(const wxQDateTime &_rDT) : QDateTime(_rDT)                              {}
    wxQDateTime(const QDateTime &_rDT) : QDateTime(_rDT) {}
    wxQDateTime(wxLongLong &_dt) : QDateTime(QDateTime::fromMSecsSinceEpoch(_dt.GetValue())) {}
    wxQDateTime(double _dValue) :  QDateTime(QDate::fromJulianDay(_dValue))             {}
    wxQDateTime(const struct tm &date): QDateTime(QDateTime(QDate(date.tm_year, date.tm_mon, date.tm_mday), QTime(date.tm_hour, date.tm_min,date.tm_sec, 0),Qt::LocalTime)) {}
    wxQDateTime(const QDate & _rDate): QDateTime(_rDate)                                {}
    wxQDateTime(const QTime & _rTime): QDateTime(QDate(2000,10,10),_rTime)              {} // <-- Set Date to dummy to have a valid date / time!
    operator QDate () const;
    operator QTime () const;

    void SetMillisecond(int _iMillisecond)                                              { setTime(QTime(time().hour(),time().minute(),time().second(),_iMillisecond)); }
    bool IsValid() const                                                                { return isValid(); }
    // This function returns the date representation in the ISO 8601 format (YYYY-MM-DD).
    wxString FormatISODate() const;
    // This function returns the time representation in the ISO 8601 format (HH:MM:SS).
    wxString FormatISOTime() const;
    wxString Format(const char *_pszFormat) const;
    wxQDateTime MakeUTC() const                                                         { return toUTC(); }

    const wxChar * ParseDate(const QString &_rString);
    const wxChar * ParseTime(const QString &_rString);
    const wxChar * ParseDateTime(const QString &_rString);

    // get the internal representation
    inline wxLongLong GetValue() const                                                  { return QDateTime::toMSecsSinceEpoch(); }
    inline wxLongLong GetJulianDayNumber() const                                        { return (qint64) ((QDateTime::toMSecsSinceEpoch() - EPOCH_JDN) / MILLISECONDS_PER_DAY + EPOCH_JDN + 0.5); }

    time_t GetTicks() const                                                             { return toTime_t(); }
};

#endif // WX_DATETIME_HEADER
