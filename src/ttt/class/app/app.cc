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
#include <iostream>
#include <cstring>
#include <ttt/helper/helper_tui.h>

#include "app.h"
#include "app_config.h"
#include "app_help.h"
#include "../report/report_browser.h"
#include "../report/report_file.h"
#include "../report/report_crud.h"
#include "../report/report_renderer_csv.h"
#include "../report/report_renderer_cli.h"
#include "../app/app_error.h"
#include "../report/report_recalculator.h"
#include "../../helper/helper_numeric.h"
#include "../../helper/helper_string.h"
#include "../../helper/helper_date_time.h"
#include "../../helper/helper_system.h"

namespace tictac_track {

// Meta constants
const std::string App::kAppName = "tictac-track";
const std::string App::kAppExecutableName = "ttt";

/**
 * Constructor: init (resolve) command and arguments
 */
App::App(int argc, char **argv) {
  if (argc==1) {
    // No command given
    AppHelp::PrintHelp(true, AppCommand::Commands::Command_Invalid);
    return;
  }

  command_ = new AppCommand(argc > 0 ? argv[1] : "");
  arguments_ = new AppArguments(argc, argv, *command_);
}

/**
 * Process command + arguments
 */
bool App::Process() {
  bool keep_backup;

  switch (command_->GetResolved()) {
    case AppCommand::Command_ClearTimesheet:
      ReportFile::BackupReportTemporary();
      ClearTimesheet();
      break;
    case AppCommand::Command_BrowseTimesheet:return ReportBrowser::BrowseTimesheet();
    case AppCommand::Command_BrowseTaskUrl:return BrowseTaskUrl();
    case AppCommand::Command_Comment:ReportFile::BackupReportTemporary();
      keep_backup = UpdateComment();
      break;
    case AppCommand::Command_Csv:return ExportCsv();
    case AppCommand::Command_Date:return DisplayDate();
    case AppCommand::Command_Day:ReportFile::BackupReportTemporary();
      keep_backup = AddFullDayEntry();
      break;
    case AppCommand::Command_Help:return Help();
    case AppCommand::Command_Merge:ReportFile::BackupReportTemporary();
      keep_backup = Merge();
      break;
    case AppCommand::Command_Recalculate:ReportFile::BackupReportTemporary();
      keep_backup = Recalculate();
      break;
    case AppCommand::Command_Resume:ReportFile::BackupReportTemporary();
      keep_backup = Resume();
      break;
    case AppCommand::Command_CsvRecentTaskNumbers:return CsvRecentTaskNumbers();
    case AppCommand::Command_CsvDayTracks:return CsvTodayTracks();
    case AppCommand::Command_Remove:ReportFile::BackupReportTemporary();
      keep_backup = Remove();
      break;
    case AppCommand::Command_Split:ReportFile::BackupReportTemporary();
      keep_backup = Split();
      break;
    case AppCommand::Command_Start:ReportFile::BackupReportTemporary();
      keep_backup = Start();
      break;
    case AppCommand::Command_Stop:ReportFile::BackupReportTemporary();
      keep_backup = Stop();
      break;
    case AppCommand::Command_Task:ReportFile::BackupReportTemporary();
      keep_backup = UpdateTaskNumber();
      break;
    case AppCommand::Command_Undo:return ReportFile::RestoreBackup();
    case AppCommand::Command_Version:AppHelp::PrintVersion();
      std::cout << "\n";
      return true;
    case AppCommand::Command_View:return View();
    case AppCommand::Command_ViewWeek:return ViewWeek();
    case AppCommand::Command_BrowseDayTasks:return BrowseDayTasks();
    case AppCommand::Command_Invalid:
    default:AppHelp::PrintUnknownArgumentMessage(arguments_->argv_[1]);
      return false;
  }

  return keep_backup ? ReportFile::ActivateTemporaryBackup() : ReportFile::RemoveTemporaryBackup();
}

/** @todo check: "d" command crashes when current day has less time left than a full-day entry spans?  */
bool App::AddFullDayEntry() {
  ReportCrud &report = ReportCrud::GetInstance();

  // "ttt d" - No arguments given: Add start entry w/o comment or task number at current day
  if (arguments_->argc_==2) return report.AddFullDayEntry();

  int offset_days = arguments_->GetNegativeNumber();
  std::string comment = arguments_->GetComment();

  std::string task_number = helper::Numeric::ToString(arguments_->GetTaskNumber());
  if (task_number=="-1") task_number = "";

  return report.AddFullDayEntry(offset_days, comment, task_number);
}

bool App::BrowseTaskUrl() {
  auto *browser = new ReportBrowser();

  std::string url_command = "url.default";

  if (arguments_->argc_==5) {
    url_command = std::string("url.").append(arguments_->argv_[4]);
  } else if (arguments_->argc_==4 && !arguments_->IsNumber(3)) {
    // E.g.: "u 123 edit"
    url_command = std::string("url.").append(arguments_->argv_[3]);
  } else if (arguments_->argc_==3 && !arguments_->IsNumber(2) && 'd'!=arguments_->argv_[2][0]) {
    // E.g.: "u edit"
    url_command = std::string("url.").append(arguments_->argv_[2]);
  }

  return browser->BrowseTaskUrlsInScope(
      static_cast<ReportRendererCli::RenderScopes>(arguments_->render_scope_), arguments_->GetNegativeNumber(),
      arguments_->GetTaskNumber(), url_command);
}

bool App::DisplayDate() {
  auto *report_date_time_ = new ReportDateTime();;

  if (arguments_->argc_==2) {
    std::cout << report_date_time_->GetDateFormatted(0) << "\n";
    return true;
  }

  int offset_days = arguments_->ResolveNumber(2);
  std::cout << report_date_time_->GetDateFormatted(offset_days) << "\n";

  return true;
}

/**
 * Export whole report to CSV file
 */
bool App::ExportCsv() {
  ReportRendererCsv renderer;
  std::string path = helper::System::GetBinaryPath(arguments_->argv_, std::strlen(App::kAppExecutableName.c_str()));

  return renderer.RenderToFile(path, static_cast<ReportRendererCli::RenderScopes>(arguments_->render_scope_));
}

bool App::Help() {
  AppCommand::Commands command = arguments_->ResolveCommandName(2);
  AppHelp::PrintHelp(true, command);

  return true;
}

/**
 * Merge given task with following one
 */
bool App::Merge() {
  ReportParser *parser = new ReportParser();
  if (!parser->LoadReportHtml()) return false;

  int last_row_index = parser->GetLastIndex();
  if (last_row_index==0)
    return tictac_track::AppError::PrintError("Cannot merge: There's only one entry.");

  int row_index = 2==arguments_->argc_ ? last_row_index - 1 : arguments_->ResolveNumber(2);
  if (row_index < 0)
    return tictac_track::AppError::PrintError("Cannot merge: Entry index cannot be < 0.");

  return row_index >= last_row_index
         ? tictac_track::AppError::PrintError("Cannot merge: Last mergeable ID is:", last_row_index - 1)
         : ReportCrud::GetInstance().Merge(row_index);
}

bool App::Recalculate() {
  return ReportRecalculator::RecalculateAndUpdate();
}

/**
 * Resume a previous entry
 */
bool App::Resume() {
  ReportParser *parser = new ReportParser();
  if (!parser->LoadReportHtml()) return false;

  if (-1==parser->GetLastIndex())
    return tictac_track::AppError::PrintError("Cannot resume: there are no entries.");

  ReportCrud &report = ReportCrud::GetInstance();
  bool is_any_running = report.IsAnyEntryRunning();

  std::string comment = arguments_->GetComment();
  switch (arguments_->argc_) {
    case 2:
      return is_any_running
             ? tictac_track::AppError::PrintError("Cannot resume: last entry is still running.")
             // Resume last entry
             : ResumeEntryByIndexOrNegativeOffset(0, comment);
    case 3:
    case 4:if (is_any_running) report.StopEntry();

      return ResumeEntryByIndexOrNegativeOffset(arguments_->ResolveNumber(2), comment);
    default:return tictac_track::AppError::PrintError("Too many arguments.");
  }
}

/**
 * Resume entry.
 * If given index is positive: is row index,
 * else: negative offset. Append/add given comment
 */
bool App::ResumeEntryByIndexOrNegativeOffset(signed int row_index, std::string add_to_comment) {
  ReportParser *parser = new ReportParser();
  if (!parser->LoadReportHtml()) return false;

  // Negative offset: Convert to index
  if (0 >= row_index)
    row_index = parser->GetLastIndex() + row_index;

  int last_index = parser->GetLastIndex();
  if (row_index > last_index) {
    bool can_resume = false;

    // Check: did user pass a task-number instead of the required ID?
    std::string task_number = helper::Numeric::ToString(row_index);
    int row_index_by_task = parser->GetLatestIndexByTaskNumber(task_number);
    if (row_index_by_task > -1) {
      // Suggest resuming the last entry of that task
      std::cout << "Cannot resume entry " << row_index << ", last entry is " << last_index << "."
                << " Do you want to resume entry " << row_index_by_task << " (last item of task " << row_index
                << ")?\nY/n";

      bool do_resume_by_task = helper::System::GetYesOrNoKeyPress();
      std::cout << "\n";

      if (!do_resume_by_task) return false;

      can_resume = true;
      row_index = row_index_by_task;
    }

    if (!can_resume)
      return tictac_track::AppError::PrintError(
          std::string("Cannot resume entry ")
              .append(helper::Numeric::ToString(row_index))
              .append(", last entry is ")
              .append(helper::Numeric::ToString(last_index))
              .append(".")
              .c_str());
  }
  std::string html = parser->GetHtml();
  int offset_tr = parser->GetOffsetTrOpenByIndex(html, row_index);
  std::string task_number =
      parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Task, offset_tr);

