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

#ifndef TTT_CLASS_APP_APP_ARGUMENTS_H_
#define TTT_CLASS_APP_APP_ARGUMENTS_H_

#include <ttt/class/app/app_commands.h>

#include <ttt/helper/helper_date_time.h>
#include <ttt/helper/helper_html.h>
#include <ttt/helper/helper_string.h>

#include <string>
#include <cstring>
#include <vector>

namespace tictac_track {

class AppArguments {
 public:
  enum ArgumentTypes {
    ArgumentType_Comment,
    // Task- or other (e.g. amount) number
        ArgumentType_Number,
    ArgumentType_RenderScope,
    ArgumentType_Time,
    ArgumentType_OptionAll,
    ArgumentType_OptionIndex,
    ArgumentType_Invalid
  };

  int argc_;
  char **argv_;

  int max_arguments_ = 10;
  ArgumentTypes argv_types_[10];

  int render_scope_ = 0;
  bool all_ = false;
  bool indexed_ = false;

  // Was index argument, e.g. "i=1", given at all/which argument-index?
  int argument_index_entry_id_ = -1;

  // Was task-number argument, e.g. "t=1", given at all/which argument-index?
  int argument_index_task_number_ = -1;

  // Was a negative numeric argument given,
  // e.g. "-1" (used e.g. in "v d -1" = view previous day)
  int argument_index_negative_number_ = -1;

  // Index of comment argument
  int argument_index_comment_ = -1;

  int argument_index_time_ = -1;

  // Multiple comma-separated IDs, if given stored as ints
  std::vector<int> ids_{-1};

  bool has_multiple_ids_ = false;

  // Constructor
  AppArguments(int argc, char **argv, AppCommand &command);

  bool IsNumber(int index);
  bool IsTime(int index);
  bool Contains(int index, std::string needle);

  std::string GetComment();
  int GetNegativeNumber();
  int GetTaskNumber();

  // Get numeric value of number-argument (can be prefixed w/ e.g. "t=")
  int ResolveNumber(int index);
  // Validate given time-string in format "hh:mm", or if allowed "-hh:mm"
  std::string ResolveTime(int index, bool allow_negative = false);
  // Remove possible prefix from comment ("c=" or "comment=")
  std::string ResolveComment(int index);
  // Validate command-name from argument at given index
  AppCommand::Commands ResolveCommandName(int index);

 private:
  // Set attributes from arguments
  void Resolve(AppCommand &command);

  bool ResolveAsComment(int i);

  // Try resolve i=<number> or i=<number,number,...>
  bool ResolveAsIndex(int i);
  bool ResolveAsTime(int i);
  void ResolveAsTaskIndex(
      int i,
      const std::string &argument,
      const AppCommand::Commands &command_resolved,
      bool is_numeric);

  void SetArgvDefaultTypeByCommand(AppCommand &command, int index);
};

}  // namespace tictac_track

#endif  // TTT_CLASS_APP_APP_ARGUMENTS_H_
