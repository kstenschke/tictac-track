
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <sstream>
#include <cstring>
#include "report_crud.h"
#include "app/app_config.h"
#include "helper/helper_file.h"
#include "report_parser.h"
#include "report_recalculator.h"
#include "helper/helper_string.h"
#include "helper/helper_numeric.h"
#include "helper/helper_date_time.h"
#include "app/app_error.h"

namespace tictac_track {

  /**
   * Get object instance. Initialize at 1st call: ensure config file exists, load contents to attribute
   */
  ReportCrud &ReportCrud::GetInstance() {
    // Instantiated on first use
    static ReportCrud instance;

    if (!instance.is_initialized_) {
      instance.Init();
    }

    return instance;
  }

  /**
   * Initialize: ensure report file exists, setup attributes
   */
  void ReportCrud::Init() {
    report_exists_ = EnsureReportExists();
    report_date_time_ = new ReportDateTime();
    is_initialized_ = true;
  }

  bool ReportCrud::ReportExists() {
    return report_exists_;
  }

  bool ReportCrud::EnsureReportExists() {
    AppConfig &config = AppConfig::GetInstance();
    std::string report_file_path = config.GetReportFilePath();
    if (helper::File::FileExists(report_file_path) || InitReportFile(false)) return true;

    std::cout << "Failed create report file at: " << report_file_path << "\n";
    return false;
  }