  std::string comment_old = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Comment, offset_tr);
  std::string comment = ReportParser::MergeComments(comment_old, add_to_comment);

  return ReportCrud::GetInstance().StartEntry(comment.c_str(), task_number.c_str());
}

/**
 * Remove entries
 */
bool App::Remove() {
  ReportParser *parser = new ReportParser();
  if (!parser->LoadReportHtml()) return false;

  int last_index = parser->GetLastIndex();
  if (-1==last_index)
    return tictac_track::AppError::PrintError("Cannot remove: there are no entries.");

  ReportCrud &report = ReportCrud::GetInstance();
  // No arguments: remove last entry
  if (arguments_->argc_ < 3) return report.RemoveEntries(1);

  // Remove all
  if (arguments_->all_) return report.Reset();

  if (!arguments_->Contains(2, "=")) {
    if (arguments_->IsNumber(2)) {
      int amount_rows = arguments_->ResolveNumber(2);

      // Negative number: remove given amount of latest entries
      if (amount_rows < 0) return report.RemoveEntries(amount_rows*-1);

      // Numeric argument: remove entry w/ given ID
      report.RemoveEntryById(amount_rows);
    }
  } else if (helper::String::StartsWith(arguments_->argv_[2], "i=")) {
    // Index-based argument: remove entry w/ given ID
    return report.RemoveEntryById(arguments_->ResolveNumber(arguments_->argument_index_entry_id_));
  }

  return tictac_track::AppError::PrintError("Removal argument unclear.");
}

