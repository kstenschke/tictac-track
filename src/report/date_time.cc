
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <vector>
#include "date_time.h"
#include "../app/config.h"
#include "../helper/date_time.h"
#include "../helper/helper_string.h"
#include "../helper/numeric.h"

namespace timesheetplus {

/**
   * Constructor
   */
  ReportDateTime::ReportDateTime() {
    AppConfig config = AppConfig::GetInstance();

    format_week_of_year_ = config.GetConfigValue("format_week_of_year");
    format_date_         = config.GetConfigValue("format_date");
    format_day_of_week_  = config.GetConfigValue("format_day_of_week");

    locale_key_ = config.GetConfigValue("locale");
  }

  std::string ReportDateTime::GetDateFormat() {
    return format_date_;
  }

  /**
   * Translate e.g. "Monday" to "Montag", using configured locale language
   */
  std::string ReportDateTime::TranslateWeekday(const char* weekday_name_en) {
    int weekday_index = HelperDateTime::GetWeekdayIndexByName(weekday_name_en);
    if (weekday_index > 7) return weekday_name_en;

    const char* locale = locale_key_.c_str();

    if (0 == std::strcmp("de", locale)) return weekDayLabelsDe[weekday_index];
    if (0 == std::strcmp("dk", locale)) return weekDayLabelsDk[weekday_index];
    if (0 == std::strcmp("es", locale)) return weekDayLabelsEs[weekday_index];
    if (0 == std::strcmp("fi", locale)) return weekDayLabelsFi[weekday_index];
    if (0 == std::strcmp("fr", locale)) return weekDayLabelsFr[weekday_index];
    if (0 == std::strcmp("hu", locale)) return weekDayLabelsHu[weekday_index];
    if (0 == std::strcmp("it", locale)) return weekDayLabelsIt[weekday_index];
    if (0 == std::strcmp("lt", locale)) return weekDayLabelsLt[weekday_index];
    if (0 == std::strcmp("lv", locale)) return weekDayLabelsLv[weekday_index];
    if (0 == std::strcmp("nl", locale)) return weekDayLabelsNl[weekday_index];
    if (0 == std::strcmp("no", locale)) return weekDayLabelsNo[weekday_index];
    if (0 == std::strcmp("pl", locale)) return weekDayLabelsPl[weekday_index];
    if (0 == std::strcmp("pt", locale)) return weekDayLabelsPt[weekday_index];
    if (0 == std::strcmp("ro", locale)) return weekDayLabelsRo[weekday_index];
    if (0 == std::strcmp("sv", locale)) return weekDayLabelsSv[weekday_index];
    if (0 == std::strcmp("tr", locale)) return weekDayLabelsTr[weekday_index];

    return weekDayLabelsEn[weekday_index];
  }

  std::string ReportDateTime::GetCurrentWeekOfYear(int offset_weeks) {
    std::string week = HelperDateTime::GetCurrentTimeFormatted(format_week_of_year_.c_str());

    if (0 != offset_weeks) {
      int week_number = HelperString::ToInt(week.c_str());
      week_number += offset_weeks;

      week = HelperNumeric::ToString(week_number);
      if (1 == week.size()) week = "0" + week;
    }

    return week;
  }

  std::string ReportDateTime::GetCurrentDate(int offset_days) {
    return HelperDateTime::GetCurrentTimeFormatted(format_date_.c_str(), offset_days);
  }

  std::string ReportDateTime::GetTimestampForMeta() {
    return HelperDateTime::GetCurrentTimeFormatted("%Y/%m/%W/%d");
  }

  /**
   * Get day of week as string, localized into configured language
   */
  std::string ReportDateTime::GetCurrentDayOfWeek() {
    std::string weekdayNameEnglish = HelperDateTime::GetCurrentTimeFormatted(format_day_of_week_.c_str());

    return TranslateWeekday(weekdayNameEnglish.c_str());
  }

  /**
   * Get localize name of day of week, from given meta data
   */
  std::string ReportDateTime::GetWeekdayByMeta(std::string meta) {
    std::vector<std::string> meta_parts = HelperString::Explode(meta, '/');
    int day_of_week  = HelperDateTime::GetWeekdayIndexByDate(
      HelperString::ToInt(meta_parts[1]),  // year
      HelperString::ToInt(meta_parts[2]),  // month
      HelperString::ToInt(meta_parts[4])); // day

    return TranslateWeekday(HelperDateTime::GetWeekdayEnByIndex(day_of_week).c_str());
  }

  std::string ReportDateTime::GetCurrentTime() {
    return HelperDateTime::GetCurrentTimeFormatted(FORMAT_TIME);
  }

  std::string ReportDateTime::GetDurationFormatted(std::string time_started, std::string time_stopped) {
    int minutes_start = HelperDateTime::GetSumMinutesFromTime(std::move(time_started), ":");
    int minutes_end   = HelperDateTime::GetSumMinutesFromTime(std::move(time_stopped), ":");

    int minutes_duration = minutes_end - minutes_start;
    if (minutes_duration < 0) {
      // Entry timespan is crossing midnight (1 midnight is supported supported)
      minutes_duration = HelperDateTime::kMinutesPerDay - minutes_start + minutes_end;
    }

    return HelperDateTime::GetHoursFormattedFromMinutes(minutes_duration);
  }

  /**
   * Check whether date1 is after date2
   */
  bool ReportDateTime::IsMetaDateAfter(std::string meta_date1, std::string meta_date2) {
    std::vector<std::string> meta_parts1 = HelperString::Explode(meta_date1, '/');
    std::vector<std::string> meta_parts2 = HelperString::Explode(meta_date2, '/');

    int year_date1 = HelperString::ToInt(meta_parts1[1]);
    int year_date2 = HelperString::ToInt(meta_parts2[1]);
    if (year_date1 > year_date2) return true;
    if (year_date1 < year_date2) return false;

    // Years are same
    int month_date1 = HelperString::ToInt(meta_parts1[2]);
    int month_date2 = HelperString::ToInt(meta_parts2[2]);
    if (month_date1 > month_date2) return true;
    if (month_date1 < month_date2) return false;

    // Months are same
    int day_date1 = HelperString::ToInt(meta_parts1[4]);
    int day_date2 = HelperString::ToInt(meta_parts2[4]);

    return day_date1 > day_date2;
  }
} // namespace timesheetplus
