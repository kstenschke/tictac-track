
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_REPORT
#define CLASS_TIMETRACKERCL_REPORT

#include "date_time.h"

namespace tictac_track {
class Report {
 public:
  enum RenderScopes {
    Scope_All,
    Scope_Day,
    Scope_Month,
    Scope_Week,
    Scope_Invalid
  };

  enum ColumnIndexes {
    Index_Meta = 0,
    Index_Week = 1,
    Index_Day = 2,
    Index_Date = 3,
    Index_Start = 4,
    Index_End = 5,
    Index_Task = 6,
    Index_Comment = 7,
    Index_Duration = 8,
    Index_SumTaskDay = 9,
    Index_SumDay = 10,
    Index_Balance = 11
  };

 protected:
  // Render scope (all or filtered by current: day / week / month)
  int render_scope_;
  std::string rows_filter_ = "";

  ReportDateTime report_date_time_instance_;

  void InitScopeFilter(int scope, int offset);
  void InitScopeFilter(int offset = 0);

 private:
  // Get filter value: string that row must contain for being display w/ active day- / week- / month- filter
  std::string GetFilterValueForDayScope(int offset = 0);
  std::string GetFilterValueForWeekScope(int offset = 0);
  std::string GetFilterValueForMonthScope(int offset = 0);
};
} // namespace tictac_track

#endif
