
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <iostream>
#include <cstring>
#include "app.h"
#include "app_config.h"
#include "app_help.h"
#include "apps/client/report/report_browser.h"
#include "apps/client/report/report_file.h"
#include "apps/client/report/report_crud.h"
#include "apps/client/report/report_renderer_csv.h"
#include "apps/client/report/report_renderer_cli.h"
#include "lib/app/app_error.h"
#include "apps/client/report/report_recalculator.h"
#include "lib/helper/helper_numeric.h"
#include "lib/helper/helper_string.h"
#include "lib/helper/helper_date_time.h"
#include "lib/helper/helper_system.h"

namespace tictac_track {

// Meta constants
const std::string App::kAppName = "tictac-track";
const std::string App::kAppExecutableName = "ttt";
const std::string App::kAppVersion = "v1.3";

/**
 * Constructor: init (resolve) command and arguments
 */
App::App(int argc, char **argv) {
  if (argc == 1) {
    // No command given
    AppHelp::PrintHelp(true, AppCommand::Commands::Command_Invalid);
    return;
  }

  command_ = new AppCommand(argc > 0 ? argv[1] : "");
  arguments_ = new AppArguments(argc, argv, *command_);
}

void App::function(bool& keep_backup){
  ReportFile::BackupReportTemporary();
  keep_backup = Split();
}

/**
 * Process command + arguments
 */
bool App::Process() {
  bool keep_backup;

  switch (command_->GetResolved()) {
    case AppCommand::Command_Browse:
      return ReportBrowser::Browse();
    case AppCommand::Command_Comment:
      ReportFile::BackupReportTemporary();
      keep_backup = UpdateComment();
      break;
    case AppCommand::Command_Csv:
      return ExportCsv();
    case AppCommand::Command_Date:
      return DisplayDate();
    case AppCommand::Command_Day:
      ReportFile::BackupReportTemporary();
      keep_backup = AddFullDayEntry();
      break;
    case AppCommand::Command_ExternalTaskUrl:
      return BrowseTaskUrl();
    case AppCommand::Command_Help:
      return Help();
    case AppCommand::Command_Merge:
      ReportFile::BackupReportTemporary();
      keep_backup = Merge();
      break;
    case AppCommand::Command_Recalculate:
      ReportFile::BackupReportTemporary();
      keep_backup = Recalculate();
      break;
    case AppCommand::Command_Resume:
      ReportFile::BackupReportTemporary();
      keep_backup = Resume();
      break;
    case AppCommand::Command_Remove:
      ReportFile::BackupReportTemporary();
      keep_backup = Remove();
      break;
    case AppCommand::Command_Split:
      function(keep_backup);
      break;
    case AppCommand::Command_Start:
      ReportFile::BackupReportTemporary();
      keep_backup = Start();
      break;
    case AppCommand::Command_Stop:
      ReportFile::BackupReportTemporary();
      keep_backup = Stop();
      break;
    case AppCommand::Command_Task:
      ReportFile::BackupReportTemporary();
      keep_backup = UpdateTaskNumber();
      break;
    case AppCommand::Command_Undo:
      return ReportFile::RestoreBackup();
    case AppCommand::Command_Version:
      AppHelp::PrintVersion();
      std::cout << "\n";
      return true;
    case AppCommand::Command_View:
      return View();
    case AppCommand::Command_ViewWeek:
      return ViewWeek();
    case AppCommand::Command_Invalid:
    default:
      AppHelp::PrintUnknownArgumentMessage(arguments_->argv_[1]);
      return false;
  }

  return keep_backup ? ReportFile::ActivateTemporaryBackup() : ReportFile::RemoveTemporaryBackup();
}

bool App::AddFullDayEntry() {
  ReportCrud &report = ReportCrud::GetInstance();

  // "ttt d" - No arguments given: Add start entry w/o comment or task number at current day
  if (arguments_->argc_ == 2) return report.AddFullDayEntry();

  int offset_days = arguments_->GetNegativeNumber();
  std::string comment = arguments_->GetComment();
  std::string task_number = helper::Numeric::ToString(arguments_->GetTaskNumber());
  if (task_number == "-1") task_number = "";

  return report.AddFullDayEntry(offset_days, comment, task_number);
}

bool App::BrowseTaskUrl() {
  auto *browser = new ReportBrowser();

  std::string url_command = "url.default";

  if (arguments_->argc_ == 5) {
    url_command = std::string("url.").append(arguments_->argv_[4]);
  } else if (arguments_->argc_ == 4 && !arguments_->IsNumber(3)) {
    // E.g.: "u 123 edit"
    url_command = std::string("url.").append(arguments_->argv_[3]);
  } else if (arguments_->argc_ == 3 && !arguments_->IsNumber(2) && 'd' != arguments_->argv_[2][0]) {
    // E.g.: "u edit"
    url_command = std::string("url.").append(arguments_->argv_[2]);
  }

  return browser->BrowseTaskUrl(
      static_cast<ReportRendererCli::RenderScopes>(arguments_->render_scope_), arguments_->GetNegativeNumber(),
      arguments_->GetTaskNumber(), url_command);
}

bool App::DisplayDate() {
  auto *report_date_time_ = new ReportDateTime();;

  if (arguments_->argc_ == 2) {
    std::cout << report_date_time_->GetCurrentDate(0) << "\n";
    return true;
  }

  int offset_days = arguments_->ResolveNumber(2);
  std::cout << report_date_time_->GetCurrentDate(offset_days) << "\n";

  return true;
}

/**
 * Export report to CSV file
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
  if (last_row_index == 0) return tictac_lib::AppError::PrintError("Cannot merge: There's only one entry.");

  int row_index = 2 == arguments_->argc_ ? last_row_index - 1 : arguments_->ResolveNumber(2);
  if (row_index < 0) return tictac_lib::AppError::PrintError("Cannot merge: Entry index cannot be < 0.");

  return row_index >= last_row_index
         ? tictac_lib::AppError::PrintError("Cannot merge: Last mergeable ID is:", last_row_index - 1)
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
  if (-1 == parser->GetLastIndex()) return tictac_lib::AppError::PrintError("Cannot resume: there are no entries.");

  ReportCrud &report = ReportCrud::GetInstance();
  bool is_any_running = report.IsAnyEntryRunning();

  switch (arguments_->argc_) {
    case 2:if (is_any_running) return tictac_lib::AppError::PrintError("Cannot resume: last entry is still running.");
      // Resume last entry
      return ResumeEntryByIndexOrNegativeOffset(0);
    case 3:if (is_any_running) report.StopEntry();
      return ResumeEntryByIndexOrNegativeOffset(arguments_->ResolveNumber(2));
    default:return tictac_lib::AppError::PrintError("Too many arguments.");
  }
}

/**
 * Resume entry, if given index is positive: is row index, else: negative offset
 */
bool App::ResumeEntryByIndexOrNegativeOffset(signed int row_index) {
  ReportParser *parser = new ReportParser();
  if (!parser->LoadReportHtml()) return false;

  // Negative offset: Convert to index
  if (0 >= row_index) row_index = parser->GetLastIndex() + row_index;

  int last_index = parser->GetLastIndex();
  if (row_index > last_index) {
    bool can_resume = false;

    // Check: did user pass as task-number instead the required ID?
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
      return tictac_lib::AppError::PrintError(
          std::string("Cannot resume entry ").append(helper::Numeric::ToString(row_index)).append(", last entry is ")
              .append(helper::Numeric::ToString(last_index)).append(".").c_str());
  }

  std::string task_number = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Task);
  std::string comment = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Comment);

  return ReportCrud::GetInstance().StartEntry(comment.c_str(), task_number.c_str());
}

