
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cstring>
#include "renderer_cli.h"
#include "../helper/helper_string.h"
#include "../app/config.h"
#include "../helper/numeric.h"
#include "html_parser.h"
#include "../helper/date_time.h"
#include "../helper/html.h"
#include "../app/error.h"

namespace timesheetplus {

const std::string ReportRendererCli::kAnsiFormatReset = "\033[0m";

/**
 * Constructor: init CLI ANSI color theme
 */
ReportRendererCli::ReportRendererCli() {
  InitAnsiTheme();
  offset_id_column_ = HelperString::ToInt(AppConfig::GetInstance().GetConfigValue("id_column"));
}

/**
 * Pretty-print timesheet HTML (table) to CLI, using given filters
 */
bool ReportRendererCli::PrintToCli(RenderScopes scope, int lookbehind_amount, int task_number, std::string comment) {
  render_scope_ = scope;
  if (!ExtractPartsFromReport(lookbehind_amount)) return false;

  max_index_digits_ = HelperNumeric::GetAmountDigits(amount_rows_);

  PrintHeader();
  if (0 != PrintRows(task_number, std::move(comment))) return true;

  std::string message = rows_filter_.empty()
                        ? std::string(" No entries found.")
                        : std::string(" No entries with ").append("\"").append(rows_filter_).append("\" in ")
                            .append(GetActiveScopeName()).append(" column found.");

  if (RenderScopes::Scope_Day == render_scope_)
    message = message.append(GetMessageHintClosestDayEntryBefore(lookbehind_amount));

  AppError::PrintError(message.append("\n").c_str());

  return false;
}

std::string ReportRendererCli::GetMessageHintClosestDayEntryBefore(int lookbehind_amount) {
  ReportHtmlParser *parser = new ReportHtmlParser();
  if (!parser->LoadReportHtml() || -1 == parser->GetLastIndex()) return std::string("");

  int available_lookbehind_offset = parser->GetExistingEntryOffsetBefore(lookbehind_amount);

  return std::string(" Closest entry is at: d ").append(HelperNumeric::ToString(available_lookbehind_offset))
      .append(" (").append(HelperDateTime::GetCurrentTimeFormatted("%Y/%m/%d", available_lookbehind_offset))
      .append(")");
}

std::string ReportRendererCli::GetActiveScopeName() {
  switch (render_scope_) {
    case RenderScopes::Scope_Day:return std::string("day");
    case RenderScopes::Scope_Week:return std::string("week");
    case RenderScopes::Scope_Month:return std::string("month");
    case RenderScopes::Scope_Invalid:
    default:return std::string("");
  }
}

void ReportRendererCli::PrintHeader() {
  std::cout << "\n" << theme_style_header_;
  if (offset_id_column_ == 0) {
    PrintHeaderCellForId(true);
  } else {
    std::cout << " ";
  }

  int content_len;
  // Skip meta-column (start from index 1 instead of 0)
  for (int index_column = 1; index_column < amount_columns_; index_column++) {
    if (offset_id_column_ == 0 || index_column > 1) std::cout << "| ";

    std::string column_title = column_titles_[index_column];
    std::cout << HelperHtml::Decode(column_title) << " ";

    content_len = HelperString::GetAmountChars(HelperHtml::Decode(column_title));
    if (content_len < column_content_max_len_[index_column]) {
      int column_len_diff = column_content_max_len_[index_column] - content_len;
      std::cout << std::string(static_cast<unsigned long>(column_len_diff), ' ');
    }

    if (offset_id_column_ == index_column) PrintHeaderCellForId(false);
  }
  std::cout << " " << kAnsiFormatReset << "\n";
}

void ReportRendererCli::PrintHeaderCellForId(bool is_left_most) {
  if (!is_left_most) {
    std::cout << "|";
  }
  unsigned long amount_spaces = max_index_digits_ - 1;
  if (amount_spaces > 0) std::cout << std::string((amount_spaces), ' ');
  std::cout << " ID ";
}

/**
 * Output <tr>s using given filters, returns amount of rows printed
 */
int ReportRendererCli::PrintRows(int task_number, std::string comment) {
  std::string task_number_str = task_number == -1 ? "" : HelperNumeric::ToString(task_number);

  // Pre-render grid line inbetween content of two rows
  std::string separation_row = RenderSeparationRow();

  bool has_comment = !comment.empty();
  int index_cell = 0;
  auto amount_cells = static_cast<int>(cells_.size());
  bool is_even = true;

  std::string comment_in_row;
  std::string task_number_in_row;
  std::string date_in_row;
  std::string duration_in_row;
  std::string previous_day;
  bool is_entry_running;
  bool display_viewed_sum = RenderScopes::Scope_All != render_scope_ || -1 != task_number;

  int sum_task_minutes = 0;
  int amount_rows_printed = 0;
  for (int index_row = 0; index_row < amount_rows_ && index_cell < amount_cells; index_row++) {
    comment_in_row     = cells_[index_cell + ReportHtmlParser::ColumnIndexes::Index_Comment];
    date_in_row        = cells_[index_cell + ReportHtmlParser::ColumnIndexes::Index_Date];
    duration_in_row    = cells_[index_cell + ReportHtmlParser::ColumnIndexes::Index_Duration];
    task_number_in_row = cells_[index_cell + ReportHtmlParser::ColumnIndexes::Index_Week + 5];

    is_entry_running = 's' == cells_[index_cell + ReportHtmlParser::ColumnIndexes::Index_Meta][0];

    bool do_display = (task_number == -1 || 0 == std::strcmp(task_number_in_row.c_str(), task_number_str.c_str()))
                      && (!has_comment || HelperString::Contains(comment_in_row, comment));
    if (do_display) {
      amount_rows_printed++;

      // New day: preceded by separation row
      if (index_cell > 0 && previous_day != cells_[index_cell + 2]) std::cout << separation_row << "\n";

      if (display_viewed_sum)
        sum_task_minutes = AddSumMinutes(index_cell, duration_in_row, is_entry_running, sum_task_minutes);

      if (offset_id_column_ == 0) PrintRowCellForId(true, index_row + id_first_row_rendered_);

      is_even = !is_even;
    }
    // Skip meta-column (start from index 1 instead of 0)
    for (int index_column = 0; index_column < amount_columns_ && index_cell < amount_cells; index_column++) {
      if (index_column == 2) previous_day = cells_[index_cell];

      if (do_display) PrintColumn(index_cell, is_even, index_row, index_column);

      // Skip meta cell
      index_cell++;
    }

    if (do_display) {
      std::cout << " " << kAnsiFormatReset << "\n";
      if (is_even) std::cout << kAnsiFormatReset;
    }
  }

  if (display_viewed_sum && 0 < sum_task_minutes) PrintDurationSums(task_number, sum_task_minutes);

  std::cout << kAnsiFormatReset;

  return amount_rows_printed;
}

int ReportRendererCli::AddSumMinutes(int index_cell, const std::string &duration_in_row, bool is_entry_running,
                                     int sum_task_minutes) const {
  if (is_entry_running) {
          int minutes_start = HelperDateTime::GetSumMinutesFromTime(cells_[index_cell + Index_Start]);
          int minutes_now   = HelperDateTime::GetSumMinutesFromTime();
          sum_task_minutes += minutes_now - minutes_start;
        } else if (!duration_in_row.empty()) sum_task_minutes += HelperDateTime::GetSumMinutesFromTime(duration_in_row);
  return sum_task_minutes;
}

void ReportRendererCli::PrintColumn(int index_cell, bool is_even, int index_row, int index_column) {
  if (index_column > 1) {
    // Skip column 0 (meta)
    std::cout << (is_even ? theme_style_default_ : theme_style_grid_) << "| " << kAnsiFormatReset;
  } else if (index_column == 1 && offset_id_column_ > 0) std::cout << " ";

  cells_[index_cell];
  if (index_column > 0) {
    std::string content = cells_[index_cell];
    if (index_column == Index_Comment) content = HelperHtml::Decode(content);
    std::cout << (is_even ? theme_style_default_ : "") << content << " ";
  }

  PrintRhsCellSpaces(index_cell, index_column);

  if (offset_id_column_ == index_column && index_column > 0)
    PrintRowCellForId(false, index_row + id_first_row_rendered_);
}

void ReportRendererCli::PrintDurationSums(int task_number, int sum_task_minutes) {
  std::string sum_duration_formatted = HelperDateTime::GetHoursFormattedFromMinutes(sum_task_minutes);
  std::cout << "    ";
  for (int index_column = 0; index_column < Index_Duration; index_column++) {
    PrintRhsCellSpaces(-1, index_column);
    std::cout << " ";
  }

  std::cout << "  Σ";
  if (-1 != task_number) std::cout << " " << task_number;
  std::cout << ": " << sum_duration_formatted << "\n";
}

/**
 * Render separator row (printed between days)
 */
std::string ReportRendererCli::RenderSeparationRow() {
  std::string separation_row = theme_style_grid_ + std::string(4 + max_index_digits_, '-');
  for (int indexColumn = 1; indexColumn < amount_columns_; indexColumn++) {
    separation_row.append(std::string(column_content_max_len_[indexColumn] + 3, '-'));
  }

  return separation_row + kAnsiFormatReset;
}

void ReportRendererCli::PrintRowCellForId(bool is_left_most, int index_row) {
  if (!is_left_most) std::cout << '|';
  std::cout << ' ';

  unsigned long amountDigitsInCurrentRowIndex = HelperNumeric::GetAmountDigits(index_row);
  unsigned long amountSpaces = max_index_digits_ - amountDigitsInCurrentRowIndex;

  if (amountSpaces > 0) std::cout << std::string(amountSpaces, ' ');
  std::cout << ' ' << index_row << ' ';

  if (is_left_most) std::cout << "| ";

  std::cout << theme_style_grid_;
}

/**
 * Fill cell w/ spaces to keep width of cells in column identical
 */
void ReportRendererCli::PrintRhsCellSpaces(int index_cell, int index_column) {
  int content_len = -1 == index_cell
                    ? 0
                    : HelperString::GetAmountChars(HelperHtml::Decode(cells_[index_cell]));

  if (content_len < column_content_max_len_[index_column]) {
    int column_len_diff = column_content_max_len_[index_column] - content_len;
    std::cout << std::string(static_cast<unsigned long>(column_len_diff), ' ');
  }
}

/**
 * Initialize color/formatting theme style codes
 */
void ReportRendererCli::InitAnsiTheme() {
  AppConfig config = AppConfig::GetInstance();
  int theme_id = HelperString::ToInt(config.GetConfigValue("cli_theme").c_str(), 0);

  switch (theme_id) {
    case 4:
      // Mac OS Terminal Dracula
      //theme_style_header_  = "\033[0;100m";
      theme_style_header_ = "\033[0;100m";
      theme_style_default_ = "\033[17;36m";
      theme_style_grid_ = "\033[0m";
      break;
    case 3:
      // Linux Bash Dracula
      theme_style_header_ = "\033[1;40m";
      theme_style_default_ = "\033[17;34m";
      theme_style_grid_ = "\033[0m";
      break;
    case 2:
      // Linux Bash alternative
      theme_style_header_ = "\033[1;44m";
      theme_style_default_ = "\033[17;36m";
      theme_style_grid_ = "\033[0m";
      break;
    case 1:
      // Linux Bash default
      theme_style_header_ = "\033[17;104m";
      theme_style_default_ = "\033[17;34m";
      theme_style_grid_ = "\033[0m";
      break;
    case 0:
    default:
      // Mac OS Terminal "Visor"
      theme_style_header_ = "\033[1;44m";
      theme_style_default_ = "\033[17;100m";
      theme_style_grid_ = "\033[0m";
  }
}
} // namespace timesheetplus
