/*
  Copyright (c) 2018, Kay Stenschke
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
#include "app_arguments.h"
#include "helper/helper_string.h"
#include "helper/helper_html.h"
#include "report/report_renderer_cli.h"
#include "helper/helper_date_time.h"

namespace tictac_track {

/**
 * Constructor
 */
AppArguments::AppArguments(int argc, char **argv, AppCommand &command) {
  argc_ = argc;
  argv_ = argv;

  if (argc_ > 1) Resolve(command);
}

/**
 * Resolve arguments to attributes
 */
void AppArguments::Resolve(AppCommand &command) {
  for (int i = 2; i < argc_; ++i) {
    if (i == max_arguments_) break;

    std::string argument = argv_[i];

    if (argument == "a" || argument == "all") {
      argv_types_[i] = ArgumentType_OptionAll;
      all_ = true;
      continue;
    }
    if (argument == "d" || argument == "day") {
      argv_types_[i] = ArgumentType_RenderScope;
      render_scope_ = ReportRendererCli::Scope_Day;
      continue;
    }
    if (argument == "i" || argument == "indexed") {
      // View w/ indexes
      argv_types_[i] = ArgumentType_RenderScope;
      indexed_ = true;
      continue;
    }
    if (argument == "m" || argument == "month") {
      argv_types_[i] = ArgumentType_RenderScope;
      render_scope_ = ReportRendererCli::Scope_Month;
      continue;
    }
    if (argument == "w" || argument == "week") {
      argv_types_[i] = ArgumentType_RenderScope;
      render_scope_ = ReportRendererCli::Scope_Week;
      continue;
    }

    // Try resolve i=<number> or i=<number,number,...>
    if (helper::String::StartsWith(argv_[i], "i=") && ResolveAsIndex(i)) continue;

    if (helper::DateTime::IsTime(argument) && ResolveAsTime(i)) continue;

    AppCommand::Commands command_resolved = command.GetResolved();
    bool is_numeric = helper::String::IsNumeric(argument, true);

    if (command_resolved == AppCommand::Command_Stop && is_numeric && -1 == argument_index_entry_id_) {
        argv_types_[i] = ArgumentType_Number;
        argument_index_entry_id_ = i;
        continue;
    }

    if (helper::String::StartsWith(argument.c_str(), "t=") || is_numeric) {
      ResolveAsTaskIndex(i, argument, command_resolved, is_numeric);
      continue;
    }
    if (((helper::String::StartsWith(argv_[i], "c=") || helper::String::StartsWith(argv_[i], "comment="))
        || (command_resolved == AppCommand::Command_Resume && -1 == argument_index_comment_))
          && ResolveAsComment(i)) continue;

    // Argument is not numeric, no command or command shortcut
    SetArgvDefaultTypeByCommand(command, i);
  }
}

bool AppArguments::ResolveAsComment(int i) {
  argv_types_[i] = ArgumentType_Comment;
  argument_index_comment_ = i;

  return true;
}

void AppArguments::ResolveAsTaskIndex(int i, const std::string &argument, const AppCommand::Commands &command_resolved,
                                      bool is_numeric) {
  argv_types_[i] = ArgumentType_Number;
  if ('-' == argument[0]) {
    argument_index_negative_number_ = i;
    return;
  }
  if (command_resolved == AppCommand::Command_Comment && is_numeric && -1 == argument_index_entry_id_) {
    argument_index_entry_id_ = i;
    return;
  }
  argument_index_task_number_ = i;
}

bool AppArguments::ResolveAsTime(int i) {
  argv_types_[i] = ArgumentType_Time;
  argument_index_time_ = i;

  return true;
}

/**
 * Try resolve i=<number> or i=<number,number,...>
 */
bool AppArguments::ResolveAsIndex(int i) {
  argument_index_entry_id_ = i;
  if (!helper::String::Contains(argv_[i], ",")) {
    // Single index given
    argv_types_[i] = ArgumentType_Number;
    return true;
  }

  // Multiple comma-separated indexes given - parse and store as vector of integers
  has_multiple_ids_ = true;
  std::vector<std::string> entry_indexes_str = helper::String::Explode(std::string(argv_[i]).substr(2), ',');
  ids_.assign(entry_indexes_str.size(), -1);
  int j = 0;
  for(auto const &index: entry_indexes_str) {
    ids_[j] = helper::String::ToInt(index);
    j++;
  }

  return false;
}

