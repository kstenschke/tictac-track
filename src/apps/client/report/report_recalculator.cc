
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include "report_recalculator.h"
#include "lib/helper/helper_date_time.h"
#include "apps/client/app/app_config.h"
#include "report_file.h"
#include "report_crud.h"

namespace tictac_track {

/**
 * Constructor
 */
ReportRecalculator::ReportRecalculator(std::string html) {
  html_ = std::move(html);
}

/**
 * Recalculate: Update title, column titles, durations, day of week, sum task/day, sum day, balance
 */
bool ReportRecalculator::RecalculateAndUpdate() {
  ReportRecalculator *recalculator = new ReportRecalculator();

  return recalculator->LoadReportHtml() && recalculator->Recalculate();
}

/**
 * Non-static recalculate: Update title, column titles, durations, day of week, sum task/day, sum day, balance
 */
bool ReportRecalculator::Recalculate() {
  ReportParser *parser = new ReportParser(html_);
  int last_index = parser->GetLastIndex();
  if (last_index == -1 || (last_index == 0 && parser->IsAnyEntryRunning())) return false;

  parser->UpdateTitle();
  parser->UpdateTableHeader();

  std::vector<std::string> tasks_in_day;
  std::vector<int> task_durations;

  int amount_rows = parser->GetLastIndex();
  std::string balance_formatted, current_date, current_duration, current_task, sum_day_formatted, task_number;
  std::string previous_date;
  bool is_new_day;
  int sum_minutes_day = 0;
  int minutes_per_day_should = helper::DateTime::GetSumMinutesFromTime(AppConfig::GetConfigValueStatic("debit_per_day"));
  int balance = 0;

  auto *report_date_time = new ReportDateTime();
  ClearTaskMaps();
  html_ = parser->GetHtml();
  for (int index_row = 0; index_row <= amount_rows; index_row++) {
    // Update entry duration (start to end)
    ReportRecalculator::CalculateAndUpdateDuration(html_, index_row);

    parser->SetHtml(html_);
    std::string weekday_name = report_date_time->GetWeekdayByMeta(
        parser->GetColumnContent(index_row, Report::ColumnIndexes::Index_Meta));

    ReportParser::UpdateColumn(html_, index_row, Report::ColumnIndexes::Index_Day, weekday_name);
    ReportParser::UpdateColumn(html_, index_row, Report::ColumnIndexes::Index_SumTaskDay, "");
    ReportParser::UpdateColumn(html_, index_row, Report::ColumnIndexes::Index_SumDay, "");
    ReportParser::UpdateColumn(html_, index_row, Report::ColumnIndexes::Index_Balance, "");

    parser->SetHtml(html_);

    current_date = parser->GetColumnContent(index_row, Report::ColumnIndexes::Index_Date);
    current_duration = parser->GetColumnContent(index_row, Report::ColumnIndexes::Index_Duration);

    is_new_day = !previous_date.empty() && previous_date != current_date;
    if (is_new_day) {
      int debit_day = sum_minutes_day - minutes_per_day_should;
      balance += debit_day;
      balance_formatted = helper::DateTime::GetHoursFormattedFromMinutes(balance);
      ReportParser::UpdateColumn(html_, index_row - 1, Report::ColumnIndexes::Index_Balance, balance_formatted);

      sum_day_formatted = helper::DateTime::GetHoursFormattedFromMinutes(sum_minutes_day);
      ReportParser::UpdateColumn(html_, index_row - 1, Report::ColumnIndexes::Index_SumDay, sum_day_formatted);
      sum_minutes_day = 0;

      UpdateTaskSumsFromMaps(html_);
      ClearTaskMaps();
    } else {
      ReportParser::UpdateColumn(html_, index_row - 1, Report::ColumnIndexes::Index_SumDay, "");
    }

    parser->SetHtml(html_);
    task_number = parser->GetColumnContent(index_row, Report::ColumnIndexes::Index_Task);

    if (!current_duration.empty()) {
      int duration_minutes = helper::DateTime::GetSumMinutesFromTime(current_duration);
      if (!task_number.empty()) {
        AddToTaskMaps(task_number, index_row, duration_minutes);
      }
      sum_minutes_day += duration_minutes;
    }

    previous_date = current_date;
  }

  // Update last entry: balance, sum task/day (if any task), sum/day
  int debit_day = sum_minutes_day - minutes_per_day_should;
  balance += debit_day;
  balance_formatted = helper::DateTime::GetHoursFormattedFromMinutes(balance);
  ReportParser::UpdateColumn(html_, amount_rows, Report::ColumnIndexes::Index_Balance, balance_formatted);

  sum_day_formatted = helper::DateTime::GetHoursFormattedFromMinutes(sum_minutes_day);
  ReportParser::UpdateColumn(html_, amount_rows, Report::ColumnIndexes::Index_SumDay, sum_day_formatted);

  UpdateTaskSumsFromMaps(html_);
  return ReportCrud::SaveReport(html_);
}

void ReportRecalculator::ClearTaskMaps() {
  task_in_day_last_index_.clear();
  task_in_day_duration_sum_.clear();
}

void ReportRecalculator::AddToTaskMaps(std::string task_number, int index_row, int duration_minutes) {
  task_in_day_last_index_[task_number] = index_row;

  auto it = task_in_day_duration_sum_.find(task_number);
  if (it == task_in_day_duration_sum_.end()) task_in_day_duration_sum_[task_number] = 0;
  task_in_day_duration_sum_[task_number] += duration_minutes;
}

/**
 * Update sums of task/day by current task maps
 */
void ReportRecalculator::UpdateTaskSumsFromMaps(std::string &html) {
  for (auto const &task_row_index : task_in_day_last_index_) {
    std::string task_number = task_row_index.first;
    int index_row = task_row_index.second;

    int sum_minutes = task_in_day_duration_sum_[task_number];
    std::string sum_formatted = helper::DateTime::GetHoursFormattedFromMinutes(sum_minutes);

    ReportParser::UpdateColumn(html, index_row, Report::ColumnIndexes::Index_SumTaskDay, sum_formatted);
  }
}

/**
 * Extract start- and end-time and calculate and update duration column in given row from it
 */
void ReportRecalculator::CalculateAndUpdateDuration(std::string &html, int row_index) {
  ReportParser *parser = new ReportParser(html);
  // Still ongoing: silently abort
  if (parser->IsEntryRunning(row_index)) return;

  std::string start = parser->GetColumnContent(row_index, ColumnIndexes::Index_Start);
  std::string end = parser->GetColumnContent(row_index, ColumnIndexes::Index_End);
  std::string duration = ReportDateTime::GetDurationFormatted(start, end);

  ReportParser::UpdateColumn(html, row_index, ColumnIndexes::Index_Duration, duration);
}

/**
 * Load report HTML, than update duration
 */
std::string ReportRecalculator::CalculateAndUpdateDuration(int row_index) {
  std::string html = ReportFile::GetReportHtml();

  CalculateAndUpdateDuration(html, row_index);

  return html;
}
} // namespace tictac_lib