/**
 * Split entry into two
 */
bool App::Split() {
  int row_index = arguments_->ResolveNumber(2);
  if (-1==row_index)
    return tictac_track::AppError::PrintError("No entry ID given.");

  ReportParser *parser = new ReportParser();
  if (!parser->LoadReportHtml()) return false;

  int last_index = parser->GetLastIndex();

  if (last_index==-1)
    return tictac_track::AppError::PrintError("Cannot split: there are no entries yet.");

  if (parser->IsEntryRunning(row_index))
    return tictac_track::AppError::PrintError("Cannot split: Entry is still running.");

  if (last_index < row_index)
    return tictac_track::AppError::PrintError(
        std::string("Cannot split entry ")
            .append(helper::Numeric::ToString(row_index))
            .append(", last entry is ")
            .append(helper::Numeric::ToString(last_index))
            .append(".")
            .c_str());

  std::string split_duration = arguments_->ResolveTime(3, true);
  if (split_duration.empty())
    return tictac_track::AppError::PrintError("Failed parsing entry-duration to split at.");

  if (split_duration[0]=='-')
    split_duration = split_duration.substr(1, std::string::npos);

  return SplitAtEnd(parser, split_duration, row_index);
}

/**
 * Split given entry at given duration before its end into two entries
 */
bool App::SplitAtEnd(ReportParser *parser, std::string split_duration, int row_index) {
  // New entry end is end of entry before reduction
  std::string time_end = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_End);

  // Negative duration: Reduce end-time, insert new entry after: w/ start-time same as changed end-time
  if (!parser->ReduceEntryTime(
      row_index,
      std::move(split_duration),
      AppCommand::Commands::Command_Split)
      )
    return false;

  std::string html = parser->GetHtml();
  ReportRecalculator::CalculateAndUpdateDuration(html, row_index);
  parser->SetHtml(html);

  int offset_tr = parser->GetOffsetTrOpenByIndex(html, row_index);
  // New entry start is end of reduced entry
  std::string time_start = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_End, offset_tr);
  std::string meta = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Meta, offset_tr);
  std::string week_number = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Week, offset_tr);
  std::string weekday = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Day, offset_tr);
  std::string day_date = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Date, offset_tr);
  std::string task_number = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Task, offset_tr);

  return ReportCrud::GetInstance().InsertEntryAfter(
      html, row_index, meta, week_number, weekday, day_date, time_start, time_end, task_number);
}