  std::string ReportCrud::RenderEntryHtml(
          bool is_new_day, std::string meta, std::string week_number, std::string weekday, std::string day_date,
          std::string time_start, std::string time_end, std::string task_number, std::string comment, std::string duration,
          std::string task_day_total, std::string day_total, std::string balance
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

  /**
   * Insert/update timesheet entry
   */
  bool ReportCrud::UpsertEntry(EntryStatus status, const char *comment, const char *task_number) {
    ReportParser *parser = new ReportParser();
    if (!parser->LoadReportHtml()) return false;

    std::string time_stopped = status == EntryStatus::Status_Started ? "..." : report_date_time_->GetCurrentTime();
    std::string html = parser->GetHtml();
    UpdateRunningEntry(html, comment, true, time_stopped);

    if (status != EntryStatus::Status_Started) return SaveReport(html);

    parser->SetHtml(html);
    if (parser->GetLastIndex() > 0) parser->UpdateTitle();
    html = parser->GetHtml();

    // Add newly started entry
    std::string date_current = report_date_time_->GetDateFormatted();
    std::string meta = (status == EntryStatus::Status_Started ? "s/" : "p/") + report_date_time_->GetTimestampForMeta();

    const std::string &task = 0 == std::strcmp(task_number, "0") || 0 == std::strcmp(task_number, "-1")
            ? "" : task_number;
    std::string entry = RenderEntryHtml(
            !parser->IsDateOfLatestEntry(date_current), meta, report_date_time_->GetCurrentWeekOfYear(),
            report_date_time_->GetCurrentDayOfWeek(), date_current, report_date_time_->GetCurrentTime(), time_stopped,
            task, comment);

    // Replace "</table>" by new entry + "</table>"
    unsigned long offset = html.find("</table>", 0);
    html.replace(offset, 8, entry.append("\n</table>"));

    return SaveReport(html) && ReportRecalculator::RecalculateAndUpdate();
  }

  /**
   * Insert stopped entry w/ given attributes, after entry w/ given index. If given index == -1: insert before 1st entry
   */
  bool ReportCrud::InsertEntryAfter(std::string html, int row_index, std::string meta, std::string week_number,
          std::string weekday, std::string day_date, std::string time_start,
          std::string time_end, std::string task_number, std::string comment
          ) {
    size_t insertion_offset;

    bool is_valid_row = row_index > -1;
    if (!is_valid_row) {
      // Insert before / as first entry, independent on whether there's any yet
      insertion_offset = html.find("</thead>");
      if (std::string::npos == insertion_offset) {
        return tictac_track::AppError::PrintError("Error: Failed finding insertion offset.");
      }
      insertion_offset += 8;
    } else {
      size_t offset_tr_open = ReportParser::GetOffsetTrOpenByIndex(html, row_index);
      insertion_offset = html.find(std::string("</tr>"), offset_tr_open);
      if (std::string::npos == insertion_offset) {
        return tictac_track::AppError::PrintError(
                std::string("Error: Failed finding insertion offset (row index: ").append(helper::Numeric::ToString(row_index))
                .append(").").c_str());
      }
      insertion_offset += 5;
    }

    std::string duration = !time_start.empty() && !time_end.empty()
            ? ReportDateTime::GetDurationFormatted(time_start, time_end)
            : "";

    // @todo determine by looking at date of entry before
    bool is_new_day = true;

    meta[0] = 'p';
    std::string entry = RenderEntryHtml(is_new_day, meta, std::move(week_number),
            std::move(weekday), std::move(day_date), time_start, time_end,
            std::move(task_number), std::move(comment), duration);
    html = html.replace(insertion_offset, 0, entry);

    return SaveReport(html);
  }

  /**
   * Update running entry: append text to comment, set stop-time, set stopped
   */
  void ReportCrud::UpdateRunningEntry(std::string &html, std::string add_to_comment, bool do_stop,
          std::string time_stopped) {
    bool has_comment = !add_to_comment.empty();

    ReportParser *parser = new ReportParser(html);
    if (!parser->IsAnyEntryRunning() || (!do_stop && !has_comment)) return;

    unsigned long offset_td_start = parser->GetOffsetTdStartInRunningEntry();
    unsigned long offset_td_end = html.find("<td>", offset_td_start + 1);

    std::string duration;
    if (do_stop) {
      html = html.replace(offset_td_end + 4, 3, time_stopped);

      // Extract starting time
      unsigned long time_str_len = time_stopped.size();
      std::string time_started = html.substr(offset_td_start + 4, time_str_len);
      duration = report_date_time_->GetDurationFormatted(time_started, time_stopped);
    }

    unsigned long offset_td_task = html.find("<td>", offset_td_end + 1);
    unsigned long offset_td_open_comment = html.find("<td>", offset_td_task + 1);

    if (has_comment) {
      unsigned long offset_td_close_comment = html.find("</td>", offset_td_open_comment + 1);
      // TODO detect whether there is any text in the content column already, add separator, e.g. ", " than.
      html = html.replace(offset_td_close_comment, 0, add_to_comment);
    }

    unsigned long offset_td_duration = html.find("<td>", offset_td_open_comment + 1);
    //int offset_td_duration_end = html.find("</td>", offset_td_duration );
    if (do_stop && !duration.empty()) {
      // Insert duration
      // TODO replace all content of duration, though there shouldn't be any, not just assume there isn't
      //html = html.replace(offset_td_duration + 4, offset_td_duration_end - offset_td_duration + 4, duration);
      html = html.replace(offset_td_duration + 4, 0, duration);
      // Change status in meta column from in-progress ("s") to stopped ("p")
      html = helper::String::ReplaceAll(html, "<td class=\"meta\">s/", "<td class=\"meta\">p/");
    }
  }

  /**
   * Add timesheet entry: start work
   */
  bool ReportCrud::StartEntry(const char *comment, const char *task_number) {
    return (task_number[0] == '\0' || task_number[0] == '-') && !CurrentDayHasTasks()
           ? UpsertEntry(EntryStatus::Status_Started, comment, AppConfig::GetDefaultFirstTaskOfDay())
           : UpsertEntry(EntryStatus::Status_Started, comment, task_number);
  }

  bool ReportCrud::StartEntry(const char *comment, int task_number) {
    return StartEntry(comment, helper::Numeric::ToString(task_number).c_str());
  }

  bool ReportCrud::AddFullDayEntry(int offset_days, std::string comment, std::string task_number) {
    std::string html = GetReportHtml();
    if (html.empty()) return false;

    AppConfig config = AppConfig::GetInstance();

    std::string format_date = config.GetConfigValue("format_date");
    std::string date_meta = helper::DateTime::GetCurrentTimeFormatted("%Y/%m/%W/%d", offset_days);
    std::string date_day = helper::DateTime::GetCurrentTimeFormatted(format_date.c_str(), offset_days);

    if (std::string::npos != html.find(date_meta + "</td>")) {
      tictac_track::AppError::PrintError(
              std::string("Cannot add full-day entry. There are entries already on ").append(date_day).append(".").c_str()
              );
      return false;
    }

    std::string week_number = helper::DateTime::GetCurrentTimeFormatted("%W", offset_days);

    ReportParser *parser = new ReportParser(html);
    int row_index = -1 == parser->GetLastIndex() ? -1 : parser->GetIndexBeforeMetaDate(date_meta);
    
    std::string meta = "p/" + date_meta;

    auto *report_date_time = new ReportDateTime();
    std::string weekday = report_date_time->GetWeekdayByMeta(meta);

    std::string time_start = config.GetConfigValue("default_daily_start_time");

    int time_start_minutes = helper::DateTime::GetSumMinutesFromTime(time_start);
    std::string required_per_day = config.GetConfigValue("debit_per_day");
    int required_per_day_minutes = helper::DateTime::GetSumMinutesFromTime(required_per_day);
    int time_end_minutes = time_start_minutes + required_per_day_minutes;
    std::string time_end = helper::DateTime::GetHoursFormattedFromMinutes(time_end_minutes);

    helper::String::Trim(comment);

    return ReportCrud::InsertEntryAfter(html, row_index, meta, week_number, weekday, date_day, time_start, time_end,
            std::move(task_number), comment);
  }

  /**
   * Add timesheet entry: stop work
   */
  bool ReportCrud::StopEntry(const char *comment) {
    return UpsertEntry(EntryStatus::Status_Stopped, comment);
  }

  /**
   * Append given text to comment of given or latest entry
   */
  bool ReportCrud::AppendComment(std::string &comment, int row_index, bool start_with_space) {
    if (comment.empty()) return false;

    ReportParser *parser = new ReportParser();
    if (!parser->LoadReportHtml()) return false;

    std::string comment_lhs = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Comment);
    std::string append = std::string(start_with_space ? " " : "").append(comment);

    return SaveReport(parser->AppendToColumn(row_index, Report::ColumnIndexes::Index_Comment, append));
  }

