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

#include <ttt/class/app/app.h>

#include <ttt/helper/helper_tui.h>
#include <ttt/class/app/app_config.h>
#include <ttt/class/app/app_help.h>
#include <ttt/class/report/report_browser.h>
#include <ttt/class/report/report_file.h>
#include <ttt/class/report/report_renderer_csv.h>
#include <ttt/class/report/report_renderer_cli.h>
#include <ttt/class/report/report_recalculator.h>

namespace tictac_track {

// Constructor: init (resolve) command and arguments
App::App(int argc, char **argv) {
  if (argc == 1) {
    // No command given
    AppHelp::PrintHelp(true, AppCommand::Commands::Command_Invalid);

    return;
  }

  command_ = new AppCommand(argc > 0 ? argv[1] : "");
  arguments_ = new AppArguments(argc, argv, *command_);
}

App::~App() {
  delete command_;
  //delete arguments_;
}

// Process command + arguments
bool App::Process() {
  AppCommand::Commands kCommand = command_->GetResolved();

  bool keep_backup;
  ReportFile::BackupReportBeforeProcessCommand(kCommand);

  switch (kCommand) {
    case AppCommand::Command_ClearTimesheet:{
      keep_backup = ClearTimesheet();

      break;
    }
    case AppCommand::Command_BrowseTimesheet: {
      ReportBrowser::BrowseTimesheet();

      return true;
    }
    case AppCommand::Command_BrowseTaskUrl: {
      return BrowseTaskUrl();
    }
    case AppCommand::Command_Comment:{
      keep_backup = UpdateComment();

      break;
    }
    case AppCommand::Command_DisplayCalendarWeek: {
      DisplayCalendarWeek();

      return true;
    }
    case AppCommand::Command_Csv: {
      return ExportCsv();
    }
    case AppCommand::Command_DisplayDate: {
      DisplayDate();

      return true;
    }
    case AppCommand::Command_Day: {
      keep_backup = AddFullDayEntry();
      break;
    }
    case AppCommand::Command_Help: {
      Help();

      return true;
    }
    case AppCommand::Command_Merge: {
      keep_backup = Merge();

      break;
    }
    case AppCommand::Command_Recalculate: {
      keep_backup = Recalculate();

      break;
    }
    case AppCommand::Command_Resume: {
      keep_backup = Resume();

      break;
    }
    case AppCommand::Command_CsvRecentTaskNumbers: {
      return CsvRecentTaskNumbers();
    }
    case AppCommand::Command_CsvDayTracks: {
      return CsvTodayTracks();
    }
    case AppCommand::Command_Remove:{
      keep_backup = Remove();

      break;
    }
    case AppCommand::Command_Split: {
      keep_backup = Split();

      break;
    }
    case AppCommand::Command_Start: {
      keep_backup = Start();

      break;
    }
    case AppCommand::Command_Stop: {
      keep_backup = Stop();

      break;
    }
    case AppCommand::Command_Task: {
      keep_backup = UpdateTaskNumber();

      break;
    }
    case AppCommand::Command_Undo: {
      return ReportFile::RestoreBackup();
    }
    case AppCommand::Command_Version: {
      AppHelp::PrintVersion();
      std::cout << "\n";

      return true;
    }
    case AppCommand::Command_View: {
      return View();
    }
    case AppCommand::Command_ViewWeek: {
      return ViewWeek();
    }
    case AppCommand::Command_BrowseDayTasks: {
      return BrowseDayTasks();
    }
    case AppCommand::Command_Invalid:
    default: {
      AppHelp::PrintUnknownArgumentMessage(arguments_->argv_[1]);

      return false;
    }
  }

  return keep_backup
         ? ReportFile::ActivateTemporaryBackup()
         : ReportFile::RemoveTemporaryBackup();
}

// TODO(kay): cmd d crashes when current day has < time left than
//  a full-day entry spans
bool App::AddFullDayEntry() {
  ReportCrud::GetInstance();

  // "ttt d" - No arguments given:
  // Add start entry w/o comment or task number at current day
  if (arguments_->argc_ == 2) {
    return tictac_track::ReportCrud::AddFullDayEntry();
  }

  int offset_days = arguments_->GetNegativeNumber();

  std::string comment = arguments_->GetComment();

  std::string task_number = helper::Numeric::ToString(
      arguments_->GetTaskNumber());

  if (task_number == "-1") {
    task_number = "";
  }

  return tictac_track::ReportCrud::AddFullDayEntry(
      offset_days,
      comment,
      task_number);
}

bool App::BrowseTaskUrl() {
  auto *browser = new ReportBrowser();

  std::string url_command = "url.default";

  if (arguments_->argc_ == 5) {
    url_command = std::string("url.").append(arguments_->argv_[4]);
  } else if (arguments_->argc_ == 4 && !arguments_->IsNumber(3)) {
    // E.g.: "u 123 edit"
    url_command = std::string("url.").append(arguments_->argv_[3]);
  } else if (
      arguments_->argc_ == 3
          && !arguments_->IsNumber(2)
          && 'd' != arguments_->argv_[2][0]) {
    // E.g.: "u edit"
    url_command = std::string("url.").append(arguments_->argv_[2]);
  }

  auto result = browser->BrowseIssueUrlsInScope(
      static_cast<ReportRendererCli::RenderScopes>(arguments_->render_scope_),
      arguments_->GetNegativeNumber(),
      arguments_->GetTaskNumber(),
      url_command);

  delete browser;

  return result;
}

void App::DisplayCalendarWeek() {
  auto *report_date_time = new ReportDateTime();
  int offset_weeks = arguments_->argc_ > 2 ? arguments_->ResolveNumber(2) : 0;

  int week_number = helper::String::ToInt(
      report_date_time->GetCurrentWeekOfYear(offset_weeks));

  std::cout
      << (offset_weeks == 0 ? "Current " : "")
      << "Week Number: " << week_number << "\n";

  delete report_date_time;
}

void App::DisplayDate() {
  auto *report_date_time = new ReportDateTime();

  if (arguments_->argc_ == 2) {
    std::cout << report_date_time->GetDateFormatted(0) << "\n";

    delete report_date_time;

    return;
  }

  int offset_days = arguments_->ResolveNumber(2);
  std::cout << report_date_time->GetDateFormatted(offset_days) << "\n";

  delete report_date_time;
}

// Export whole report to CSV file
bool App::ExportCsv() {
  ReportRendererCsv renderer;
  std::string path = helper::System::GetBinaryPath(arguments_->argv_, 3);

  return renderer.RenderToFile(
      path,
      static_cast<ReportRendererCli::RenderScopes>(arguments_->render_scope_));
}

void App::Help() {
  AppCommand::Commands command = arguments_->ResolveCommandName(2);
  AppHelp::PrintHelp(true, command);
}

// Merge given task with following one
bool App::Merge() {
  auto *parser = new ReportParser();

  if (!parser->LoadReportHtml()) {
    delete parser;

    return false;
  }

  int last_row_index = parser->GetLastIndex();

  delete parser;

  if (last_row_index == 0) {
    return tictac_track::AppError::PrintError(
        "Cannot merge: There's only one entry.");
  }

  int row_index = 2 == arguments_->argc_
                  ? last_row_index - 1
                  : arguments_->ResolveNumber(2);

  if (row_index < 0) {
    return tictac_track::AppError::PrintError(
        "Cannot merge: Entry index cannot be < 0.");
  }

  return row_index >= last_row_index
         ? tictac_track::AppError::PrintError(
          "Cannot merge: Last mergeable ID is:",
          last_row_index - 1)
         : ReportCrud::Merge(row_index);
}

bool App::Recalculate() {
  return ReportRecalculator::RecalculateAndUpdate();
}

// Resume a previous entry
bool App::Resume() {
  auto *parser = new ReportParser();

  if (!parser->LoadReportHtml()) {
    delete parser;

    return false;
  }

  if (-1 == parser->GetLastIndex()) {
    delete parser;

    return tictac_track::AppError::PrintError(
        "Cannot resume: there are no entries.");
  }

  ReportCrud &report = ReportCrud::GetInstance();

  bool is_any_ongoing = tictac_track::ReportCrud::IsAnyEntryOngoing();

  std::string comment = arguments_->GetComment();

  switch (arguments_->argc_) {
    case 2:
      return is_any_ongoing
             ? tictac_track::AppError::PrintError(
              "Cannot resume: last entry is still ongoing.")
             // Resume last entry
             : ResumeEntryByIndexOrNegativeOffset(0, comment);
    case 3:
    case 4:if (is_any_ongoing) report.StopEntry();

      return ResumeEntryByIndexOrNegativeOffset(
          arguments_->ResolveNumber(2),
          comment);
    default:return tictac_track::AppError::PrintError("Too many arguments.");
  }
}

// Resume entry.
// If given index is positive: is row index,
// else: negative offset. Append/add given comment
bool App::ResumeEntryByIndexOrNegativeOffset(
    signed int row_index,
    std::string add_to_comment) {
  auto *parser = new ReportParser();

  if (!parser->LoadReportHtml()) {
    delete parser;

    return false;
  }

  // Negative offset: Convert to index
  if (0 >= row_index) {
    row_index = parser->GetLastIndex() + row_index;
  }

  int last_index = parser->GetLastIndex();

  if (row_index > last_index) {
    bool can_resume = false;

    // Check: did user pass a task-number instead of the required ID?
    std::string task_number = helper::Numeric::ToString(row_index);

    int row_index_by_task = parser->GetLatestIndexByTaskNumber(task_number);

    if (row_index_by_task > -1) {
      // Suggest resuming the last entry of that task
      std::cout
          << "Cannot resume entry " << row_index
          << ", last entry is " << last_index << "."
          << " Do you want to resume entry " << row_index_by_task
          << " (last item of task " << row_index
          << ")?\nY/n";

      bool do_resume_by_task = helper::System::GetYesOrNoKeyPress();

      std::cout << "\n";

      if (!do_resume_by_task) {
        delete parser;

        return false;
      }

      can_resume = true;
      row_index = row_index_by_task;
    }

    if (!can_resume) {
      delete parser;

      return tictac_track::AppError::PrintError(
          std::string("Cannot resume entry ")
              .append(helper::Numeric::ToString(row_index))
              .append(", last entry is ")
              .append(helper::Numeric::ToString(last_index))
              .append(".")
              .c_str());
    }
  }

  std::string html = parser->GetHtml();

  int offset_tr =
      tictac_track::ReportParser::GetOffsetTrOpenByIndex(html, row_index);

  std::string task_number = parser->GetColumnContent(
      row_index,
      Report::ColumnIndexes::Index_Issue,
      offset_tr);

  std::string comment_old = parser->GetColumnContent(
      row_index,
      Report::ColumnIndexes::Index_Comment,
      offset_tr);

  std::string comment =
      ReportParser::MergeComments(comment_old, add_to_comment);

  delete parser;

  return ReportCrud::GetInstance().StartEntry(
      comment.c_str(),
      task_number.c_str());
}

// Remove entries
bool App::Remove() {
  auto *parser = new ReportParser();

  if (!parser->LoadReportHtml()) {
    delete parser;

    return false;
  }

  int last_index = parser->GetLastIndex();

  delete parser;

  if (-1 == last_index) {
    return tictac_track::AppError::PrintError(
        "Cannot remove: there are no entries.");
  }

  ReportCrud::GetInstance();

  // No arguments: remove last entry
  if (arguments_->argc_ < 3) {
    return tictac_track::ReportCrud::RemoveEntries(1);
  }

  // Remove all
  if (arguments_->all_) {
    return tictac_track::ReportCrud::Reset();
  }

  if (!arguments_->Contains(2, "=")) {
    if (arguments_->IsNumber(2)) {
      int amount_rows = arguments_->ResolveNumber(2);

      // Negative number: remove given amount of latest entries
      if (amount_rows < 0) {
        return tictac_track::ReportCrud::RemoveEntries(amount_rows * -1);
      }

      // Numeric argument: remove entry w/ given ID
      tictac_track::ReportCrud::RemoveEntryById(amount_rows);
    }
  } else if (helper::String::StartsWith(arguments_->argv_[2], "i=")) {
    // Index-based argument: remove entry w/ given ID
    return tictac_track::ReportCrud::RemoveEntryById(
        arguments_->ResolveNumber(arguments_->argument_index_entry_id_));
  }

  return tictac_track::AppError::PrintError("Removal argument unclear.");
}

// Split entry into two
bool App::Split() {
  int row_index = arguments_->ResolveNumber(2);

  if (-1 == row_index) {
    return tictac_track::AppError::PrintError("No entry ID given.");
  }

  auto *parser = new ReportParser();

  if (!parser->LoadReportHtml()) {
    delete parser;

    return false;
  }

  int last_index = parser->GetLastIndex();

  if (last_index == -1) {
    delete parser;

    return tictac_track::AppError::PrintError(
        "Cannot split: there are no entries yet.");
  }

  if (parser->IsEntryOngoing(row_index)) {
    delete parser;

    return tictac_track::AppError::PrintError(
        "Cannot split: Entry is still ongoing.");
  }

  if (last_index < row_index) {
    delete parser;

    return tictac_track::AppError::PrintError(
        std::string("Cannot split entry ")
            .append(helper::Numeric::ToString(row_index))
            .append(", last entry is ")
            .append(helper::Numeric::ToString(last_index))
            .append(".")
            .c_str());
  }

  std::string split_duration = arguments_->ResolveTime(3, true);

  if (split_duration.empty()) {
    delete parser;

    return tictac_track::AppError::PrintError(
        "Failed parsing entry-duration to split at.");
  }

  if (split_duration[0] == '-') {
    split_duration = split_duration.substr(1, std::string::npos);
  }

  return SplitAtEnd(parser, split_duration, row_index);
}

// Split given entry at given duration before its end into two entries
bool App::SplitAtEnd(
    ReportParser *parser,
    std::string split_duration,
    int row_index) {
  // New entry end is end of entry before reduction
  std::string time_end = parser->GetColumnContent(
      row_index,
      Report::ColumnIndexes::Index_End);

  // Negative duration: Reduce end-time,
  // insert new entry after: w/ start-time same as changed end-time
  if (!parser->ReduceEntryTime(row_index,
                               std::move(split_duration),
                               AppCommand::Commands::Command_Split)) {
    return false;
  }

  std::string html = parser->GetHtml();
  ReportRecalculator::CalculateAndUpdateDuration(html, row_index);
  parser->SetHtml(html);

  int offset_tr =
      tictac_track::ReportParser::GetOffsetTrOpenByIndex(html, row_index);

  // New entry start is end of reduced entry
  std::string time_start = parser->GetColumnContent(
      row_index, Report::ColumnIndexes::Index_End, offset_tr);

  std::string meta = parser->GetColumnContent(
      row_index, Report::ColumnIndexes::Index_Meta, offset_tr);

  std::string week_number = parser->GetColumnContent(
      row_index, Report::ColumnIndexes::Index_Week, offset_tr);

  std::string weekday = parser->GetColumnContent(
      row_index, Report::ColumnIndexes::Index_Day, offset_tr);

  std::string day_date = parser->GetColumnContent(
      row_index, Report::ColumnIndexes::Index_Date, offset_tr);

  std::string task_number = parser->GetColumnContent(
      row_index, Report::ColumnIndexes::Index_Issue, offset_tr);

  return ReportCrud::InsertEntryAfter(
      html,
      row_index,
      meta,
      week_number,
      weekday,
      day_date,
      time_start,
      time_end,
      task_number);
}

// Start new entry or modify start-time of given entry
bool App::Start() {
  // Detect ID out of invocation like: "s 1 0:30"
  if (4 == arguments_->argc_
      && arguments_->IsNumber(2)
      && arguments_->IsTime(3)) {
    arguments_->argument_index_entry_id_ = 2;
  }

  if (-1 != arguments_->argument_index_entry_id_) {
    // Index given: update already existing entry
    return UpdateTime(Report::ColumnIndexes::Index_Start);
  }

  ReportCrud &report = ReportCrud::GetInstance();

  // Stop currently ongoing entry if any
  if (tictac_track::ReportCrud::IsAnyEntryOngoing()) {
    report.StopEntry();
  }

  // No arguments given: Add start entry w/o comment or task number
  if (arguments_->argc_ == 2) {
    return report.StartEntry("", "");
  }

  // Create + start entry
  switch (arguments_->argv_types_[2]) {
    case AppArguments::ArgumentType_Comment:
      // Start w/ comment, task number if given
      return report.StartEntry(
          arguments_->ResolveComment(2).c_str(),
          arguments_->ResolveNumber(3));
    case AppArguments::ArgumentType_Number:
      // Start w/ task number, comment if given
      return report.StartEntry(
          arguments_->ResolveComment(3).c_str(),
          arguments_->ResolveNumber(2));
    case AppArguments::ArgumentType_Invalid:
    case AppArguments::ArgumentType_OptionAll:
    case AppArguments::ArgumentType_OptionIndex:
    case AppArguments::ArgumentType_RenderScope:
    case AppArguments::ArgumentType_Time:
    default:return false;
  }
}

// Add/update end-time to/of entry, append comment if given
bool App::Stop() {
  // Index given: update already existing entry
  if (-1 != arguments_->argument_index_entry_id_) {
    return UpdateTime(Report::ColumnIndexes::Index_End);
  }

  if (tictac_track::ReportCrud::IsAnyEntryOngoing()) {
    ReportCrud &report = ReportCrud::GetInstance();

    return report.StopEntry(
        // TODO(kay): encode comment special chars to HTML entities
        arguments_->ResolveComment(2).c_str());
  }

  return tictac_track::AppError::PrintError(
      "Cannot stop: No entry is currently ongoing.");
}

// Add/Append/Unset comment of latest or entry/entries with given ID/IDs
bool App::UpdateComment() {
  auto *parser = new ReportParser();

  if (!parser->LoadReportHtml()) {
    delete parser;

    return false;
  }

  int last_index = parser->GetLastIndex();

  delete parser;

  if (-1 == last_index) {
    return tictac_track::AppError::PrintError(
        "Cannot update comment: there are no entries.");
  }

  std::vector<int> row_ids{-1};

  int comment_argument_offset = 2;

  if (arguments_->has_multiple_ids_) {
    // Multiple comma-separated indexes given
    row_ids = arguments_->ids_;
    comment_argument_offset = 3;
  } else if (-1 != arguments_->argument_index_entry_id_) {
    // Single index given:
    // find argument-offsets of comment and row-index (allow arbitrary order)
    row_ids[0] = arguments_->ResolveNumber(
        arguments_->argument_index_entry_id_);

    if (row_ids[0] < 0) {
      return tictac_track::AppError::PrintError(
          "Cannot update comment: Index cannot be < 0.");
    }

    if (arguments_->argc_ > 2) {
      if (2 == arguments_->argument_index_entry_id_)
        comment_argument_offset = 3;
    } else {
      comment_argument_offset = -1;
    }
  }

  bool starts_with_space;
  std::string comment;

  if (2 == arguments_->argc_
      || -1 == comment_argument_offset
      || comment_argument_offset >= arguments_->argc_
      ) {
    starts_with_space = false;

    comment = "";
  } else {
    starts_with_space = ' ' == arguments_->argv_[comment_argument_offset][0];
    comment = arguments_->ResolveComment(comment_argument_offset);
  }

  bool res = true;

  for (auto const &index : row_ids) {
    res = UpdateCommentByEntryId(
        last_index, index, comment, starts_with_space) && res;
  }

  return res;
}

bool App::ClearTimesheet() {
  tictac_track::ReportCrud::GetInstance(true);

  return true;
}

bool App::UpdateCommentByEntryId(
    int last_index,
    int index,
    std::string comment,
    bool starts_with_space) {
  if (index > last_index) {
    return tictac_track::AppError::PrintError(
        std::string("Cannot update comment of entry ")
            .append(helper::Numeric::ToString(index))
            .append(", last index is: ")
            .append(helper::Numeric::ToString(last_index))
            .c_str());
  }

  return comment.empty()
         ? ReportParser::UpdateColumn(index, Report::Index_Comment, "")
         : ReportCrud::AppendComment(
            comment,
            index,
            starts_with_space);
}

bool App::UpdateTaskNumber() {
  int task_argument_offset = 2;

  std::vector<int> row_ids{-1};

  if (arguments_->has_multiple_ids_) {
    // Multiple comma-separated indexes given
    row_ids = arguments_->ids_;

    task_argument_offset = arguments_->argument_index_entry_id_ == 2
        ? 3
        : 2;
  } else if (arguments_->argument_index_entry_id_ != -1) {
    // Index given:
    // find argument-offsets of task no. and row-index (allow arbitrary order)
    row_ids[0] =
        arguments_->ResolveNumber(arguments_->argument_index_entry_id_);

    if (row_ids[0] < 0) {
      return tictac_track::AppError::PrintError("Entry index cannot be < 0.");
    }

    task_argument_offset = arguments_->argument_index_entry_id_ == 2 ? 3 : 2;
  }

  int task_number = arguments_->ResolveNumber(task_argument_offset);

  ReportCrud::GetInstance();

  int argument_index_comment = GetCommentArgOffsetInTaskCommand();

  std::string comment = argument_index_comment > -1
    ? arguments_->ResolveComment(argument_index_comment)
    : "";

  bool has_comment = !comment.empty();

  bool comment_starts_with_space =
      has_comment
          && ' ' == arguments_->argv_[argument_index_comment][0];

  bool res = true;

  for (auto const &index : row_ids) {
    res = tictac_track::ReportCrud::UpdateIssueNumber(task_number, index)
        && res;

    if (has_comment) {
      tictac_track::ReportCrud::AppendComment(
          comment,
          index,
          comment_starts_with_space);
    }
  }

  return res;
}

int App::GetCommentArgOffsetInTaskCommand() const {
  // Get optional comment
  // from invocation like: "t 1234 comment", or: "t 1234 comment"
  if (4 == arguments_->argc_
      && !helper::String::IsNumeric(arguments_->argv_[3])) {
    return 3;
  }

  // Get optional comment from invocation
  // like: "t i=1 1234 comment", or: "t i=1,2,3 1234 comment"
  if (5 == arguments_->argc_
      && !helper::String::IsNumeric(arguments_->argv_[4])) {
    return 4;
  }

  return -1;
}

// Update time of row + column by arguments
bool App::UpdateTime(Report::ColumnIndexes column_index) {
  if (arguments_->argc_ < 4) {
    return tictac_track::AppError::PrintError(
        "To few arguments: missing time argument in format: \"hh:mm\".");
  }

  int row_index =
      arguments_->ResolveNumber(arguments_->argument_index_entry_id_);

  if (row_index < 0) {
    return tictac_track::AppError::PrintError("Entry index cannot be < 0.");
  }

  std::string time;

  if (-1 == arguments_->argument_index_time_) {
    // Fallback: Try find index of time-argument now:
    // if entry-index is argument 2,
    // than time is 3, if entry-index is 3- time is 2.
    arguments_->argument_index_time_ =
        arguments_->argument_index_entry_id_ == 2 ? 3 : 2;
  }

  if (-1 != arguments_->argument_index_time_) {
    time = arguments_->ResolveTime(arguments_->argument_index_time_);

    if (!time.empty()) {
      // Start-time is allowed to be > end-time,
      // it is than interpreted as if the entry spans over midnight
      if (!ReportParser::UpdateColumn(row_index, column_index, time)) {
        return tictac_track::AppError::PrintError(
            std::string("Update column failed (")
                .append(helper::Numeric::ToString(column_index))
                .append(")").c_str());
      }

      const std::string &html =
          ReportRecalculator::CalculateAndUpdateDuration(row_index);

      ReportFile::SaveReport(const_cast<std::string &>(html));

      return true;
    }
  }

  return tictac_track::AppError::PrintError("Failed parsing arguments.");
}

// Pretty-print report to CLI
bool App::View() {
  ReportRendererCli renderer;

  AppConfig config = AppConfig::GetInstance();

  if (config.GetConfigValue("clear_before_view") == "1") {
    helper::Tui::ClearConsole();
  }

  return renderer.PrintToCli(
      static_cast<ReportRendererCli::RenderScopes>(arguments_->render_scope_),
      arguments_->GetNegativeNumber(),
      arguments_->GetTaskNumber(),
      arguments_->GetComment());
}

bool App::ViewWeek() {
  ReportRendererCli renderer;

  AppConfig config = AppConfig::GetInstance();

  if (config.GetConfigValue("clear_before_view") == "1") {
    helper::Tui::ClearConsole();
  }

  return renderer.PrintToCli(
      Report::RenderScopes::Scope_Week, arguments_->GetNegativeNumber(),
      arguments_->GetTaskNumber(), arguments_->GetComment());
}

bool App::CsvRecentTaskNumbers() {
  auto *parser = new ReportParser();
  parser->LoadReportHtml();
  int amount_rows = parser->GetAmountRows();

  std::string task_numbers = "|";

  // Iterate over tracked rows backwards, starting with latest
  bool is_first = true;
  int amount_task_numbers_found = 0;

  for (int row_offset = 0;
       amount_task_numbers_found < 30 && row_offset < amount_rows;
       row_offset++) {
    std::string task_number = parser->GetLatestIssueNumber(row_offset);

    if (helper::String::IsNumeric(task_number)
        && !helper::String::Contains(task_numbers, task_number)) {
      if (!is_first) {
        std::cout << ",";
      }

      std::cout << task_number;

      amount_task_numbers_found++;

      task_numbers.append(task_number).append("|");

      if (is_first) {
        is_first = false;
      }
    }
  }

  delete parser;

  return true;
}

bool App::CsvTodayTracks() {
  ReportRendererCsv renderer;

  return renderer.RenderToStdOut(
      static_cast<ReportRendererCli::RenderScopes>(arguments_->render_scope_));
}

// View entries sequentially, opening related task URL at the same time
// @see AppConfig::GetDefaultConfig()   Base URL, e.g. redmine is set within ini
bool App::BrowseDayTasks() {
  ReportRendererCli renderer;

  AppConfig config = AppConfig::GetInstance();

  if (config.GetConfigValue("clear_before_view") == "1") {
    helper::Tui::ClearConsole();
  }

  return renderer.PrintBrowseDayTasks(arguments_->GetNegativeNumber());
}

}  // namespace tictac_track
