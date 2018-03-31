
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include "commands.h"

namespace timesheetplus {

/**
 * Constructor
 */
AppCommand::AppCommand(std::string argc) {
  argc_ = std::move(argc);
  resolved_ = Resolve();
}

AppCommand::Commands AppCommand::GetResolved() {
  return resolved_;
}

/**
 * Resolve name of command string (1st argument) to related enum item (which allows e.g. switch)
 */
AppCommand::Commands AppCommand::Resolve() {
  return ResolveCommandByName(argc_);
}

AppCommand::Commands AppCommand::ResolveCommandByName(std::string command) {
  if (command == "b" || command == "browse") return Command_Browse;
  if (command == "c" || command == "comment") return Command_Comment;
  if (command == "csv") return Command_Csv;
  if (command == "d" || command == "day") return Command_Day;
  if (command == "D" || command == "date") return Command_Date;
  if (command == "h" || command == "help") return Command_Help;
  if (command == "m" || command == "merge") return Command_Merge;
  if (command == "r" || command == "resume") return Command_Resume;
  if (command == "rc" || command == "recalculate") return Command_Recalculate;
  if (command == "rm" || command == "remove") return Command_Remove;
  if (command == "sp" || command == "split") return Command_Split;
  if (command == "s" || command == "start") return Command_Start;
  if (command == "p" || command == "pause" || command == "stop") return Command_Stop;
  if (command == "t" || command == "task") return Command_Task;
  if (command == "z" || command == "undo") return Command_Undo;
  if (command == "V" || command == "version") return Command_Version;
  if (command == "v" || command == "view") return Command_View;
  if (command == "w" || command == "week") return Command_ViewWeek;
  if (command == "u" || command == "url") return Command_ExternalTaskUrl;

  return Command_Invalid;
}

} // namespace timesheetplus