/**
 * Remove entries
 */
bool App::Remove() {
  ReportParser *parser = new ReportParser();
  if (!parser->LoadReportHtml()) return false;
  int last_index = parser->GetLastIndex();
  if (-1 == last_index) return tictac_lib::AppError::PrintError("Cannot remove: there are no entries.");

  ReportCrud &report = ReportCrud::GetInstance();
  // No arguments: remove last entry
  if (arguments_->argc_ < 3) return report.RemoveEntries(1);
  // Remove all
  if (arguments_->all_) return report.Reset();

  if (!arguments_->Contains(2, "=")) {
    if (arguments_->IsNumber(2)) {
      int amount_rows = arguments_->ResolveNumber(2);

      // Negative number: remove given amount of latest entries
      if (amount_rows < 0) return report.RemoveEntries(amount_rows * -1);

      // Numeric argument: remove entry w/ given ID
      report.RemoveEntryById(amount_rows);
    }
  } else if (helper::String::StartsWith(arguments_->argv_[2], "i=")) {
    // Index-based argument: remove entry w/ given ID
    return report.RemoveEntryById(arguments_->ResolveNumber(arguments_->argument_index_entry_id_));
  }

  return tictac_lib::AppError::PrintError("Removal argument unclear.");
}

/**
 * Split entry into two
 */
