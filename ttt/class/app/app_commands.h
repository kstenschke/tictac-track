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

#ifndef TTT_CLASS_APP_APP_COMMANDS_H_
#define TTT_CLASS_APP_APP_COMMANDS_H_

#include <string>
#include <utility>

namespace tictac_track {

class AppCommand {
 public:
  enum Commands {
    Command_BrowseTimesheet,
    Command_BrowseTaskUrl,
    Command_DisplayCalendarWeek,
    Command_ClearTimesheet,
    Command_Comment,
    Command_Csv,
    Command_CsvRecentTaskNumbers,
    Command_CsvDayTracks,
    Command_Day,
    Command_DisplayDate,
    Command_Help,
    Command_Merge,
    Command_Recalculate,
    Command_Resume,
    Command_Remove,
    Command_Split,
    Command_Start,
    Command_Stop,
    Command_Task,
    Command_Undo,
    Command_Version,
    Command_View,
    Command_ViewWeek,
    Command_BrowseDayTasks,
    Command_Invalid
  };

  // Constructor
  explicit AppCommand(std::string argc);

  static Commands ResolveCommandByName(const std::string &command);

  Commands GetResolved();

 private:
  // Argument value as received from CLI
  std::string argc_;

  // Argument resolved to Commands-enum
  Commands resolved_;

  // Resolve command string (1st argument)
  // to related command enum item (which allows e.g. switch)
  Commands Resolve();
};

}  // namespace tictac_track

#endif  // TTT_CLASS_APP_APP_COMMANDS_H_
