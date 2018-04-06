
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_REPORT_RECALCULATOR
#define CLASS_TIMETRACKERCL_REPORT_RECALCULATOR

#include <map>
#include "report_html_parser.h"

namespace tictac_track {
class ReportRecalculator : public ReportHtmlParser {
 public:
  // Constructor
  explicit ReportRecalculator(std::string html = "");

  // Recalculate and update title, column titles, durations, day of week, sum task/day, sum day, balance
  static bool RecalculateAndUpdate();
  bool Recalculate();

  void AddToTaskMaps(std::string task_number, int index_row, int duration_minutes);

  // Extract start- and end-time and calculate and update duration column in given row from it
  static void CalculateAndUpdateDuration(std::string &html, int row_index);
  // Load report HTML, than update duration
  static std::string CalculateAndUpdateDuration(int row_index);

 private:
  // Map storing per one day: which entry row where tasks listed lastly? this is where their sum is to be put
  std::map<std::string, int> task_in_day_last_index_;
  // Map storing per one day: sum of durations of that task within that day
  std::map<std::string, int> task_in_day_duration_sum_;

  // Update sums of task/day by current task maps
  void UpdateTaskSumsFromMaps(std::string &html);

  void ClearTaskMaps();
};
} // namespace tictac_lib

#endif
