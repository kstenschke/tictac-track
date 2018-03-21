
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <iostream>
#include <cstring>
#include "arguments.h"
#include "../helper/helper_string.h"
#include "../helper/html.h"
#include "../report/renderer_cli.h"

namespace timesheetplus {

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

    if (HelperString::StartsWith(argv_[i], "i=")) {
      argument_index_entry_id_ = i;
      if (!HelperString::Contains(argv_[i], ",")) {
        // Single index given
        argv_types_[i] = ArgumentType_Number;
        continue;
      } else {
        // Multiple comma-separated indexes given - parse and store as vector of integers
        has_multiple_ids_ = true;
        std::vector<std::string> entry_indexes_str = HelperString::Explode(std::string(argv_[i]).substr(2), ',');
        ids_.assign(entry_indexes_str.size(), -1);
        int j = 0;
        for(auto const &index: entry_indexes_str) {
          ids_[j] = HelperString::ToInt(index);
          j++;
        }
      }
    }

    if (HelperString::StartsWith(argument.c_str(), "t=") || HelperString::IsNumeric(argument, true)) {
      argv_types_[i] = ArgumentType_Number;
      if ('-' == argument[0]) {
        argument_index_negative_number_ = i;
        continue;
      }
      argument_index_task_number_ = i;
      continue;
    }
    if (HelperString::StartsWith(argv_[i], "c=") || HelperString::StartsWith(argv_[i], "comment=")) {
      argv_types_[i] = ArgumentType_Comment;
      argument_index_comment_ = i;
      continue;
    }

    // Argument is not numeric, no command or command shortcut
    SetArgvDefaultTypeByCommand(command, i);
  }
}

void AppArguments::SetArgvDefaultTypeByCommand(AppCommand &command, int index) {
  switch (command.GetResolved()) {
    case AppCommand::Command_Day:
    case AppCommand::Command_Help:
    case AppCommand::Command_Remove:return;
    case AppCommand::Command_Comment:
    case AppCommand::Command_Start:
    case AppCommand::Command_Stop:
      argv_types_[index] = ArgumentType_Comment;
      argument_index_comment_ = index;
      return;
    case AppCommand::Command_Version:return;
    case AppCommand::Command_View:return;
    case AppCommand::Command_Invalid:
    default:argv_types_[index] = ArgumentType_Invalid;
      return;
  }
}

bool AppArguments::IsComment(int index) {
  return index >= argc_ ? false : argv_types_[index] == ArgumentType_Comment;
}

bool AppArguments::IsNumber(int index) {
  return index >= argc_ ? false : argv_types_[index] == ArgumentType_Number;
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
  return index >= argc_ ? false : HelperString::Contains(argv_[index], needle);
}

/**
 * Get numeric value of number-argument (can be prefixed w/ e.g. "t=")
 */
int AppArguments::ResolveNumber(int index) {
  if (index >= argc_) return -1;

  if ('-' == argv_[index][0]) {
    // Is negative
    std::string numeric = std::string(argv_[index]).substr(1, std::string::npos);
    return -1 * HelperString::ToInt(numeric.c_str());
  }

  if (HelperString::StartsWith(argv_[index], "i=") || HelperString::StartsWith(argv_[index], "t=")) {
    std::string numeric = std::string(argv_[index] + 2, std::strlen(argv_[index]));
    return HelperString::ToInt(numeric.c_str());
  }

  return HelperString::IsNumeric(argv_[index])
         ? HelperString::ToInt(argv_[index])
         : HelperString::ToInt(std::string(argv_[index] + 5, std::strlen(argv_[index])).c_str());
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
  if (HelperString::StartsWith(argv_[index], "c=")) {
    comment = std::string(argv_[index] + 2, strlen(argv_[index]) - 2);
  } else if (HelperString::StartsWith(argv_[index], "comment=")) {
    comment = std::string(argv_[index] + 8, strlen(argv_[index]) - 8);
  } else {
    comment = HelperHtml::Encode(argv_[index]);
  }

  if (!comment.empty()) HelperString::Trim(comment);

  return comment;
}

/**
 * Validate CSV path from argument at given index
 */
std::string AppArguments::ResolvePathCsv(int index) {
  std::string pathCsv;
  if (index >= argc_) return pathCsv;

  if (HelperString::Contains(argv_[index], " ")) {
    std::cout << "Error: Space is not allowed in CSV path.\n";
    return pathCsv;
  }

  pathCsv = std::string(argv_[index]);
  return pathCsv + (HelperString::EndsWith(pathCsv, ".csv") ? "" : ".csv");
}

/**
 * Validate command-name from argument at given index
 */
AppCommand::Commands AppArguments::ResolveCommandName(int index) {
  AppCommand::Commands command = AppCommand::Commands::Command_Invalid;
  if (index >= argc_) return command;

  return AppCommand::ResolveCommandByName(argv_[index]);
}

} // namespace timesheetplus
