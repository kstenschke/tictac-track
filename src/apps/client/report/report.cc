
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include "report.h"

namespace tictac_track {

/**
 * Init filter value for time scope (all rows or filtered by: day / week / month)
 */
void Report::InitScopeFilter(int scope, int offset) {
  render_scope_ = scope;
  InitScopeFilter(offset);
}

void Report::InitScopeFilter(int offset) {
  switch (render_scope_) {
    case Scope_Day:rows_filter_ = GetFilterValueForDayScope(offset);
      break;
    case Scope_Week:rows_filter_ = GetFilterValueForWeekScope(offset);
      break;
    case Scope_Month:rows_filter_ = GetFilterValueForMonthScope(offset);
      break;
    case Scope_Invalid:
    default:
//        std::cout << "Invalid scope";
      break;
  }
}

std::string Report::GetFilterValueForDayScope(int offset) {
  return report_date_time_instance_.GetCurrentDate(offset);
}

std::string Report::GetFilterValueForWeekScope(int offset) {
  return report_date_time_instance_.GetCurrentWeekOfYear(offset);
}

std::string Report::GetFilterValueForMonthScope(int offset) {
  return report_date_time_instance_.GetCurrentDate(offset);
}
} // namespace tictac_track