/**
 * Start new entry or modify start-time of given entry
 */
bool App::Start() {
  // Detect ID out of invocation like: "s 1 0:30"
  if (4==arguments_->argc_
      && arguments_->IsNumber(2)
      && arguments_->IsTime(3)
      )
    arguments_->argument_index_entry_id_ = 2;

  if (-1!=arguments_->argument_index_entry_id_) {
    // Index given: update already existing entry
    return UpdateTime(Report::ColumnIndexes::Index_Start);
  }

  ReportCrud &report = ReportCrud::GetInstance();

  // Stop currently ongoing entry if any
  if (report.IsAnyEntryRunning()) report.StopEntry();

  // No arguments given: Add start entry w/o comment or task number
  if (arguments_->argc_==2) return report.StartEntry("", "");

  // Create + start entry
  switch (arguments_->argv_types_[2]) {
    case AppArguments::ArgumentType_Comment:
      // Start w/ comment, task number if given
      return report.StartEntry(arguments_->ResolveComment(2).c_str(), arguments_->ResolveNumber(3));
    case AppArguments::ArgumentType_Number:
      // Start w/ task number, comment if given
      return report.StartEntry(arguments_->ResolveComment(3).c_str(), arguments_->ResolveNumber(2));
    case AppArguments::ArgumentType_Invalid:
    case AppArguments::ArgumentType_OptionAll:
    case AppArguments::ArgumentType_OptionIndex:
    case AppArguments::ArgumentType_RenderScope:
    case AppArguments::ArgumentType_Time:return false;
    default:return false;
  }
}

/**
 * Add/update end-time to/of entry, append comment if given
 */
bool App::Stop() {
  // Index given: update already existing entry
  if (-1!=arguments_->argument_index_entry_id_)
    return UpdateTime(Report::ColumnIndexes::Index_End);

  ReportCrud &report = ReportCrud::GetInstance();
  if (report.IsAnyEntryRunning())
    return report.StopEntry(
        // TODO encode comment special chars to HTML entities
        arguments_->ResolveComment(2).c_str()
    );

  return tictac_track::AppError::PrintError("Cannot stop: No entry is currently running.");
}

