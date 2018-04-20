
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_REPORT_RENDERER_CLI
#define CLASS_TIMETRACKERCL_REPORT_RENDERER_CLI

#include <string>
#include "report_renderer.h"

namespace tictac_track {

class ReportRendererCli : public ReportRenderer {

 public:
  // Constructor: init CLI ANSI color theme
  ReportRendererCli();

  // Pretty-print timesheet HTML (table) to CLI, using given filters
  bool PrintToCli(RenderScopes scope, int lookbehind_amount, int task_number, std::string comment = "");
  bool PrintBrowseDayTasks(int days_offset = 0);
  
 private:
  static const std::string kAnsiFormatReset;
  static const std::string kAnsiFormatBold;
  static const std::string kAnsiFormatInverted;
  static const std::string kAnsiFormatUnderline;

  std::string GetMessageHintClosestDayEntryBefore(int lookbehind_amount);

  std::string GetActiveScopeName();

 protected:
  // ANSI color/formatting codes for CLI output styling
  std::string theme_style_header_;
  std::string theme_style_default_;
  std::string theme_style_grid_;

  // Maximum mergeable amount of minutes between to entries (lunch- or other long break)
  int minutes_break_;

  // Amount of digits in maximum index
  unsigned long max_index_digits_;
  int amount_cells_;
  int offset_id_column_;

  // Output <thead>
  void PrintHeader();
  void PrintHeaderCellForId(bool is_left_most);
  // Output <tr>s, returns amount of rows printed
  int PrintRows(int task_number, std::string comment = "", bool sequential = false);

  void PrintRowCellForId(bool is_left_most, int index_row);
  // Fill cell w/ spaces to keep width of cells in column identical
  void PrintRhsCellSpaces(int index_cell, int index_column);

  void PrintColumn(int index_cell, bool is_even, int index_row, int index_column, bool emphasize = false);
  void PrintDurationSums(int task_number, int sum_task_minutes);

  // Render separator row (printed between days)
  std::string RenderSeparationRow();

  // Initialize color/formatting theme style codes
  void InitAnsiTheme();

  // Check end-time at given cell: is there an unmergeable gap (lunch- or other break) between the start-time of the next entry?
  bool IsEndTimeBeforeBreak(int index_cell);

  int AddSumMinutes(int index_cell, const std::string &duration_in_row, bool is_entry_running, int sum_task_minutes) const;
};
} // namespace tictac_lib

#endif