  /**
   * Set task number of latest or given entry
   */
  bool ReportCrud::UpdateTaskNumber(int task_number, int row_index) {
    std::string html = GetReportHtml();
    if (html.empty()) return false;

    std::string task_number_str = task_number > 0 ? helper::Numeric::ToString(task_number) : "";
    ReportParser::UpdateColumn(html, row_index, Report::ColumnIndexes::Index_Task, task_number_str);

    return SaveReport(html);
  }

  /**
   * Merge (time, task, comment of) given entry w/ next one (and remove next one)
   */
  bool ReportCrud::Merge(int row_index) {
    ReportParser *parser = new ReportParser();
    if (!parser->LoadReportHtml()) return false;

    std::string date = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Date);
    std::string date_next = parser->GetColumnContent(row_index + 1, Report::ColumnIndexes::Index_End);
    if (0 != std::strcmp(date.c_str(), date_next.c_str())) {
      tictac_track::AppError::PrintError("Cannot merge: Next entry is in different day");
      return false;
    }
      
    int minutes_gap = parser->GetMinutesBetweenEntryAndNext(row_index);
    if (!IsMergeableAmountMinutes(minutes_gap)) return false;

    std::string html = parser->GetHtml();
    int offset_tr      = parser->GetOffsetTrOpenByIndex(html, row_index);
    int offset_tr_next = parser->GetOffsetTrOpenByIndex(html, row_index + 1);
    