/**
 * Add/Append/Unset comment of latest or entry/entries with given ID/IDs
 */
bool App::UpdateComment() {
  ReportParser *parser = new ReportParser();
  if (!parser->LoadReportHtml()) return false;

  int last_index = parser->GetLastIndex();
  if (-1==last_index)
    return tictac_track::AppError::PrintError("Cannot update comment: there are no entries.");

  std::vector<int> row_ids{-1};
  int comment_argument_offset = 2;

  if (arguments_->has_multiple_ids_) {
    // Multiple comma-separated indexes given
    row_ids = arguments_->ids_;
    comment_argument_offset = 3;
  } else if (-1!=arguments_->argument_index_entry_id_) {
    // Single index given: find argument-offsets of comment and row-index (allow arbitrary order)
    row_ids[0] = arguments_->ResolveNumber(arguments_->argument_index_entry_id_);

    if (row_ids[0] < 0)
      return tictac_track::AppError::PrintError("Cannot update comment: Index cannot be < 0.");

    if (arguments_->argc_ > 2) {
      if (2==arguments_->argument_index_entry_id_)
        comment_argument_offset = 3;
    } else comment_argument_offset = -1;
  }

  bool starts_with_space;
  std::string comment;

  if (2==arguments_->argc_
      || -1==comment_argument_offset
      || comment_argument_offset >= arguments_->argc_
      ) {
    starts_with_space = false;
    comment = "";
  } else {
    starts_with_space = ' '==arguments_->argv_[comment_argument_offset][0];
    comment = arguments_->ResolveComment(comment_argument_offset);
  }

  bool res = true;
  for (auto const &index: row_ids) {
    res = UpdateCommentByEntryId(last_index, index, comment, starts_with_space) && res;
  }

  return res;
}

bool App::ClearTimesheet() {
  tictac_track::ReportCrud report_crud = tictac_track::ReportCrud::GetInstance(true);
  return true;
}

bool App::UpdateCommentByEntryId(int last_index, int index, std::string comment, bool starts_with_space) {
  if (index > last_index)
    return tictac_track::AppError::PrintError(
        std::string("Cannot update comment of entry ")
            .append(helper::Numeric::ToString(index))
            .append(", last index is: ")
            .append(helper::Numeric::ToString(last_index))
            .c_str());

  return comment.empty()
         ? ReportParser::UpdateColumn(index, Report::Index_Comment, "")
         : ReportCrud::GetInstance().AppendComment(comment, index, starts_with_space);
}

bool App::UpdateTaskNumber() {
  int task_argument_offset = 2;
  std::vector<int> row_ids{-1};

  if (arguments_->has_multiple_ids_) {
    // Multiple comma-separated indexes given
    row_ids = arguments_->ids_;
    task_argument_offset = arguments_->argument_index_entry_id_==2 ? 3 : 2;
  } else if (arguments_->argument_index_entry_id_!=-1) {
    // Index given: find argument-offsets of task number and row-index (allow arbitrary order)
    row_ids[0] = arguments_->ResolveNumber(arguments_->argument_index_entry_id_);

    if (row_ids[0] < 0)
      return tictac_track::AppError::PrintError("Entry index cannot be < 0.");

    task_argument_offset = arguments_->argument_index_entry_id_==2 ? 3 : 2;
  }

  int task_number = arguments_->ResolveNumber(task_argument_offset);
  ReportCrud &reportCrud = ReportCrud::GetInstance();

  int argument_index_comment = GetCommentArgOffsetInTaskCommand();
  std::string comment = argument_index_comment > -1
                        ? arguments_->ResolveComment(argument_index_comment)
                        : "";

  bool has_comment = !comment.empty();

  bool comment_starts_with_space =
      has_comment
          && ' '==arguments_->argv_[argument_index_comment][0];

  bool res = true;
  for (auto const &index: row_ids) {
    res = reportCrud.UpdateTaskNumber(task_number, index) && res;

    if (has_comment)
      reportCrud.AppendComment(comment, index, comment_starts_with_space);
  }

  return res;
}

