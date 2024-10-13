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

#include <ttt/class/app/app_commands.h>
#include <algorithm>

namespace tictac_track {

AppCommand::AppCommand(std::string argc) : argc_(std::move(argc)) {
  resolved_ = Resolve();
}

AppCommand::Commands AppCommand::GetResolved() {
  return resolved_;
}

// Resolve name of command string (1st argument)
// to related enum item (which allows e.g. switch)
AppCommand::Commands AppCommand::Resolve() {
  if (argc_.length() > 1) {
    // accept multi-character commands case-insensitive
    std::transform(argc_.begin(), argc_.end(), argc_.begin(), ::tolower);
  }

  return ResolveCommandByName(argc_);
}

AppCommand::Commands AppCommand::ResolveCommandByName(
    const std::string &command) {
  // Commands are sorted by assumed average usage frequency
  if (command == "w" || command == "week") {
    return Command_ViewWeek;
  }

  if (command == "s" || command == "start") {
    return Command_Start;
  }

  if (command == "p" || command == "pause" || command == "stop") {
    return Command_Stop;
  }

  if (command == "r" || command == "resume") {
    return Command_Resume;
  }

  if (command == "c" || command == "comment") {
    return Command_Comment;
  }

  if (command == "t" || command == "task") {
    return Command_Task;
  }

  if (command == "sp" || command == "split") {
    return Command_Split;
  }

  if (command == "m" || command == "merge") {
    return Command_Merge;
  }

  if (command == "d" || command == "day") {
    return Command_Day;
  }

  if (command == "v" || command == "view") {
    return Command_View;
  }

  if (command == "W" || command == "calendarweek") {
    return Command_DisplayCalendarWeek;
  }

  if (command == "D" || command == "date") {
    return Command_DisplayDate;
  }

  if (command == "h" || command == "help") {
    return Command_Help;
  }

  if (command == "rc" || command == "recalculate") {
    return Command_Recalculate;
  }

  if (command == "rm" || command == "remove") {
    return Command_Remove;
  }

  if (command == "z" || command == "undo") {
    return Command_Undo;
  }

  if (command == "V" || command == "version") {
    return Command_Version;
  }

  if (command == "b" || command == "browse") {
    return Command_BrowseTimesheet;
  }

  if (command == "u" || command == "url") {
    return Command_BrowseTaskUrl;
  }

  if (command == "ud" || command == "daytasks") {
    return Command_BrowseDayTasks;
  }

  if (command == "csvdt") {
    return Command_CsvDayTracks;
  }

  if (command == "csvrtn") {
    return Command_CsvRecentTaskNumbers;
  }

  if (command == "csv") {
    return Command_Csv;
  }

  if (command == "cls" || command == "clear") {
    return Command_ClearTimesheet;
  }

  return Command_Invalid;
}

}  // namespace tictac_track