    std::string time_end_second = parser->GetColumnContent(row_index + 1, Report::ColumnIndexes::Index_End, offset_tr_next);
    std::string comment = parser->GetCommentMergedWithNextByRowIndex(row_index, offset_tr, offset_tr_next);
    
    std::string task = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Task, offset_tr);
    if (task.empty()) task = parser->GetColumnContent(row_index + 1, Report::ColumnIndexes::Index_Task, offset_tr_next);

    // Merge meta: set merged entry to status of 2nd entry before merge
    std::string meta_second = parser->GetColumnContent(row_index + 1, Report::ColumnIndexes::Index_Meta, offset_tr_next);
    if ('s' == meta_second[0]) {
      std::string meta_first = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Meta, offset_tr);
      meta_first[0] = 's';
      ReportParser::UpdateColumn(html, row_index, Report::ColumnIndexes::Index_Meta, meta_first);  
    }
    
    ReportParser::UpdateColumn(html, row_index, Report::ColumnIndexes::Index_End, time_end_second);
    ReportParser::UpdateColumn(html, row_index, Report::ColumnIndexes::Index_Comment, comment);
    ReportParser::UpdateColumn(html, row_index, Report::ColumnIndexes::Index_Task, task);

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

  /**
   * Remove given amount of latest entries (but not more than exist)
   */
  bool ReportCrud::RemoveEntries(int amount) {
    ReportParser *parser = new ReportParser();
    if (!parser->LoadReportHtml() || -1 == parser->GetLastIndex()) return false;

    if (amount - 1 >= parser->GetLastIndex()) return Reset();

    std::string html = parser->GetHtml();
    for (int i = 0; i < amount; i++) {
      std::size_t offset_last_tr = html.rfind("\n<tr");
      if (std::string::npos == offset_last_tr) continue;

      std::size_t offset_last_tr_end = html.rfind("</tr>");
      if (std::string::npos != offset_last_tr_end) html = html.erase(offset_last_tr, offset_last_tr_end - offset_last_tr + 6);
    }

    return SaveReport(html) && ReportRecalculator::RecalculateAndUpdate();
  }

  /**
   * Remove 1 entry at given index
   */
  bool ReportCrud::RemoveEntryById(std::string &html, int id) {
    if (0 > id) return false;

    ReportParser *parser = new ReportParser(html);
    int last_index = parser->GetLastIndex();
    if (id > last_index)
      return tictac_track::AppError::PrintError(std::string("Cannot remove entry ")
            .append(helper::Numeric::ToString(id)).append(", last index is ")
            .append(helper::Numeric::ToString(last_index)).c_str());

    int offset_tr_open = ReportParser::GetOffsetTrOpenByIndex(html, id);
    if (-1 == offset_tr_open) return false;

    size_t offset_tr_close = html.find("</tr>", static_cast<unsigned long> (offset_tr_open));
    if (std::string::npos == offset_tr_close) return false;

    html = html.erase(static_cast<unsigned long> (offset_tr_open), offset_tr_close - offset_tr_open + 6);

    return SaveReport(html) && ReportRecalculator::RecalculateAndUpdate();
  }

  bool ReportCrud::RemoveEntryById(int id) {
    std::string html = GetReportHtml();

    return !html.empty() && RemoveEntryById(html, id);
  }

  bool ReportCrud::Reset() {
    return InitReportFile(true);
  }

  bool ReportCrud::IsAnyEntryRunning() {
    std::string html = GetReportHtml();

    return !html.empty() && ReportParser::IsAnyEntryRunning(html);
  }

  bool ReportCrud::CurrentDayHasTasks() {
    std::string html = GetReportHtml();
    std::string date_cell = "<td>" + report_date_time_->GetDateFormatted() + "</td>";

    return std::string::npos != html.find(date_cell);
  }
} // namespace tictac_lib