void AppArguments::SetArgvDefaultTypeByCommand(AppCommand &command, int index) {
  switch (command.GetResolved()) {
    case AppCommand::Command_Day:
    case AppCommand::Command_Help:
    case AppCommand::Command_Remove:
      return;
    case AppCommand::Command_Comment:
    case AppCommand::Command_Start:
    case AppCommand::Command_Stop:
      argv_types_[index] = ArgumentType_Comment;
      argument_index_comment_ = index;
      return;
    case AppCommand::Command_Version:
      return;
    case AppCommand::Command_View:
      return;
    case AppCommand::Command_Invalid:
    default:argv_types_[index] = ArgumentType_Invalid;
      return;
  }
}

bool AppArguments::IsComment(int index) {
  return index < argc_ && argv_types_[index] == ArgumentType_Comment;
}

bool AppArguments::IsNumber(int index) {
  return index < argc_ && argv_types_[index] == ArgumentType_Number;
}

bool AppArguments::IsTime(int index) {
  return index < argc_ && helper::DateTime::IsTime(argv_[index]);
}

std::string AppArguments::GetComment() {
  return argument_index_comment_ == -1 ? "" : ResolveComment(argument_index_comment_);
}

int AppArguments::GetNegativeNumber() {
  return argument_index_negative_number_ == -1 ? 0 : ResolveNumber(argument_index_negative_number_);
}

int AppArguments::GetTaskNumber() {
  return argument_index_task_number_ == -1 ? -1 : ResolveNumber(argument_index_task_number_);
}

/**
 * Was the argument given w/ an identifier prefix? e.g. "c=", "i=", etc.
 */
bool AppArguments::Contains(int index, std::string needle) {
  return index < argc_ && helper::String::Contains(argv_[index], needle);
}

/**
 * Get numeric value of number-argument (can be prefixed w/ e.g. "t=")
 */
int AppArguments::ResolveNumber(int index) {
  if (index >= argc_) return -1;

  if ('-' == argv_[index][0]) {
    // Is negative
    std::string numeric = std::string(argv_[index]).substr(1, std::string::npos);
    return -1 * helper::String::ToInt(numeric.c_str());
  }

  if (helper::String::StartsWith(argv_[index], "i=") || helper::String::StartsWith(argv_[index], "t=")) {
    std::string numeric = std::string(argv_[index] + 2, std::strlen(argv_[index]));
    return helper::String::ToInt(numeric.c_str());
  }

  return helper::String::IsNumeric(argv_[index])
         ? helper::String::ToInt(argv_[index])
         : helper::String::ToInt(std::string(argv_[index] + 5, std::strlen(argv_[index])).c_str());
}

/**
 * Validate given time-string in format "hh:mm", or if allowed: "-hh:mm"
 */
std::string AppArguments::ResolveTime(int index, bool allow_negative) {
  if (index >= argc_) return "";

  std::string value = std::string(argv_[index]);

  bool is_negative = allow_negative && value[0] == '-';
  if (is_negative) value = value.substr(1, std::string::npos);

  size_t offset_colon = value.find(':');
  if (std::string::npos == offset_colon || (value.size() - 1) == offset_colon) return "";

  std::string hours = value.substr(0, offset_colon);
  std::string minutes = value.substr(offset_colon + 1, std::string::npos);

  std::string zero = "0";
  while (hours.size() < 2) hours = zero.append(hours);
  while (minutes.size() < 2) minutes = zero.append(minutes);

  return (is_negative ? "-" : "") + hours + ":" + minutes;
}

/**
 * Remove possible prefix from comment ("c=" or "comment=")
 */
std::string AppArguments::ResolveComment(int index) {
  if (index >= argc_) return "";

  std::string comment;
  if (helper::String::StartsWith(argv_[index], "c=")) {
    comment = std::string(argv_[index] + 2, strlen(argv_[index]) - 2);
  } else if (helper::String::StartsWith(argv_[index], "comment=")) {
    comment = std::string(argv_[index] + 8, strlen(argv_[index]) - 8);
  } else {
    comment = helper::Html::Encode(argv_[index]);
  }

  if (!comment.empty()) helper::String::Trim(comment);

  return comment;
}

/**
 * Validate CSV path from argument at given index
 */
std::string AppArguments::ResolvePathCsv(int index) {
  std::string pathCsv;
  if (index >= argc_) return pathCsv;

  if (helper::String::Contains(argv_[index], " ")) {
    std::cout << "Error: Space is not allowed in CSV path.\n";
    return pathCsv;
  }

  pathCsv = std::string(argv_[index]);
  return pathCsv + (helper::String::EndsWith(pathCsv, ".csv") ? "" : ".csv");
}

/**
 * Validate command-name from argument at given index
 */
AppCommand::Commands AppArguments::ResolveCommandName(int index) {
  AppCommand::Commands command = AppCommand::Commands::Command_Invalid;
  if (index >= argc_) return command;

  return AppCommand::ResolveCommandByName(argv_[index]);
}

} // namespace tictac_lib