bool App::Split() {
  int row_index = arguments_->ResolveNumber(2);
  if (-1 == row_index) return tictac_lib::AppError::PrintError("No entry ID given.");

  ReportParser *parser = new ReportParser();
  if (!parser->LoadReportHtml()) return false;

  int last_index = parser->GetLastIndex();
  if (last_index == -1) return tictac_lib::AppError::PrintError("Cannot split: there are no entries yet.");

  if (parser->IsEntryRunning(row_index)) return tictac_lib::AppError::PrintError("Cannot split: Entry is still running.");
  if (last_index < row_index)
    return tictac_lib::AppError::PrintError(std::string("Cannot split entry ")
                                    .append(helper::Numeric::ToString(row_index)).append(", last entry is ").append(
            helper::Numeric::ToString(last_index))
                                    .append(".").c_str());

  std::string split_duration = arguments_->ResolveTime(3, true);
  if (split_duration.empty()) return tictac_lib::AppError::PrintError("Failed parsing entry-duration to split at.");

  if (split_duration[0] == '-') split_duration = split_duration.substr(1, std::string::npos);

  return SplitAtEnd(parser, split_duration, row_index);
}

/**
 * Split given entry at given duration before its end into two entries
 */
bool App::SplitAtEnd(ReportParser *parser, std::string split_duration, int row_index) {
  // New entry end is end of entry before reduction
  std::string time_end = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_End);

  // Negative duration: Reduce end-time, insert new entry after: w/ start-time same as changed end-time
  if (!parser->ReduceEntryTime(row_index, std::move(split_duration), AppCommand::Commands::Command_Split)) return false;

  std::string html = parser->GetHtml();
  ReportRecalculator::CalculateAndUpdateDuration(html, row_index);
  parser->SetHtml(html);

  // New entry start is end of reduced entry
  std::string time_start = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_End);
  std::string meta = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Meta);
  std::string week_number = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Week);
  std::string weekday = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Day);
  std::string day_date = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Date);
  std::string task_number = parser->GetColumnContent(row_index, Report::ColumnIndexes::Index_Task);

  return ReportCrud::GetInstance().InsertEntryAfter(
      html, row_index, meta, week_number, weekday, day_date, time_start, time_end, task_number);
}

/**
 * Add start-entry
 */
