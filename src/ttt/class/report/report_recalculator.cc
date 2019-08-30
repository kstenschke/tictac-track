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

#include <string>

#include "report_recalculator.h"
#include "../../helper/helper_date_time.h"
#include "../app/app_config.h"
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

  std::string balance_formatted, current_date, current_duration, current_task, sum_day_formatted, task_number;
  std::string previous_date;
  bool is_new_day;
  int sum_minutes_day = 0;
  int minutes_per_day_should = helper::DateTime::GetSumMinutesFromTime(AppConfig::GetConfigValueStatic("debit_per_day"));
  int balance = 0;

  auto *report_date_time = new ReportDateTime();
  ClearTaskMaps();
  html_ = parser->GetHtml();
  std::string weekday_name;
  std::string previous_meta;
  for (int row_index = 0; row_index <= last_index; row_index++) {
    // Update entry duration (start to end)
    ReportRecalculator::CalculateAndUpdateDuration(html_, row_index);

    parser->SetHtml(html_);
    
    std::string meta = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Meta);
    if (meta != previous_meta || weekday_name.empty())
      weekday_name = report_date_time->GetWeekdayByMeta(meta);
    
    ReportParser::UpdateColumn(html_, row_index, Report::ColumnIndexes::Index_Day, weekday_name);
    ReportParser::UpdateColumn(html_, row_index, Report::ColumnIndexes::Index_SumTaskDay, "");
    ReportParser::UpdateColumn(html_, row_index, Report::ColumnIndexes::Index_SumDay, "");
    ReportParser::UpdateColumn(html_, row_index, Report::ColumnIndexes::Index_Balance, "");

    parser->SetHtml(html_);

    int offset_tr = parser->GetOffsetTrOpenByIndex(html_, row_index);
    current_date = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Date, offset_tr);
    current_duration = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Duration, offset_tr);

    is_new_day = !previous_date.empty() && previous_date != current_date;
    if (is_new_day) {
      int debit_day = sum_minutes_day - minutes_per_day_should;
      balance += debit_day;
      balance_formatted = helper::DateTime::GetHoursFormattedFromMinutes(balance);
      ReportParser::UpdateColumn(html_, row_index - 1, Report::ColumnIndexes::Index_Balance, balance_formatted);

      sum_day_formatted = helper::DateTime::GetHoursFormattedFromMinutes(sum_minutes_day);
      ReportParser::UpdateColumn(html_, row_index - 1, Report::ColumnIndexes::Index_SumDay, sum_day_formatted);
      sum_minutes_day = 0;

      UpdateTaskSumsFromMaps(html_);
      ClearTaskMaps();
    } else {
      ReportParser::UpdateColumn(html_, row_index - 1, Report::ColumnIndexes::Index_SumDay, "");
    }

    parser->SetHtml(html_);
    task_number = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Task);

    if (!current_duration.empty()) {
      int duration_minutes = helper::DateTime::GetSumMinutesFromTime(current_duration);
      sum_minutes_day += duration_minutes;
      
      if (!task_number.empty()) AddToTaskMaps(task_number, row_index, duration_minutes);
    }

    previous_meta = meta;
    previous_date = current_date;
  }

  // Update last entry: balance, sum task/day (if any task), sum/day
  int debit_day = sum_minutes_day - minutes_per_day_should;
  balance += debit_day;
  balance_formatted = helper::DateTime::GetHoursFormattedFromMinutes(balance);
  ReportParser::UpdateColumn(html_, last_index, Report::ColumnIndexes::Index_Balance, balance_formatted);

  sum_day_formatted = helper::DateTime::GetHoursFormattedFromMinutes(sum_minutes_day);
  ReportParser::UpdateColumn(html_, last_index, Report::ColumnIndexes::Index_SumDay, sum_day_formatted);

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
  // An entry is still ongoing: silently abort
  if (parser->IsEntryRunning(row_index)) return;

  int offset_tr = parser->GetOffsetTrOpenByIndex(html, row_index);
  std::string start = parser->GetColumnContent(row_index, ColumnIndexes::Index_Start, offset_tr);
  std::string end = parser->GetColumnContent(row_index, ColumnIndexes::Index_End, offset_tr);
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
