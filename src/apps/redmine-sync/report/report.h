
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_REPORT
#define CLASS_TIMETRACKERCL_REPORT

namespace tictac_rms {
class Report {
 public:
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
};
} // namespace tictac_rms

#endif
