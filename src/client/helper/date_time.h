
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_HELPER_DATE_TIME
#define CLASS_TIMETRACKERCL_HELPER_DATE_TIME

namespace tictac_track {
  namespace HelperDateTime {
    const int kMinutesPerDay = 1440;

    // Get string of current time, w/ given offset in days added, in given format
    extern std::string GetCurrentTimeFormatted(const char* format, int offset_days = 0);

    // Adjust date by given number of days
    extern void AddDaysToDate(struct tm* date, int days);

    extern std::string ReformatDateAsYyyyMmDd(std::string date_first, std::string source_format);

    // Calculate minutes since 0:00 from given formatted time label
    extern int GetSumMinutesFromTime(std::string time_str = "", const char* separator = ":");

    // Format given amount of minutes like "hh:mm". Hours >= 24 are treated as being in next day (begin from 0 again)
    extern std::string GetHoursFormattedFromMinutes(int minutes);

    // Get index of day of week: sunday == 0, monday == 1, etc.
    extern int GetWeekdayIndexByDate(int year, int month, int day);
    // Get index of day of week from english name of weekday: sunday == 0, monday == 1, etc.
    int GetWeekdayIndexByName(const char* weekday_name_en);
    std::string GetWeekdayEnByIndex(int day_index);

    extern bool IsTime(std::string str);
  }
} // namespace tictac_track

#endif
