
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <ctime>
#include "date_time.h"
#include "helper_string.h"
#include "numeric.h"
#include "../report/date_time.h"

namespace timesheetplus {

  /**
   * Get string of current timestamp, w/ given offset in days added, in given format.
   */
  std::string HelperDateTime::GetCurrentTimeFormatted(const char* format, int offset_days) {
    time_t raw_time;
    struct tm * time_info;
    char buffer[80];

    time(&raw_time);
    time_info = localtime(&raw_time);

    if (0 != offset_days) AddDaysToDate(time_info, offset_days);

    strftime(buffer,sizeof(buffer), format, time_info);
    std::string str(buffer);

    return str;
  }

  /**
   * Adjust date by given number of days
   */
  void HelperDateTime::AddDaysToDate(struct tm* date, int days) {
    const time_t ONE_DAY = 24 * 60 * 60;

    // Seconds since start of epoch
    time_t date_seconds = mktime( date ) + (days * ONE_DAY) ;

    // Update caller's date
    // Use localtime because mktime converts to UTC so may change date
    *date = *localtime(&date_seconds);
  }

  std::string HelperDateTime::ReformatDateAsYyyyMmDd(std::string date_str, std::string source_format) {
    size_t strlen = date_str.size();
    if (0 == strlen || std::string::npos == strlen) return "";

    if (source_format == "%d.%m.%Y") {
      std::string day   = date_str.substr(0, 2);
      std::string month = date_str.substr(3, 2);
      std::string year  = date_str.substr(6, std::string::npos);
      date_str = year + month + day;
    }
    // TODO make this generic to support any arbitrary date format

    date_str = HelperString::ReplaceAll(date_str, ".", "");
    date_str = HelperString::ReplaceAll(date_str, "-", "");
    date_str = HelperString::ReplaceAll(date_str, "/", "");
    date_str = HelperString::ReplaceAll(date_str, " ", "");

    return date_str;
  }

  /**
   * Calculate minutes since 0:00 from given formatted time label
   */
  int HelperDateTime::GetSumMinutesFromTime(std::string time_str, const char* separator) {
    if (time_str.empty()) time_str = GetCurrentTimeFormatted(ReportDateTime::FORMAT_TIME);

    size_t offset_separator = time_str.find(separator);
    if (std::string::npos == offset_separator) return 0;

    int hours   = HelperString::ToInt( time_str.substr(0, offset_separator).c_str() );
    int minutes = HelperString::ToInt( time_str.substr(offset_separator + 1, std::string::npos ).c_str() );

    return hours * 60 + minutes;
  }

  /**
   * Format given amount of minutes like "hh:mm". Hours >= 24 are treated as being in next day (begin from 0 again)
   */
  std::string HelperDateTime::GetHoursFormattedFromMinutes(int minutes) {
    bool is_negative = minutes < 0;
    if (is_negative) minutes *= -1;

    int hours   = 0;
    while(minutes > 60) {
      hours++;
      minutes -= 60;
      if (hours > 23) hours = 0;
    }

    return (is_negative ? "-" : "") + HelperNumeric::ToString(hours, 2) + ":" + HelperNumeric::ToString(minutes, 2);
  }

  /**
   * Get index of day of week: sunday == 0, monday == 1, etc.
   */
  int HelperDateTime::GetWeekdayIndexByDate(int year, int month, int day) {
    std::tm time_in = { 0, 0, 0,                       // second, minute, hour,
                        day, month - 1, year - 1900 }; // 1-based day, 0-based month, year since 1900
    std::time_t time_temp = std::mktime(&time_in);

    // Note: Return value of localtime is not thread-safe, because it might be
    // (and will be) reused in subsequent calls to std::localtime!
    const std::tm * time_out = std::localtime(&time_temp);

    return time_out->tm_wday;
  }

  /**
   * Get index of day of week from english name of weekday: sunday == 0, monday == 1, etc.
   */
  int HelperDateTime::GetWeekdayIndexByName(const char* weekday_name_en) {
    if (0 == strcmp(weekday_name_en, "Monday"))    return 0;
    if (0 == strcmp(weekday_name_en, "Tuesday"))   return 1;
    if (0 == strcmp(weekday_name_en, "Wednesday")) return 2;
    if (0 == strcmp(weekday_name_en, "Thursday"))  return 3;
    if (0 == strcmp(weekday_name_en, "Friday"))    return 4;
    if (0 == strcmp(weekday_name_en, "Saturday"))  return 5;
    if (0 == strcmp(weekday_name_en, "Sunday"))    return 6;

    return 8;
  }

  std::string HelperDateTime::GetWeekdayEnByIndex(int day_index) {
    switch (day_index) {
      case 0: return "Sunday";
      case 1: return "Monday";
      case 2: return "Tuesday";
      case 3: return "Wednesday";
      case 4: return "Thursday";
      case 5: return "Friday";
      case 6: return "Saturday";
      case 7: return "Sunday";
      default: return "";
    }
  }
} // namespace timesheetplus