bool App::Start() {
  // Detect ID out of invocation like: "s 1 0:30"
  if (4 == arguments_->argc_ && arguments_->IsNumber(2) && arguments_->IsTime(3))
    arguments_->argument_index_entry_id_ = 2;

  if (-1 != arguments_->argument_index_entry_id_) {
    // Index given: update already existing entry
    return UpdateTime(Report::ColumnIndexes::Index_Start);
  }

  ReportCrud &report = ReportCrud::GetInstance();

  // Stop currently ongoing entry if any
  if (report.IsAnyEntryRunning()) report.StopEntry();

  // No arguments given: Add start entry w/o comment or task number
  if (arguments_->argc_ == 2) return report.StartEntry("", "");

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
    case AppArguments::ArgumentType_Time:
      return false;
  }

  return false;
}

/**
 * Add/update end-time to/of entry, append comment if given
 */
bool App::Stop() {
  // Index given: update already existing entry
  if (-1 != arguments_->argument_index_entry_id_) return UpdateTime(Report::ColumnIndexes::Index_End);

  ReportCrud &report = ReportCrud::GetInstance();
  if (report.IsAnyEntryRunning())
    return report.StopEntry(
        // TODO encode comment special chars to HTML entities
        arguments_->ResolveComment(2).c_str()
    );

  return tictac_lib::AppError::PrintError("Cannot stop: No entry is currently running.");
}

/**
 * Add/Append/Unset comment of latest or entry/entries with given ID/IDs
 *
 * @todo TBD: add argument that allows reuse of already existing comments, e.g. "c from=<TASK-NO>"
 */
bool App::UpdateComment() {
  ReportParser *parser = new ReportParser();
  if (!parser->LoadReportHtml()) return false;
  int last_index = parser->GetLastIndex();
  if (-1 == last_index) return tictac_lib::AppError::PrintError("Cannot update comment: there are no entries.");

  std::vector<int> row_ids{-1};
  int comment_argument_offset = 2;

  if (arguments_->has_multiple_ids_) {
    // Multiple comma-separated indexes given
    row_ids = arguments_->ids_;
    comment_argument_offset = 3;
  } else if (-1 != arguments_->argument_index_entry_id_) {
    // Single index given: find argument-offsets of comment and row-index (allow arbitrary order)
    row_ids[0] = arguments_->ResolveNumber(arguments_->argument_index_entry_id_);
    if (row_ids[0] < 0) return tictac_lib::AppError::PrintError("Cannot update comment: Index cannot be < 0.");

    if (arguments_->argc_ > 2) {
      if (2 == arguments_->argument_index_entry_id_) {
        comment_argument_offset = 3;
      }
    } else {
      comment_argument_offset = -1;
    }
  }

  bool starts_with_space;
  std::string comment;

  if (2 == arguments_->argc_ || -1 == comment_argument_offset || comment_argument_offset >= arguments_->argc_) {
    starts_with_space = false;
    comment = "";
  } else {
    starts_with_space = ' ' == arguments_->argv_[comment_argument_offset][0];
    comment = arguments_->ResolveComment(comment_argument_offset);
  }

  bool res = true;
  for(auto const &index: row_ids) {
    res = UpdateCommentByEntryId(last_index, index, comment, starts_with_space) && res;
  }

  return res;
}

bool App::UpdateCommentByEntryId(int last_index, int index, std::string comment, bool starts_with_space) {
  if (index > last_index)
    return tictac_lib::AppError::PrintError(
        std::string("Cannot update comment of entry ").append(helper::Numeric::ToString(index)).append(", last index is: ")
            .append(helper::Numeric::ToString(last_index)).c_str());

  if (comment.empty()) return ReportParser::UpdateColumn(index, Report::Index_Comment, "");

  return ReportCrud::GetInstance().AppendComment(comment, index, starts_with_space);
}

