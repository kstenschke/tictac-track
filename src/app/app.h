
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_APP
#define CLASS_TIMETRACKERCL_APP

#include "app_commands.h"
#include "app_arguments.h"
#include "report/report_parser.h"
#include "../report/report.h"

namespace tictac_track {

  class App
  {
    public:
      // Meta constants
      static const std::string kAppName;
      static const std::string kAppExecutableName;
      static const std::string kAppVersion;

      // Constructor: init (resolve) command and arguments
      App(int argc,  char** argv);

      // Process command + arguments
      bool Process();

    private:
      AppCommand *command_;
      AppArguments *arguments_;

      bool AddFullDayEntry();

      bool BrowseTaskUrl();

      bool DisplayDate();

      bool ExportCsv();

      bool Help();

      // Merge given task with following one
      bool Merge();

      bool Recalculate();

      // Resume a previous entry
      bool Resume();
      // Resume entry, if given index is positive: is row index, else: negative offset
      bool ResumeEntryByIndexOrNegativeOffset(int row_index, std::string add_to_comment = "");

      // Remove entries
      bool Remove();

      // Split entry into two
      bool Split();
      // Split given entry at given duration before its end into two entries
      bool SplitAtEnd(ReportParser *parser, std::string split_duration, int row_index);

      // Add start-entry
      bool Start();

      // Add end-entry
      bool Stop();

      // Add/Append/Unset comment of latest or entry with given ID
      bool UpdateComment();
      bool UpdateCommentByEntryId(int last_index, int index, std::string comment, bool starts_with_space = false);

      // Update time of row + column by arguments
      bool UpdateTime(Report::ColumnIndexes column_index);

      // Set task number of latest or given entry
      bool UpdateTaskNumber();

      // Pretty-print report to CLI
      bool View();
      bool ViewWeek();

      // View entries sequentially, opening related task URL at the same time
      bool BrowseDayTasks();

      int GetCommentArgOffsetInTaskCommand() const;
  };
} // namespace tictac_lib

#endif