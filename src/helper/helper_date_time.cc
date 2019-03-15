/*
  Copyright (c) 2018, Kay Stenschke
  All rights reserved.
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
   * Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   * Neither the name of  nor the names of its contributors may be used to
     endorse or promote products derived from this software without specific
     prior written permission.
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

#include <string>
#include <ctime>
#include <cstring>
#include "helper_date_time.h"
#include "helper_string.h"
#include "helper_numeric.h"

namespace helper {

  /**
   * Get string of current timestamp, w/ given offset in days added, in given format.
   */
  std::string DateTime::GetCurrentTimeFormatted(const char* format, int offset_days) {
    time_t raw_time;
    struct tm * time_info;
    char buffer[80];

    time(&raw_time);
    time_info = localtime_r(&raw_time);

    if (0 != offset_days) AddDaysToDate(time_info, offset_days);

    strftime(buffer,sizeof(buffer), format, time_info);
    std::string str(buffer);

    return str;
  }

  /**
   * Adjust date by given number of days
   */
  void DateTime::AddDaysToDate(struct tm* date, int days) {
    const time_t ONE_DAY = 24 * 60 * 60;

    // Seconds since start of epoch
    time_t date_seconds = mktime( date ) + (days * ONE_DAY) ;

    // Update caller's date
    // Use localtime because mktime converts to UTC so may change date
    *date = *localtime(&date_seconds);
  }

  std::string DateTime::ReformatDateAsYyyyMmDd(std::string date_str, std::string source_format) {
    size_t strlen = date_str.size();
    if (0 == strlen || std::string::npos == strlen) return "";

    if (source_format == "%d.%m.%Y") {
      std::string day   = date_str.substr(0, 2);
      std::string month = date_str.substr(3, 2);
      std::string year  = date_str.substr(6, std::string::npos);
      date_str = year + month + day;
    }
    // TODO make this generic to support any arbitrary date format

    date_str = String::ReplaceAll(date_str, ".", "");
    date_str = String::ReplaceAll(date_str, "-", "");
    date_str = String::ReplaceAll(date_str, "/", "");
    date_str = String::ReplaceAll(date_str, " ", "");

    return date_str;
  }

  /**
   * Calculate minutes since 0:00 from given formatted time label
   */
  int DateTime::GetSumMinutesFromTime(std::string time_str, const char* separator) {
    if (time_str.empty()) time_str = GetCurrentTimeFormatted(FORMAT_TIME);

    size_t offset_separator = time_str.find(separator);
    if (std::string::npos == offset_separator) return 0;

    int hours   = String::ToInt( time_str.substr(0, offset_separator).c_str() );
    int minutes = String::ToInt( time_str.substr(offset_separator + 1, std::string::npos ).c_str() );

    return hours * 60 + minutes;
  }

  /**
   * Format given amount of minutes like "hh:mm". Hours >= 24 are treated as being in next day (begin from 0 again)
   */
  std::string DateTime::GetHoursFormattedFromMinutes(int minutes) {
    bool is_negative = minutes < 0;
    if (is_negative) minutes *= -1;

    int hours   = 0;
    while(minutes > 59) {
      hours++;
      minutes -= 60;
      if (hours > 23) hours = 0;
    }

    return (is_negative ? "-" : "") + Numeric::ToString(hours, 2) + ":" + Numeric::ToString(minutes, 2);
  }

  /**
   * Get index of day of week: sunday == 0, monday == 1, etc.
   */
  int DateTime::GetWeekdayIndexByDate(int year, int month, int day) {
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
  int DateTime::GetWeekdayIndexByName(const char* weekday_name_en) {
    if (0 == std::strcmp(weekday_name_en, "Monday"))    return 0;
    if (0 == std::strcmp(weekday_name_en, "Tuesday"))   return 1;
    if (0 == std::strcmp(weekday_name_en, "Wednesday")) return 2;
    if (0 == std::strcmp(weekday_name_en, "Thursday"))  return 3;
    if (0 == std::strcmp(weekday_name_en, "Friday"))    return 4;
    if (0 == std::strcmp(weekday_name_en, "Saturday"))  return 5;
    if (0 == std::strcmp(weekday_name_en, "Sunday"))    return 6;

    return 8;
  }

  std::string DateTime::GetWeekdayEnByIndex(int day_index) {
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

  bool DateTime::IsTime(std::string str) {
    unsigned long offsetColon = str.find(':');
    if (std::string::npos == offsetColon) return false;
    str = str.replace(offsetColon, 1, "");

    return String::IsNumeric(str);
  }
} // namespace helper