bool App::UpdateTaskNumber() {
  int task_argument_offset = 2;
  std::vector<int> row_ids{-1};

  if (arguments_->has_multiple_ids_) {
    // Multiple comma-separated indexes given
    row_ids = arguments_->ids_;
    task_argument_offset = arguments_->argument_index_entry_id_ == 2 ? 3 : 2;
  } else if (arguments_->argument_index_entry_id_ != -1) {
    // Index given: find argument-offsets of task number and row-index (allow arbitrary order)
    row_ids[0] = arguments_->ResolveNumber(arguments_->argument_index_entry_id_);
    if (row_ids[0] < 0) return tictac_lib::AppError::PrintError("Entry index cannot be < 0.");

    task_argument_offset = arguments_->argument_index_entry_id_ == 2 ? 3 : 2;
  }

  int task_number = arguments_->ResolveNumber(task_argument_offset);
  ReportCrud &reportCrud = ReportCrud::GetInstance();

  int argument_index_comment = GetCommentArgOffsetInTaskCommand();
  std::string comment = argument_index_comment > -1 ? arguments_->ResolveComment(argument_index_comment) : "";
  bool has_comment = !comment.empty();
  bool comment_starts_with_space = has_comment && ' ' == arguments_->argv_[argument_index_comment][0];

  bool res = true;
  for(auto const &index: row_ids) {
    res = reportCrud.UpdateTaskNumber(task_number, index) && res;
    if (has_comment) reportCrud.AppendComment(comment, index, comment_starts_with_space);
  }

  return res;
}

int App::GetCommentArgOffsetInTaskCommand() const {
  // Get optional comment from invocation like: "t 1234 comment", or: "t 1234 comment"
  if (4 == arguments_->argc_ && !helper::String::IsNumeric(arguments_->argv_[3])) return 3;

  // Get optional comment from invocation like: "t i=1 1234 comment", or: "t i=1,2,3 1234 comment"
  if (5 == arguments_->argc_ && !helper::String::IsNumeric(arguments_->argv_[4])) return 4;

  return -1;
}

/**
 * Update time of row + column by arguments
 */
bool App::UpdateTime(Report::ColumnIndexes column_index) {
  if (arguments_->argc_ < 4) return tictac_lib::AppError::PrintError("To few arguments: missing time argument in format: \"hh:mm\".");

  int row_index = arguments_->ResolveNumber(arguments_->argument_index_entry_id_);
  if (row_index < 0) return tictac_lib::AppError::PrintError("Entry index cannot be < 0.");

  std::string time;
  if (-1 == arguments_->argument_index_time_) {
    // Fallback: Try find index of time-argument now: if entry-index is argument 2, than time is 3, if entry-index is 3- time is 2.
    arguments_->argument_index_time_ = arguments_->argument_index_entry_id_ == 2 ? 3 : 2;
  }
  if (-1 != arguments_->argument_index_time_) {
    time = arguments_->ResolveTime(arguments_->argument_index_time_);
    if (!time.empty()) {
      // Start-time is allowed to be > end-time, it is than interpreted as if the entry spans over midnight
      if (!ReportParser::UpdateColumn(row_index, column_index, time))
        return tictac_lib::AppError::PrintError(
            std::string("Update column failed (").append(helper::Numeric::ToString(column_index)).append(")").c_str()
        );

      ReportFile::SaveReport(ReportRecalculator::CalculateAndUpdateDuration(row_index));
      return true;
    }
  }

  return tictac_lib::AppError::PrintError("Failed parsing arguments.");
}

/**
 * Pretty-print report to CLI
 */
bool App::View() {
  ReportRendererCli renderer;
  
  AppConfig config = AppConfig::GetInstance();
  if (config.GetConfigValue("clear_before_view") == "1") helper::System::ClearConsole();
  
  return renderer.PrintToCli(static_cast<ReportRendererCli::RenderScopes>(arguments_->render_scope_),
                             arguments_->GetNegativeNumber(), arguments_->GetTaskNumber(), arguments_->GetComment());
}
bool App::ViewWeek() {
  ReportRendererCli renderer;

  return renderer.PrintToCli(Report::RenderScopes::Scope_Week, arguments_->GetNegativeNumber(),
                             arguments_->GetTaskNumber(), arguments_->GetComment());
}
} // namespace tictac_lib
