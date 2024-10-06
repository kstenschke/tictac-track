/*
  Copyright (c) 2018-2020, Kay Stenschke
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

#ifndef TTT_CLASS_REPORT_REPORT_RENDERER_CLI_H_
#define TTT_CLASS_REPORT_REPORT_RENDERER_CLI_H_

#include <ttt/class/report/report_renderer.h>
#include <ttt/class/report/report_parser.h>
#include <ttt/class/report/report_browser.h>
#include <ttt/class//app/app_error.h>
#include <ttt/helper/helper_tui.h>
#include <ttt/helper/helper_file.h>

#include <iostream>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

namespace tictac_track {

class ReportRendererCli : public ReportRenderer {
 public:
  enum Themes {
    THEME_MAC_TERMINAL_VISOR = 0,
    THEME_LINUX_BASH_DEFAULT = 1,
    THEME_LINUX_BASH_ALTERNATIVE = 2,
    THEME_LINUX_BASH_DRACULA = 3,
    THEME_MAC_TERMINAL_DRACULA = 4,
  };

  // Constructor: init CLI ANSI color theme
  ReportRendererCli();

  // Pretty-print timesheet HTML (table) to CLI, using given filters
  bool PrintToCli(
      RenderScopes scope,
      int lookbehind_amount,
      int task_number,
      std::string comment = "");

  bool PrintBrowseDayTasks(int days_offset = 0);

 private:
  static std::string GetMessageHintClosestDayEntryBefore(int lookbehind_amount);

  std::string GetActiveScopeName();

 protected:
  // ANSI color/formatting codes for CLI output styling
  std::string theme_style_header_;
  std::string theme_style_default_;
  std::string theme_style_grid_;

  // Maximum mergeable amount of minutes between to entries
  // (lunch- or other long break)
  int minutes_break_;

  // Currently available max. amount of characters per comment within terminal
  int max_chars_per_comment_;

  // Amount of digits in maximum index
  uint32_t max_index_digits_;

  int amount_cells_;
  int offset_id_column_;

  // Output <thead>
  void PrintHeader(
      bool display_id = true,
      bool display_day_sum = true,
      bool display_saldo = true);

  void PrintHeaderCellForId(bool is_left_most) const;

  // Output <tr>s, returns amount of rows printed
  int PrintRows(
      int task_number,
      std::string comment = "",
      bool display_sum = true,
      bool display_id = true,
      bool dispay_day_sum = true,
      bool display_balance = true);

  void PrintRowCellForId(bool is_left_most, int index_row);

  // Fill cell w/ spaces to keep width of cells in column identical
  void PrintRhsCellSpaces(int index_cell, int index_column);

  void PrintColumn(
      int index_row,
      int index_column,
      int index_cell,
      bool display_id,
      bool emphasize,
      bool is_even);

  void PrintDurationSums(int task_number, int sum_task_minutes);

  // Render separator row (printed between days)
  std::string RenderSeparationRow();

  // Initialize color/formatting theme style codes
  void InitAnsiTheme();

  // Check end-time at given cell: is there an unmergeable gap
  // (lunch- or other break) between the start-time of the next entry?
  bool IsEndTimeBeforeBreak(int index_cell);

  int AddSumMinutes(
      int index_cell,
      const std::string &duration_in_row,
      bool is_entry_ongoing,
      int sum_task_minutes) const;

  void PrintRow(
      int index_row,
      int &index_cell,
      bool display_balance,
      bool is_even,
      bool is_around_break,
      bool display_day_sum,
      bool do_display,
      bool display_id,
      std::string &previous_day);
};

}  // namespace tictac_track

#endif  // TTT_CLASS_REPORT_REPORT_RENDERER_CLI_H_
