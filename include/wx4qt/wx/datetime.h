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
#include <QDateTime>

#define wxDateTime          wxQDateTime
#define wxInvalidDateTime   QDateTime()
#define wxDate              QDate
#define wxTime              QTime

#define MILLISECONDS_PER_DAY 86400000l
// this is the integral part of JDN of the midnight of Jan 1, 1970
// (i.e. JDN(Jan 1, 1970) = 2440587.5)
#define EPOCH_JDN           2440587l

class EXPORT_IMPORT wxQDateTime : public QDateTime
{
private:
    mutable wxString m_strParsedDate;
public :
    wxQDateTime();
    wxQDateTime(const wxQDateTime &_rDT);
    wxQDateTime(const QDateTime &_rDT);
    wxQDateTime(wxLongLong &_dt);
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    wxQDateTime(double _dValue);
    wxQDateTime(const QDate & _rDate);
#else
    wxQDateTime(double _dValue);
    wxQDateTime(const QDate & _rDate);
#endif
    wxQDateTime(const struct tm &date);
    wxQDateTime(const QTime & _rTime);
    operator QDate () const;
    operator QTime () const;

    void SetMillisecond(int _iMillisecond);
    bool IsValid() const;
    // This function returns the date representation in the ISO 8601 format (YYYY-MM-DD).
    wxString FormatISODate() const;
    // This function returns the time representation in the ISO 8601 format (HH:MM:SS).
    wxString FormatISOTime() const;
    wxString Format(const char *_pszFormat) const;
    wxQDateTime MakeUTC() const;

    const wxChar * ParseDate(const QString &_rString);
    const wxChar * ParseTime(const QString &_rString);
    const wxChar * ParseDateTime(const QString &_rString);

    // get the internal representation
    wxLongLong GetValue() const;
    wxLongLong GetJulianDayNumber() const;

    time_t GetTicks() const;
};

#endif // WX_DATETIME_HEADER