int App::GetCommentArgOffsetInTaskCommand() const {
  // Get optional comment from invocation like: "t 1234 comment", or: "t 1234 comment"
  if (4==arguments_->argc_
      && !helper::String::IsNumeric(arguments_->argv_[3])
      )
    return 3;

  // Get optional comment from invocation like: "t i=1 1234 comment", or: "t i=1,2,3 1234 comment"
  if (5==arguments_->argc_
      && !helper::String::IsNumeric(arguments_->argv_[4])
      )
    return 4;

  return -1;
}

/**
 * Update time of row + column by arguments
 */
bool App::UpdateTime(Report::ColumnIndexes column_index) {
  if (arguments_->argc_ < 4)
    return tictac_track::AppError::PrintError("To few arguments: missing time argument in format: \"hh:mm\".");

  int row_index = arguments_->ResolveNumber(arguments_->argument_index_entry_id_);
  if (row_index < 0)
    return tictac_track::AppError::PrintError("Entry index cannot be < 0.");

  std::string time;
  if (-1==arguments_->argument_index_time_) {
    // Fallback: Try find index of time-argument now:
    // if entry-index is argument 2, than time is 3, if entry-index is 3- time is 2.
    arguments_->argument_index_time_ = arguments_->argument_index_entry_id_==2 ? 3 : 2;
  }

  if (-1!=arguments_->argument_index_time_) {
    time = arguments_->ResolveTime(arguments_->argument_index_time_);
    if (!time.empty()) {
      // Start-time is allowed to be > end-time, it is than interpreted as if the entry spans over midnight
      if (!ReportParser::UpdateColumn(row_index, column_index, time))
        return tictac_track::AppError::PrintError(
            std::string("Update column failed (")
                .append(helper::Numeric::ToString(column_index))
                .append(")").c_str()
        );

      ReportFile::SaveReport(ReportRecalculator::CalculateAndUpdateDuration(row_index));

      return true;
    }
  }

  return tictac_track::AppError::PrintError("Failed parsing arguments.");
}

/**
 * Pretty-print report to CLI
 */
bool App::View() {
  ReportRendererCli renderer;

  AppConfig config = AppConfig::GetInstance();

  if (config.GetConfigValue("clear_before_view")=="1")
    helper::Tui::ClearConsole();

  return renderer.PrintToCli(
      static_cast<ReportRendererCli::RenderScopes>(arguments_->render_scope_),
      arguments_->GetNegativeNumber(),
      arguments_->GetTaskNumber(),
      arguments_->GetComment());
}

bool App::ViewWeek() {
  ReportRendererCli renderer;

  AppConfig config = AppConfig::GetInstance();
  if (config.GetConfigValue("clear_before_view")=="1") helper::Tui::ClearConsole();

  return renderer.PrintToCli(
      Report::RenderScopes::Scope_Week, arguments_->GetNegativeNumber(),
      arguments_->GetTaskNumber(), arguments_->GetComment());
}

bool App::CsvRecentTaskNumbers() {
  int amount = arguments_->GetTaskNumber();

  std::cout << "@todo implement print csv of 50 recent task numbers...";
  return true;
}

bool App::CsvTodayTracks() {
  ReportRendererCsv renderer;

  return renderer.RenderToStdOut(static_cast<ReportRendererCli::RenderScopes>(arguments_->render_scope_));
}

/**
 * View entries sequentially, opening related task URL at the same time
 */
bool App::BrowseDayTasks() {
  ReportRendererCli renderer;

  AppConfig config = AppConfig::GetInstance();
  if (config.GetConfigValue("clear_before_view")=="1") helper::Tui::ClearConsole();

  return renderer.PrintBrowseDayTasks(arguments_->GetNegativeNumber());
}

} // namespace tictac_track
