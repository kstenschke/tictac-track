/*
  Copyright (c) 2018-2020, Kay Stenschke
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

#ifndef TTT_CLASS_APP_APP_H_
#define TTT_CLASS_APP_APP_H_

#include <ttt/class/app/app_arguments.h>
#include <ttt/class/app/app_commands.h>
#include <ttt/class/report/report_parser.h>
#include <ttt/class/report/report.h>

#include <string>
#include <iostream>
#include <cstring>
#include <utility>
#include <vector>

namespace tictac_track {

class App {
 public:
  // Meta constants
  static const std::string kAppName;
  static const std::string kAppExecutableName;

  // Constructor: init (resolve) command and arguments
  App(int argc, char **argv);

  // Process command + arguments
  bool Process();

  // Set issue number of latest or given entry
  bool UpdateTaskNumber();

 private:
  AppCommand *command_;
  AppArguments *arguments_;

  static bool ClearTimesheet();

  bool AddFullDayEntry();

  bool BrowseTaskUrl();

  bool DisplayCalendarWeek();

  bool DisplayDate();

  bool ExportCsv();

  bool Help();

  // Merge given task with following one
  bool Merge();

  static bool Recalculate();

  // Resume a previous entry
  bool Resume();

  // Resume entry,
  // if given index is positive: is row index,
  // else: negative offset
  static bool ResumeEntryByIndexOrNegativeOffset(
      int row_index,
      std::string add_to_comment = "");

  // Remove entries
  bool Remove();

  // Split entry into two
  bool Split();

  // Split given entry at given duration before its end into two entries
  static bool SplitAtEnd(
      ReportParser *parser,
      std::string split_duration,
      int row_index);

  // Add start-entry
  bool Start();

  // Add end-entry
  bool Stop();

  // Add/Append/Unset comment of latest or entry with given ID
  bool UpdateComment();
  static bool UpdateCommentByEntryId(
      int last_index,
      int index,
      std::string comment,
      bool starts_with_space = false);

  // Update time of row + column by arguments
  bool UpdateTime(Report::ColumnIndexes column_index);

  // Pretty-print report to CLI
  bool View();
  bool ViewWeek();

  // Output given amount of recent tasks (having a task-number) as CSV
  static bool CsvRecentTaskNumbers();

  // Output tracks of current day as CSV
  bool CsvTodayTracks();

  // View entries sequentially, opening related task URL at the same time
  bool BrowseDayTasks();

  int GetCommentArgOffsetInTaskCommand() const;
};

}  // namespace tictac_track

#endif  // TTT_CLASS_APP_APP_H_
