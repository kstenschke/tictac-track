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

#ifndef CLASS_TTT_APP_HELP
#define CLASS_TTT_APP_HELP

#include "app_commands.h"
namespace tictac_track {

class AppHelp {
 public:
  // Print version information
  static void PrintVersion();

  // Print usage instructions
  static bool PrintHelp(bool withTitle, AppCommand::Commands command);

  // Print message after unknown command, prepend help
  static void PrintUnknownArgumentMessage(const char *arg);

 private:
  static void PrintOverview();

  static bool PrintHelpOnBrowse();
  static bool PrintHelpOnBrowseDayTasks();
  static bool PrintHelpOnCsv();
  static bool PrintHelpOnComment();
  static bool PrintHelpOnDate();
  static bool PrintHelpOnDay();
  static bool PrintHelpOnExternalTaskUrl();
  static bool PrintHelpOnHelp();
  static bool PrintHelpOnMerge();
  static bool PrintHelpOnRecalculate();
  static bool PrintHelpOnResume();
  static bool PrintHelpOnRemove();
  static bool PrintHelpOnSplit();
  static bool PrintHelpOnStart();
  static bool PrintHelpOnStop();
  static bool PrintHelpOnTask();
  static bool PrintHelpOnUndo();
  static bool PrintHelpOnView();
  static bool PrintHelpOnWeek();
};

} // namespace tictac_track

#endif
