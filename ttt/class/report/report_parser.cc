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

#include <ttt/class/report/report_parser.h>
#include <ttt/class/app/app_config.h>

namespace tictac_track {

ReportParser::ReportParser(std::string html): html_(std::move(html)) {
}

bool ReportParser::LoadReportHtml() {
  html_ = ReportFile::GetReportHtml();

  if (html_.empty()) {
    return false;
  }

  html_ = helper::String::ReplaceAll(html_.c_str(), "\n\n", "\n");

  return true;
}

std::string ReportParser::GetHtml() {
  return html_;
}

void ReportParser::SetHtml(std::string html) {
  html_ = std::move(html);
}

// Get initial timesheet html
std::string ReportParser::GetInitialReportHtml() {
  AppLocale locale = AppLocale::GetInstance();
  AppConfig config = AppConfig::GetInstance();

  std::string title = locale.Translate("timesheet");

  std::stringstream content_stream;

  content_stream
    << "<!DOCTYPE html><html>\n"
    << "<head>\n"
    << "<title>" << title << "</title>\n"
    << GetStyle()
    << "</head>\n"
    << "<body>\n"
    << "<h1>" << title << "</h1>\n"
    << "<table border=\"1\">\n"
    << GetTHead()
    << "</table>\n"
    << "</body>\n"
    << "</html>";

  return content_stream.str();
}

std::string ReportParser::GetStyle() {
  std::stringstream style_stream;

  if ("1" == AppConfig::GetConfigValueStatic("dark_theme_html")) {
    style_stream
        << "<style>\n"
        << "html,body { background-color: #222; color: #ddd; }\n"
        << "h1, table { font-family:sans-serif; font-size:13px;}\n"
        << "table { border-collapse:collapse; border:1px solid #ddd; }\n"
        << "th, td { border:1px solid #ddd; padding:2px 4px; }\n"
        << ".meta { display:none; }\n"
        << ".new-day td { border-top:3px solid #ddd; }\n"
        << "</style>\n";
  } else {
    style_stream
        << "<style>\n"
        << "h1, table { font-family:sans-serif; font-size:13px;}\n"
        << "table { border-collapse:collapse; border:1px solid #ccc; }\n"
        << "th, td { border:1px solid #ccc; padding:2px 4px; }\n"
        << ".meta { display:none; }\n"
        << ".new-day td { border-top:3px solid #ccc; }\n"
        << "</style>\n";
  }

  return style_stream.str();
}

std::string ReportParser::GetTHead() {
  AppConfig config = AppConfig::GetInstance();
  AppLocale locale = AppLocale::GetInstance();
  locale.locale_key_ = config.GetConfigValue("locale").c_str();

  std::stringstream content_stream;

  content_stream
    << "<thead><tr>"
    << "<th class=\"meta\">Meta</th>"
    << "<th>" << locale.Translate("week", true) << "</th>"
    << "<th>" << locale.Translate("day", true) << "</th>"
    << "<th>" << locale.Translate("date", true) << "</th>"
    << "<th>" << locale.Translate("start", true) << "</th>"
    << "<th>" << locale.Translate("end", true) << "</th>"
    << "<th>" << locale.Translate("task", true) << "</th>"
    << "<th>" << locale.Translate("comment", true) << "</th>"
    << "<th>" << locale.Translate("duration", true) << "</th>"
    << "<th>" << locale.Translate("task", true) << "/"
    << locale.Translate("day", true) << " &Sigma;</th>"
    << "<th>" << locale.Translate("day", true) << " &Sigma;</th>"
    << "<th>" << locale.Translate("balance", true) << "</th>"
    << "</tr></thead>\n";

  return content_stream.str();
}

// Update table head (using labels in currently configured language)
std::string ReportParser::UpdateTableHeader() {
  size_t offset_start_thead = html_.find("<thead>");
  size_t offset_end_thead = html_.find("</thead>");

  if (std::string::npos == offset_start_thead
    || std::string::npos == offset_end_thead) return html_;

  std::string thead = GetTHead();

  html_ = html_.replace(
      offset_start_thead,
      offset_end_thead - offset_start_thead + 8,
      thead);

  return html_;
}

// Get title for timesheet HTML headline:
// "timesheet <DATE_FIRST_ENTRY> - <DATE_LAST_ENTRY>"
std::string ReportParser::GetReportTitle() {
  if (-1 == GetLastIndex()) {
    return {""};
  }

  std::string date_first = GetColumnContent(0, ColumnIndexes::Index_Date);
  std::string date_last = GetDateLatestEntry();

  AppLocale locale = AppLocale::GetInstance();
  std::string title = locale.Translate("timesheet");
  title = title.append(" ").append(date_first);

  return date_first != date_last
         ? title.append(" - ").append(date_last)
         : title;
}

std::string ReportParser::GetDateLatestEntry() {
  int index_last = GetLastIndex();

  return GetColumnContent(index_last, ColumnIndexes::Index_Date);
}

// Decrement offset (of days) until finding an entry of that date,
// going backwards from current date - given days-offset
// Ex: when "v d -1" does not find any entries,
// but there is an entry 3 days before current date, it returns: -3
int ReportParser::GetExistingEntryOffsetBefore(int offset_start) {
  int offset = offset_start;

  auto *report_date_time = new ReportDateTime();

  // TODO(kay): detect offset before first entry:
  //  return 0 immediately (would otherwise loop endlessly)
  std::string first_date = GetColumnContent(1, ColumnIndexes::Index_Date);

  while (true) {
    std::string date_formatted = report_date_time->GetDateFormatted(offset);

    if (HtmlContains(date_formatted)) {
      delete report_date_time;

      return offset;
    }

    if (date_formatted == first_date) {
      delete report_date_time;

      return 0;
    }

    --offset;
  }

  delete report_date_time;
}

// Update report title (in title- and h1-tag) to:
// "timesheet <DATE_FIRST_ENTRY> - <DATE_LAST_ENTRY>"
std::string ReportParser::UpdateTitle() {
  if (-1 == GetLastIndex()) return html_;

  std::string title = GetReportTitle();

  auto offset_title_start = html_.find("<title>") + 7;
  auto offset_title_end = html_.find("</title>");

  html_ = html_.replace(
      offset_title_start,
      offset_title_end - offset_title_start,
      title);

  auto offset_h1_start = html_.find("<h1>") + 4;
  auto offset_h1_end = html_.find("</h1>");

  html_ = html_.replace(
      offset_h1_start,
      offset_h1_end - offset_h1_start,
      title);

  return html_;
}

int ReportParser::GetAmountRows() {
  return helper::String::GetSubStrCount(html_.c_str(), "<tr") - 1;
}

int ReportParser::GetLastIndex() {
  if (-1 == last_index_)
    // Do not count header. Subtract one more, as index is zero-based
    last_index_ = helper::String::GetSubStrCount(html_.c_str(), "<tr") - 2;

  return last_index_;
}

// Get ID of latest entry w/ given task number, or -1
int ReportParser::GetLatestIndexByTaskNumber(const std::string& task_number) {
  if (!HtmlContains(task_number)) {
    return -1;
  }

  int last_index = GetLastIndex();

  for (int index = last_index; index >= 0; index--) {
    const char *task = task_number.c_str();

    if (0 == std::strcmp(
        task,
        GetColumnContent(index, ColumnIndexes::Index_Issue).c_str())) {
      return index;
    }

  }

  return -1;
}

std::string ReportParser::GetLatestIssueNumber(int offset) {
  int row_index = GetLastIndex() - offset;

  return GetColumnContent(row_index, ColumnIndexes::Index_Issue);
}

std::string ReportParser::GetLatestComment(int offset) {
  int row_index = GetLastIndex() - offset;

  return GetColumnContent(row_index, ColumnIndexes::Index_Comment);
}

int ReportParser::GetIndexFirstEntryOfDate(const std::string &date) {
  int last_index = GetLastIndex();

  for (int i = 0; i <= last_index; i++) {
    if (date == GetColumnContent(i, ColumnIndexes::Index_Date)) {
      return i;
    }
  }

  return -1;
}

// Find row w/ date immediately before given one
int ReportParser::GetIndexBeforeMetaDate(const std::string& meta_date) {
  auto *parser = new ReportParser();

  if (!parser->LoadReportHtml()) {
    delete parser;

    return false;
  }

  int last_index = parser->GetLastIndex();

  std::string meta_date_in_row;

  for (int i = 0; i <= last_index; i++) {
    // Check if meta_date_in_row is after meta_date
    meta_date_in_row =
        parser->GetColumnContent(
            i,
            Report::ColumnIndexes::Index_Meta).substr(2, std::string::npos);

    if (ReportDateTime::IsMetaDateAfter(meta_date_in_row, meta_date)) {
      delete parser;

      return i - 1;
    }
  }

  delete parser;

  return last_index;
}

// Check whether given timesheet HTML contains
// any in-progress (= w/o end time) entry
bool ReportParser::IsAnyEntryOngoing(std::string &html) {
  return helper::String::Contains(html, "<td class=\"meta\">s/");
}

bool ReportParser::IsAnyEntryOngoing() {
  return helper::String::Contains(html_, "<td class=\"meta\">s/");
}

bool ReportParser::IsEntryOngoing(int row_index) {
  std::string row_meta = GetColumnContent(row_index, ColumnIndexes::Index_Meta);

  return 's' == row_meta[0];
}

bool ReportParser::OngoingEntryContainsIssueNumber() {
  std::string issue_number = GetLatestIssueNumber();

  return helper::String::IsNumeric(issue_number);
}

bool ReportParser::OngoingEntryContainsComment() {
  std::string comment = GetLatestComment();

  return !comment.empty();
}

bool ReportParser::HtmlContains(const std::string &str) {
  return helper::String::Contains(html_, str.c_str());
}

int ReportParser::GetOffsetTrOpenByIndex(
    const std::string &html,
    int index_row) {
  size_t offset_tr;

  if (-1 == index_row) {
    // Get last row entry
    offset_tr = html.rfind("<tr");

    if (std::string::npos != offset_tr) {
      return static_cast<int> (offset_tr);
    }

    std::cout << "Error - Failed finding row.\n";

    return -1;
  }

  // Get entry at given row-index
  offset_tr = static_cast<size_t> (0);

  for (int i = 0; i <= index_row + 1; i++) {
    offset_tr = html.find("<tr", offset_tr + 1);

    if (std::string::npos == offset_tr) {
      return -1;
    }
  }

  return static_cast<int> (offset_tr);
}

// Get offset of <td> tag of start-time column
// of currently ongoing entry in given html
uint32_t ReportParser::GetOffsetTdStartInOngoingEntry() {
  auto offsetTdMeta = html_.find("<td class=\"meta\">s/");
  auto offsetTdWeek = html_.find("<td>", offsetTdMeta + 1);
  auto offsetTdDay = html_.find("<td>", offsetTdWeek + 1);
  auto offsetTdDate = html_.find("<td>", offsetTdDay + 1);

  return html_.find("<td>", offsetTdDate + 1);
}

int ReportParser::GetMinutesBetweenEntryAndNext(int row_index) {
  std::string time_end_first =
      GetColumnContent(row_index, Report::ColumnIndexes::Index_End);

  std::string time_start_second =
      GetColumnContent(row_index + 1, Report::ColumnIndexes::Index_Start);

  int minutes_end_first =
      helper::DateTime::GetSumMinutesFromTime(time_end_first);

  int minutes_start_second =
      helper::DateTime::GetSumMinutesFromTime(time_start_second);

  return minutes_start_second - minutes_end_first;
}

// Get offset of given needle (e.g. "<td>" or "</td>")
// of given column, after given initial offset
size_t ReportParser::GetColumnOffset(
    const char *needle,
    uint32_t offset_initial,
    Report::ColumnIndexes column_index) {
  auto column_index_int = static_cast<int> (column_index);

  size_t offset = offset_initial;

  for (int i = 0; i <= column_index_int; i++) {
    offset = html_.find(needle, offset + 1);

    if (std::string::npos == offset) {
      return std::string::npos;
    }
  }

  return offset;
}

std::string ReportParser::GetColumnContent(
    int row_index,
    ColumnIndexes index_column,
    int offset_tr) {
  if (-1 == offset_tr) {
    offset_tr = GetOffsetTrOpenByIndex(html_, row_index);
  }

  if (-1 == offset_tr) {
    return "";
  }

  auto offset_td_start =
      GetColumnOffset(
          "<td",
          static_cast<uint32_t> (offset_tr),
          index_column);

  auto offset_content_start =
      static_cast<int> (html_.find('>', offset_td_start) + 1);

  auto offset_content_end =
      static_cast<int> (html_.find("</td>", offset_content_start));

  return html_.substr(
      offset_content_start,
      offset_content_end - offset_content_start);
}

std::vector<std::string> ReportParser::GetIssueNumbersOfDay(
    const std::string &date) {
  int index_row_day_start = GetIndexFirstEntryOfDate(date);

  if (-1 == index_row_day_start) {
    return {};
  }

  std::vector<std::string> tasks;

  int last_index = GetLastIndex();

  std::string task;

  for (int i = index_row_day_start; i <= last_index; i++) {
    if (date != GetColumnContent(i, ColumnIndexes::Index_Date)) {
      return tasks;
    }

    task = GetColumnContent(i, ColumnIndexes::Index_Issue);

    if (task.empty()
        || (tasks.end() != find(tasks.begin(), tasks.end(), task))) {
      continue;
    }

    tasks.push_back(task);
  }

  return tasks;
}

bool ReportParser::IsDateOfLatestEntry(const std::string &date_compare) {
  int row_index_last = GetLastIndex();

  std::string date_last =
      GetColumnContent(row_index_last, ColumnIndexes::Index_Date);

  return date_last == date_compare;
}

// Replace/insert content of given column of given row,
// return changed report HTML
bool ReportParser::UpdateColumn(std::string &html, int row_index,
                                Report::ColumnIndexes column_index,
                                const std::string& content) {
  auto *parser = new ReportParser(html);

  int last_index = parser->GetLastIndex();

  if (row_index > last_index) {
    delete parser;

    return
        tictac_track::AppError::PrintError(
            std::string("Cannot update entry ")
                .append(helper::Numeric::ToString(row_index))
                .append(", last entry is ")
                .append(helper::Numeric::ToString(last_index))
                .append(".")
                .c_str());
  }

  int offset_tr = GetOffsetTrOpenByIndex(html, row_index);

  if (-1 == offset_tr) {
    delete parser;

    return tictac_track::AppError::PrintError(
        std::string("Cannot update entry: Failed finding row ")
            .append(helper::Numeric::ToString(row_index)).c_str());
  }

  auto offset_td_content_start = parser->GetColumnOffset(
      "<td",
      static_cast<uint32_t> (offset_tr),
      column_index) + 4;

  delete parser;

  // Meta-column td contains also class name
  if (column_index == ColumnIndexes::Index_Meta) {
    offset_td_content_start += 13;
  }

  size_t offset_td_content_end = html.find("</td>", offset_td_content_start);

  if (std::string::npos == offset_td_content_start
      || std::string::npos == offset_td_content_end) {
    return false;
  }

  html = html.replace(
      offset_td_content_start,
      offset_td_content_end - offset_td_content_start,
      content);

  return true;
}

// Load report HTML,
// replace given column content, save changed report,
// return bool: succeeded?
bool ReportParser::UpdateColumn(
    int row_index,
    Report::ColumnIndexes column_index,
    const std::string& content) {
  std::string html = ReportFile::GetReportHtml();

  if (html.empty()) {
    return false;
  }

  auto *parser = new ReportParser(html);

  int last_index = parser->GetLastIndex();

  if (row_index > last_index) {
    delete parser;

    return tictac_track::AppError::PrintError(
        std::string("Cannot update entry ")
            .append(helper::Numeric::ToString(row_index))
            .append(", last entry is ")
            .append(helper::Numeric::ToString(last_index))
            .append(".")
            .c_str());
  }

  delete parser;

  UpdateColumn(html, row_index, column_index, content);

  return ReportCrud::SaveReport(html);
}

// Append given content to content of column in row
std::string ReportParser::AppendToColumn(
    int row_index,
    Report::ColumnIndexes column_index,
    const std::string& content) {
  int offset_tr = GetOffsetTrOpenByIndex(html_, row_index);

  if (-1 == offset_tr) {
    return html_;
  }

  size_t offset_td_content_end = GetColumnOffset(
      "</td>",
      static_cast<u_int32_t> (offset_tr),
      column_index);

  return std::string::npos == offset_td_content_end
         ? html_
         : html_.replace(offset_td_content_end, 0, content);
}

// Reduce given time-column by given duration
bool ReportParser::ReduceEntryTime(
    int row_index,
    std::string subtrahend_hhmm,
    AppCommand::Commands command) {
  int offset_tr = GetOffsetTrOpenByIndex(html_, row_index);

  int minutes_end = helper::DateTime::GetSumMinutesFromTime(
      GetColumnContent(row_index, Report::ColumnIndexes::Index_End, offset_tr));

  int minutes_start = helper::DateTime::GetSumMinutesFromTime(
      GetColumnContent(
          row_index,
          Report::ColumnIndexes::Index_Start,
          offset_tr));

  int duration = minutes_end - minutes_start;

  int minutes_subtrahend =
      helper::DateTime::GetSumMinutesFromTime(std::move(subtrahend_hhmm));

  if (duration < minutes_subtrahend) {
    std::string message = std::string("Entry duration is ")
        .append(helper::DateTime::GetHoursFormattedFromMinutes(duration))
        .append(", cannot subtract ")
        .append(
            helper::DateTime::GetHoursFormattedFromMinutes(
                minutes_subtrahend).append("."));

    if (AppCommand::Commands::Command_Split == command)
      message = std::string("Cannot split entry: ").append(message);

    return tictac_track::AppError::PrintError(message.c_str());
  }

  std::string content =
      helper::DateTime::GetHoursFormattedFromMinutes(
          minutes_end - minutes_subtrahend);

  UpdateColumn(html_, row_index, Report::ColumnIndexes::Index_End, content);

  return true;
}

// Merge comments from given row and following
std::string ReportParser::GetCommentMergedWithNextByRowIndex(
    int row_index,
    int offset_tr,
    int offset_tr_next) {
  std::string comment_1 =
      GetColumnContent(
          row_index,
          Report::ColumnIndexes::Index_Comment,
          offset_tr);

  std::string comment_2 =
      GetColumnContent(
          row_index + 1,
          Report::ColumnIndexes::Index_Comment,
          offset_tr_next);

  helper::String::Trim(comment_1);
  helper::String::Trim(comment_2);

  return MergeComments(comment_1, comment_2);
}

std::string ReportParser::MergeComments(
    std::string &comment_1,
    std::string &comment_2) {
  if (comment_1.empty() || comment_1 == comment_2) {
    return comment_2;
  }

  if (comment_2.empty()) {
    return comment_1;
  }

  std::string merged =
      helper::String::EndsWith(comment_1, ".")
      || helper::String::EndsWith(comment_1, ",")
      || helper::String::StartsWith(comment_2.c_str(), ".")
      || helper::String::StartsWith(comment_2.c_str(), ",")
        ? comment_1.append(" ").append(comment_2)
        : comment_1.append(". ").append(comment_2);

  return helper::String::ReplaceAll(merged, " , ", ", ");
}

}  // namespace tictac_track
