/*
  Copyright (c) Kay Stenschke
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

#include <ttt/class/report/report_crud.h>
#include <ttt/class/app/app.h>
#include <ttt/class/app/app_config.h>
#include <ttt/class/report/report_parser.h>
#include <ttt/class/report/report_recalculator.h>

namespace tictac_track {

// Get object instance.
// Initialize at 1st call: ensure config file exists, load contents to attribute
ReportCrud &ReportCrud::GetInstance(bool clear) {
  // Instantiated on first use
  static ReportCrud instance;

  if (!instance.is_initialized_ || clear) {
    instance.Init(clear);
  }

  return instance;
}

// Initialize: ensure report file exists, setup attributes
// Optional: reset the timesheet file
void ReportCrud::Init(bool clear) {
  report_exists_ = EnsureReportExists(clear);
  report_date_time_ = new ReportDateTime();
  is_initialized_ = true;
}

bool ReportCrud::ReportExists() const {
  return report_exists_;
}

bool ReportCrud::EnsureReportExists(bool clear) {
  AppConfig &config = AppConfig::GetInstance();
  std::string report_file_path = config.GetReportFilePath();

  if (clear && helper::File::FileExists(report_file_path)) {
    helper::File::Remove(report_file_path.c_str());
  }

  if (helper::File::FileExists(report_file_path) || InitReportFile(false)) {
    return true;
  }

  std::cout << "Failed create report file at: " << report_file_path << "\n";

  return false;
}

std::string ReportCrud::RenderEntryHtml(
    bool is_new_day,
    const std::string& meta,
    const std::string& week_number,
    const std::string& weekday,
    const std::string& day_date,
    const std::string& time_start,
    const std::string& time_end,
    const std::string& task_number,
    const std::string& comment,
    const std::string& duration,
    const std::string& task_day_total,
    const std::string& day_total,
    const std::string& balance
) {
  std::stringstream entry;
  entry << "\n<tr" << (is_new_day ? " class=\"new-day\"" : "") << ">"
        << "<td class=\"meta\">" << meta << "</td>"
        << "<td>" << week_number << "</td>"
        << "<td>" << weekday << "</td>"
        << "<td>" << day_date << "</td>"
        << "<td>" << time_start << "</td>"
        << "<td>" << time_end << "</td>"
        << "<td>" << task_number << "</td>"
        << "<td>" << comment << "</td>"
        << "<td>" << duration << "</td>"
        << "<td>" << task_day_total << "</td>"
        << "<td>" << day_total << "</td>"
        << "<td>" << balance << "</td>"
        << "</tr>";

  return entry.str();
}

// Insert/update timesheet entry
bool ReportCrud::UpsertEntry(
    EntryStatus status,
    const char *comment,
    const char *task_number) {
  auto *parser = new ReportParser();

  if (!parser->LoadReportHtml()) {
    delete parser;

    return false;
  }

  std::string time_stopped = status == EntryStatus::Status_Started
    ? "..."
    : tictac_track::ReportDateTime::GetCurrentTime();

  std::string html = parser->GetHtml();

  bool hasTrackedItems =
      helper::String::GetSubStrCount(html.c_str(), "<tr") > 1;

  if (hasTrackedItems) {
    UpdateOngoingEntry(html, comment, true, time_stopped);
  }

  if (status != EntryStatus::Status_Started) {
    delete parser;

    return SaveReport(html);
  }

  parser->SetHtml(html);

  if (hasTrackedItems && parser->GetLastIndex() > 0) {
    parser->UpdateTitle();
  }

  html = parser->GetHtml();

  // Add newly started entry
  std::string date_current = report_date_time_->GetDateFormatted();

  std::string meta =
      (status == EntryStatus::Status_Started ? "s/" : "p/")
          + ReportDateTime::GetTimestampForMeta();

  const std::string &task =
      0 == std::strcmp(task_number, "0")
      || 0 == std::strcmp(task_number, "-1")
        ? ""
        : task_number;

  std::string entry = RenderEntryHtml(
      !parser->IsDateOfLatestEntry(date_current),
      meta,
      report_date_time_->GetCurrentWeekOfYear(),
      report_date_time_->GetCurrentDayOfWeek(),
      date_current,
      tictac_track::ReportDateTime::GetCurrentTime(),
      time_stopped,
      task, comment);

  delete parser;

  // Replace "</table>" by new entry + "</table>"
  auto offset = html.find("</table>", 0);

  html.replace(offset, 8, entry.append("\n</table>"));

  return SaveReport(html) && ReportRecalculator::RecalculateAndUpdate();
}

// Insert stopped entry w/ given attributes, after entry w/ given index.
// If given index == -1: insert before 1st entry
bool ReportCrud::InsertEntryAfter(
    std::string html,
    int row_index,
    std::string meta,
    const std::string& week_number,
    const std::string& weekday,
    const std::string& day_date,
    const std::string& time_start,
    const std::string& time_end,
    const std::string& task_number,
    const std::string& comment) {
  size_t insertion_offset;

  bool is_valid_row = row_index > -1;

  if (!is_valid_row) {
    // Insert before / as first entry,
    // independent on whether there's any yet
    insertion_offset = html.find("</thead>");

    if (std::string::npos == insertion_offset)
      return tictac_track::AppError::PrintError(
          "Error: Failed finding insertion offset.");

    insertion_offset += 8;
  } else {
    size_t offset_tr_open =
        ReportParser::GetOffsetTrOpenByIndex(html, row_index);

    insertion_offset = html.find(std::string("</tr>"), offset_tr_open);

    if (std::string::npos == insertion_offset) {
      return tictac_track::AppError::PrintError(
          std::string("Error: Failed finding insertion offset (row index: ")
              .append(helper::Numeric::ToString(row_index))
              .append(").")
              .c_str());
    }

    insertion_offset += 5;
  }

  std::string duration =
      !time_start.empty()
      && !time_end.empty()
        ? ReportDateTime::GetDurationFormatted(time_start, time_end)
        : "";

  // TODO(kay): determine by looking at date of entry before
  bool is_new_day = true;

  meta[0] = 'p';

  std::string entry = RenderEntryHtml(
      is_new_day,
      meta,
      week_number,
      weekday,
      day_date,
      time_start,
      time_end,
      task_number,
      comment,
      duration);

  html = html.replace(insertion_offset, 0, entry);

  return SaveReport(html);
}

// Update ongoing entry: append text to comment, set stop-time, set stopped
void ReportCrud::UpdateOngoingEntry(
    std::string &html,
    const std::string& add_to_comment,
    bool do_stop,
    const std::string& time_stopped) {
  bool has_comment = !add_to_comment.empty();

  auto *parser = new ReportParser(html);

  if (
      !parser->IsAnyEntryOngoing()
      || (!do_stop && !has_comment)) {
    delete parser;

    return;
  }

  auto offset_td_start = parser->GetOffsetTdStartInOngoingEntry();
  auto offset_td_end = html.find("<td>", offset_td_start + 1);

  std::string duration;

  if (do_stop) {
    html = html.replace(offset_td_end + 4, 3, time_stopped);

    // Extract starting time
    auto time_str_len = time_stopped.size();

    std::string time_started = html.substr(offset_td_start + 4, time_str_len);

    duration = tictac_track::ReportDateTime::GetDurationFormatted(
        time_started,
        time_stopped);
  }

  auto offset_td_task = html.find("<td>", offset_td_end + 1);
  auto offset_td_open_comment = html.find("<td>", offset_td_task + 1);

  if (has_comment) {
    auto offset_td_close_comment =
        html.find("</td>", offset_td_open_comment + 1);

    // TODO(kay): detect whether there is any text
    //  in the content column already, add separator, e.g. ", " than.

    html = html.replace(offset_td_close_comment, 0, add_to_comment);
  }

  auto offset_td_duration =
      html.find("<td>", offset_td_open_comment + 1);

  if (do_stop && !duration.empty()) {
    // Insert duration

    // TODO(kay): replace all content of duration,
    //  though there shouldn't be any, not just assume there isn't

    html = html.replace(offset_td_duration + 4, 0, duration);

    // Change status in meta column from in-progress ("s") to stopped ("p")
    html = helper::String::ReplaceAll(
        html,
        "<td class=\"meta\">s/", "<td class=\"meta\">p/");
  }

  delete parser;
}

// Add timesheet entry: start work
bool ReportCrud::StartEntry(const char *comment, const char *task_number) {
  return
      (strcmp(task_number, "") == 0
          || (task_number[0] == '\0'
              || task_number[0] == '-'))
      && !CurrentDayHasTasks()
        ? UpsertEntry(
            EntryStatus::Status_Started,
            comment,
            AppConfig::GetDefaultFirstTaskOfDay())
        : UpsertEntry(EntryStatus::Status_Started, comment, task_number);
}

bool ReportCrud::StartEntry(const char *comment, int task_number) {
  return StartEntry(comment, helper::Numeric::ToString(task_number).c_str());
}

bool ReportCrud::AddFullDayEntry(
    int offset_days,
    std::string comment,
    const std::string& task_number) {
  std::string html = GetReportHtml();

  if (html.empty()) return false;

  AppConfig config = AppConfig::GetInstance();

  std::string format_date = config.GetConfigValue("format_date");

  std::string date_meta =
      helper::DateTime::GetCurrentTimeFormatted("%Y/%m/%W/%d", offset_days);

  std::string date_day =
      helper::DateTime::GetCurrentTimeFormatted(
          format_date.c_str(),
          offset_days);

  if (std::string::npos != html.find(date_meta + "</td>")) {
    tictac_track::AppError::PrintError(
        std::string("Cannot add full-day entry. There are entries already on ")
            .append(date_day)
            .append(".")
            .c_str());

    return false;
  }

  std::string week_number =
      helper::DateTime::GetCurrentTimeFormatted("%W", offset_days);

  auto *parser = new ReportParser(html);

  int row_index =
      -1 == parser->GetLastIndex()
        ? -1
        : tictac_track::ReportParser::GetIndexBeforeMetaDate(date_meta);

  delete parser;

  std::string meta = "p/" + date_meta;

  auto *report_date_time = new ReportDateTime();
  std::string weekday = report_date_time->GetWeekdayByMeta(meta);
  delete report_date_time;

  std::string time_start = config.GetConfigValue("default_daily_start_time");

  int time_start_minutes = helper::DateTime::GetSumMinutesFromTime(time_start);

  std::string required_per_day = config.GetConfigValue("debit_per_day");

  int required_per_day_minutes =
      helper::DateTime::GetSumMinutesFromTime(required_per_day);

  int time_end_minutes = time_start_minutes + required_per_day_minutes;

  std::string time_end =
      helper::DateTime::GetHoursFormattedFromMinutes(time_end_minutes);

  helper::String::Trim(comment);

  return ReportCrud::InsertEntryAfter(
      html,
      row_index,
      meta,
      week_number,
      weekday,
      date_day,
      time_start,
      time_end,
      task_number,
      comment);
}

// Add timesheet entry: stop currently ongoing entry
bool ReportCrud::StopEntry(const char *comment) {
  auto *parser = new ReportParser();

  if (!parser->LoadReportHtml()) {
    delete parser;

    return false;
  }

  AppConfig config = AppConfig::GetInstance();

  std::string do_safeguard_issue_number =
      config.GetConfigValue("require_issue_no_when_stopping_entry");

  if ("1" == do_safeguard_issue_number
      && !parser->OngoingEntryContainsIssueNumber()) {
    SafeguardToAddIssueNumber();
  }

  std::string do_safeguard_comment =
      config.GetConfigValue("require_comment_when_stopping_entry");

  if ("1" == do_safeguard_comment && !parser->OngoingEntryContainsComment()) {
    parser->LoadReportHtml();

    SafeguardToAddComment(parser->GetHtml());
  }

  delete parser;

  return UpsertEntry(EntryStatus::Status_Stopped, comment);
}

void ReportCrud::SafeguardToAddIssueNumber() {
  std::cout
    << "Please enter the related issue number of the entry to be stopped: ";

  int issue_number = 0;

  std::string issue_number_str;

  while (0 == issue_number) {
    std::cin >> issue_number_str;

    if (helper::String::IsNumeric(issue_number_str)
        && issue_number_str != "0") {
      issue_number = helper::String::ToInt(issue_number_str);
      UpdateIssueNumber(issue_number);
    } else {
      std::cout
        << "Invalid issue number (must be numeric).\n"
           "Please enter issue number: ";
    }
  }
}

void ReportCrud::SafeguardToAddComment(std::string html) {
  std::cout << "Please enter a comment for the entry to be stopped: ";

  std::string comment;
  char input[100];
  int i = -1;

  while (comment.empty()) {
    i++;

    std::cin.getline(input, sizeof(input));
    comment = input;

    if (!comment.empty()) {
      UpdateOngoingEntry(html, comment, false, "");

      SaveReport(html);
    } else if (i > 0) {
      // TODO(kay): use more elegant way to
      //  only after 1st getline-invocation execute validation/output
      std::cout << "Invalid comment (cannot be empty).\nPlease enter comment: ";
    }
  }
}

// Append given text to comment of given or latest entry
bool ReportCrud::AppendComment(
    std::string &comment,
    int row_index,
    bool start_with_space) {
  if (comment.empty()) {
    return false;
  }

  auto *parser = new ReportParser();

  if (!parser->LoadReportHtml()) {
    delete parser;

    return false;
  }

  std::string append = std::string(start_with_space ? " " : "").append(comment);

  const std::string &html = parser->AppendToColumn(
      row_index,
      Report::ColumnIndexes::Index_Comment,
      append);

  delete parser;

  return SaveReport(const_cast<std::string &>(html));
}

// Set task number of latest or given entry
bool ReportCrud::UpdateIssueNumber(int task_number, int row_index) {
  std::string html = GetReportHtml();

  if (html.empty()) {
    return false;
  }

  std::string task_number_str = task_number > 0
      ? helper::Numeric::ToString(task_number)
      : "";

  ReportParser::UpdateColumn(
      html,
      row_index,
      Report::ColumnIndexes::Index_Issue,
      task_number_str);

  return SaveReport(html);
}

// Merge (time, task, comment of) given entry w/ next one (and remove next one)
bool ReportCrud::Merge(int row_index) {
  auto *parser = new ReportParser();

  if (!parser->LoadReportHtml()) {
    delete parser;

    return false;
  }

  std::string date = parser->GetColumnContent(
      row_index,
      Report::ColumnIndexes::Index_Date);

  std::string date_next = parser->GetColumnContent(
      row_index + 1,
      Report::ColumnIndexes::Index_Date);

  if (0 != std::strcmp(date.c_str(), date_next.c_str())) {
    tictac_track::AppError::PrintError(
        (std::string("Cannot merge: Next entry is in different day (")
            + date + " / " + date_next + ")").c_str());

    delete parser;

    return false;
  }

  int minutes_gap = parser->GetMinutesBetweenEntryAndNext(row_index);

  if (!IsMergeableAmountMinutes(minutes_gap)) {
    delete parser;

    return false;
  }

  std::string html = parser->GetHtml();

  int offset_tr =
      tictac_track::ReportParser::GetOffsetTrOpenByIndex(html, row_index);

  int offset_tr_next =
      tictac_track::ReportParser::GetOffsetTrOpenByIndex(html, row_index + 1);

  std::string
      time_end_second = parser->GetColumnContent(
          row_index + 1,
          Report::ColumnIndexes::Index_End,
          offset_tr_next);

  std::string comment = parser->GetCommentMergedWithNextByRowIndex(
      row_index,
      offset_tr,
      offset_tr_next);

  std::string task = parser->GetColumnContent(
      row_index,
      Report::ColumnIndexes::Index_Issue,
      offset_tr);

  if (task.empty()) {
    task = parser->GetColumnContent(
        row_index + 1,
        Report::ColumnIndexes::Index_Issue,
        offset_tr_next);
  }

  // Merge meta: set merged entry to status of 2nd entry before merge
  std::string meta_second = parser->GetColumnContent(
      row_index + 1,
      Report::ColumnIndexes::Index_Meta,
      offset_tr_next);

  if ('s' == meta_second[0]) {
    std::string meta_first = parser->GetColumnContent(
        row_index,
        Report::ColumnIndexes::Index_Meta,
        offset_tr);

    meta_first[0] = 's';

    delete parser;

    ReportParser::UpdateColumn(
        html,
        row_index,
        Report::ColumnIndexes::Index_Meta,
        meta_first);
  }

  ReportParser::UpdateColumn(
      html,
      row_index,
      Report::ColumnIndexes::Index_End,
      time_end_second);

  ReportParser::UpdateColumn(
      html,
      row_index,
      Report::ColumnIndexes::Index_Comment,
      comment);

  ReportParser::UpdateColumn(
      html,
      row_index,
      Report::ColumnIndexes::Index_Issue,
      task);

  ReportRecalculator::CalculateAndUpdateDuration(html, row_index);

  // Remove next entry and save updated entry
  return RemoveEntryById(html, row_index + 1);
}

bool ReportCrud::IsMergeableAmountMinutes(int amount_minutes) {
  AppConfig config = AppConfig::GetInstance();

  std::string max_gap_str = config.GetConfigValue("max_mergeable_minutes_gap");

  int max_gap = helper::String::ToInt(max_gap_str);

  return amount_minutes <= max_gap || tictac_track::AppError::PrintError(
      std::string("Cannot merge: Entries have a gap of ")
          .append(helper::Numeric::ToString(amount_minutes))
          .append(" minutes (allowed maximum: ")
          .append(max_gap_str)
          .append(1 == amount_minutes ? " minute)." : " minutes).").c_str());
}

// Remove given amount of latest entries (but not more than exist)
bool ReportCrud::RemoveEntries(int amount) {
  auto *parser = new ReportParser();

  if (!parser->LoadReportHtml() || -1 == parser->GetLastIndex()) {
    delete parser;

    return false;
  }

  if (amount - 1 >= parser->GetLastIndex()) {
    delete parser;

    return Reset();
  }

  std::string html = parser->GetHtml();

  delete parser;

  for (int i = 0; i < amount; i++) {
    std::size_t offset_last_tr = html.rfind("\n<tr");

    if (std::string::npos == offset_last_tr) {
      continue;
    }

    std::size_t offset_last_tr_end = html.rfind("</tr>");

    if (std::string::npos != offset_last_tr_end) {
      html = html.erase(
          offset_last_tr,
          offset_last_tr_end - offset_last_tr + 6);
    }
  }

  return SaveReport(html) && ReportRecalculator::RecalculateAndUpdate();
}

// Remove 1 entry at given index
bool ReportCrud::RemoveEntryById(std::string &html, int id) {
  if (0 > id) {
    return false;
  }

  auto *parser = new ReportParser(html);

  int last_index = parser->GetLastIndex();

  delete parser;

  if (id > last_index) {
    return tictac_track::AppError::PrintError(
        std::string("Cannot remove entry ")
            .append(helper::Numeric::ToString(id))
            .append(", last index is ")
            .append(helper::Numeric::ToString(last_index))
            .c_str());
  }

  int offset_tr_open = ReportParser::GetOffsetTrOpenByIndex(html, id);

  if (-1 == offset_tr_open) {
    return false;
  }

  size_t offset_tr_close = html.find(
      "</tr>",
      static_cast<uint32_t> (offset_tr_open));

  if (std::string::npos == offset_tr_close) return false;

  html = html.erase(
      static_cast<uint32_t> (offset_tr_open),
      offset_tr_close - offset_tr_open + 6);

  return SaveReport(html) && ReportRecalculator::RecalculateAndUpdate();
}

bool ReportCrud::RemoveEntryById(int id) {
  std::string html = GetReportHtml();

  return !html.empty() && RemoveEntryById(html, id);
}

bool ReportCrud::Reset() {
  return InitReportFile(true);
}

bool ReportCrud::IsAnyEntryOngoing() {
  std::string html = GetReportHtml();

  return !html.empty() && ReportParser::IsAnyEntryOngoing(html);
}

bool ReportCrud::CurrentDayHasTasks() {
  std::string html = GetReportHtml();

  if (helper::String::GetSubStrCount(html.c_str(), "<tr") == 1) {
    // There are no items tracked at all
    return false;
  }

  std::string date_cell =
      "<td>"
      + report_date_time_->GetDateFormatted()
      + "</td>";

  return std::string::npos != html.find(date_cell);
}

}  // namespace tictac_track
