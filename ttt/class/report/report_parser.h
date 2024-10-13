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

#ifndef TTT_CLASS_REPORT_REPORT_PARSER_H_
#define TTT_CLASS_REPORT_REPORT_PARSER_H_

#include <ttt/class/app/app_commands.h>
#include <ttt/class/app/app_error.h>
#include <ttt/class/report/report.h>
#include <ttt/class/report/report_crud.h>
#include <ttt/class/report/report_file.h>

#include <ttt/helper/helper_string.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace tictac_track {

class ReportParser : public Report {
 public:
  explicit ReportParser(std::string html = "");

  bool LoadReportHtml();
  std::string GetHtml();
  void SetHtml(std::string html);

  // Get initial timesheet html
  static std::string GetInitialReportHtml();

  // Update table head (using labels in currently configured language)
  std::string UpdateTableHeader();

  // Update report title (in title- and h1-tag)
  // to: "timesheet <DATE_FIRST_ENTRY> - <DATE_LAST_ENTRY>"
  std::string UpdateTitle();

  // Get title for timesheet HTML headline:
  // "timesheet <DATE_FIRST_ENTRY> - <DATE_LAST_ENTRY>"
  std::string GetReportTitle();

  int GetAmountRows();

  int GetLastIndex();

  // Get ID of latest entry w/ given task number, or -1
  int GetLatestIndexByTaskNumber(const std::string& task_number);

  // Get issue number of (latest entry - offset)
  std::string GetLatestIssueNumber(int offset = 0);

  // Get comment of (latest entry - offset)
  std::string GetLatestComment(int offset = 0);

  // Find row w/ date immediately before given one
  static int GetIndexBeforeMetaDate(const std::string& meta_date);

  std::string GetDateLatestEntry();

  // Find offset (of days) of first existing entry backwards from current date,
  // before given offset
  // Ex: when "v d -1" does not find any entries,
  // but there is an entry 3 days before current date, it returns: -3
  int GetExistingEntryOffsetBefore(int offset_start);

  std::vector<std::string> GetIssueNumbersOfDay(const std::string &date);

  static int GetOffsetTrOpenByIndex(const std::string &html, int index = -1);

  uint32_t GetOffsetTdStartInOngoingEntry();

  int GetMinutesBetweenEntryAndNext(int row_index);

  // Get offset of given needle (e.g. "<td>" or "</td>") of given column,
  // after given initial offset
  size_t GetColumnOffset(
      const char *needle,
      uint32_t offset_initial,
      ColumnIndexes column_index);

  std::string GetColumnContent(
      int row_index,
      ColumnIndexes index_column,
      int offset_tr = -1);

  bool IsDateOfLatestEntry(const std::string &date_compare);

  // Replace/insert content of given column of given row, return changed html
  static bool UpdateColumn(
      std::string &html,
      int row_index,
      Report::ColumnIndexes column_index,
      const std::string& content);

  // Load report HTML,
  // replace given column content,
  // save changed report,
  // return bool: succeeded?
  static bool UpdateColumn(
      int row_index,
      Report::ColumnIndexes column_index,
      const std::string& content);

  // Append given content to content of column in row
  std::string AppendToColumn(
      int row_index,
      Report::ColumnIndexes column_index,
      const std::string& content);

  // Reduce given time-column by given duration
  bool ReduceEntryTime(
      int row_index,
      std::string subtrahend_hhmm,
      AppCommand::Commands command = AppCommand::Commands::Command_Invalid);

  static bool IsAnyEntryOngoing(std::string &html);
  bool IsAnyEntryOngoing();
  bool IsEntryOngoing(int row_index);

  bool OngoingEntryContainsIssueNumber();
  bool OngoingEntryContainsComment();

  bool HtmlContains(const std::string &str);

  static std::string MergeComments(
      std::string &comment_1,
      std::string &comment_2);

  static std::string GetStyle();

  static std::string GetTHead();

  // Merge comments from given row and following
  std::string GetCommentMergedWithNextByRowIndex(
      int row_index,
      int offset_tr,
      int offset_tr_next);

 protected:
  std::string html_;
  int last_index_ = -1;

 private:
  int GetIndexFirstEntryOfDate(const std::string &date);
};

}  // namespace tictac_track

#endif  // TTT_CLASS_REPORT_REPORT_PARSER_H_
