/*
  Copyright (c) 2018-2019, Kay Stenschke
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

#ifndef CLASS_TTT_REPORT_RENDERER_CLI
#define CLASS_TTT_REPORT_RENDERER_CLI

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
  void PrintHeader(bool display_id = true, bool display_day_sum = true, bool display_saldo = true);
  void PrintHeaderCellForId(bool is_left_most);
  // Output <tr>s, returns amount of rows printed
  int PrintRows(int task_number, std::string comment = "", 
                bool display_sum = true, bool display_id = true,
                bool dispay_day_sum = true, bool display_balance = true);

  void PrintRowCellForId(bool is_left_most, int index_row);
  // Fill cell w/ spaces to keep width of cells in column identical
  void PrintRhsCellSpaces(int index_cell, int index_column);

  void PrintColumn(int index_cell, bool is_even, int index_row, int index_column, 
                   bool emphasize = false, bool display_id = true);
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
