
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_REPORT_HTML_ANALYZER
#define CLASS_TIMETRACKERCL_REPORT_HTML_ANALYZER

#include <vector>
#include "report.h"
#include "../app/commands.h"

namespace tictac_track {
  class ReportHtmlParser : public Report {
    public:
      // Constructor
      explicit ReportHtmlParser(std::string html = "");

      bool LoadReportHtml();
      std::string GetHtml();
      void SetHtml(std::string html);

      // Get initial timesheet html
      static std::string GetInitialReportHtml();

      // Update table head (using labels in currently configured language)
      std::string UpdateTableHeader();
      // Update report title (in title- and h1-tag) to: "timesheet <DATE_FIRST_ENTRY> - <DATE_LAST_ENTRY>"
      std::string UpdateTitle();
      // Get title for timesheet HTML headline: "timesheet <DATE_FIRST_ENTRY> - <DATE_LAST_ENTRY>"
      std::string GetReportTitle();

      int GetLastIndex();

      // Get ID of latest entry w/ given task number, or -1
      int GetLatestIndexByTaskNumber(std::string task_number);

      // Find row w/ date immediately before given one
      int GetIndexBeforeMetaDate(std::string meta_date);

      std::string GetDateLatestEntry();

      /**
       * Find offset (of days) of first existing entry backwards from current date, before given offset
       * Ex: when "v d -1" does not find any entries, but there is an entry 3 days before current date, it returns: -3
       */
      int GetExistingEntryOffsetBefore(int offset_start);

      std::vector<std::string> GetTasksOfDay(std::string &date);

      static int GetOffsetTrOpenByIndex(std::string &html, int index = -1);

      unsigned long GetOffsetTdStartInRunningEntry();

      int GetMinutesBetweenEntryAndNext(int row_index);

      // Get offset of given needle (e.g. "<td>" or "</td>") of given column, after given initial offset
      size_t GetColumnOffset(const char* needle, unsigned long offset_initial, ColumnIndexes column_index);

      std::string GetColumnContent(int row_index, ColumnIndexes index_column);

      bool IsDateOfLatestEntry(std::string &date_compare);

      // Replace/insert content of given column of given row, return changed html
      static void UpdateColumn(std::string &html, int row_index, Report::ColumnIndexes column_index,
                               std::string content);

      // Load report HTML, replace given column content, save changed report, return bool: succeeded?
      static bool UpdateColumn(int row_index, Report::ColumnIndexes column_index, std::string content);

      // Append given content to content of column in row
      std::string AppendToColumn(int row_index, Report::ColumnIndexes column_index, std::string content);

      // Reduce given time-column by given duration
      bool ReduceEntryTime(int row_index, std::string subtrahend_hhmm,
                           AppCommand::Commands command = AppCommand::Commands::Command_Invalid);

      static bool IsAnyEntryRunning(std::string &html);
      bool IsAnyEntryRunning();

      bool IsEntryRunning(int row_index);

      bool HtmlContains(std::string &str);

      static std::string GetTHead();

      // Merge comments from given row and following
      std::string MergeCommentByRowIndexWithNext(int row_index);

    protected:
      std::string html_;

    private:
      int GetIndexFirstEntryOfDate(std::string &date);
  };
} // namespace tictac_track

#endif
