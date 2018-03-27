
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_APP_ARGUMENTS
#define CLASS_TIMETRACKERCL_APP_ARGUMENTS

#include <vector>
#include "commands.h"
namespace timesheetplus {

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
  // Was a negative numeric argument given, e.g. "-1" (used e.g. in "v d -1" = view previous day)
  int argument_index_negative_number_ = -1;
  // Index of comment argument
  int argument_index_comment_ = -1;
  int argument_index_time_ = -1;

  // Multiple comma-separated IDs, if given stored as ints
  std::vector<int> ids_{-1};
  bool has_multiple_ids_ = false;

  // Constructor
  AppArguments(int argc, char **argv, AppCommand &command);

  bool IsComment(int index);
  bool IsNumber(int index);
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
  // Validate CSV path from argument at given index
  std::string ResolvePathCsv(int index);
  // Validate command-name from argument at given index
  AppCommand::Commands ResolveCommandName(int index);

 private:
  //AppCommand *command_;

  // Set attributes from arguments
  void Resolve(AppCommand &command);

  void SetArgvDefaultTypeByCommand(AppCommand &command, int index);

  // Argument value as received from CLI, that is w/ "c="-prefix if given
  //std::string argv_;
  //bool is_comment_;
};
} // namespace timesheetplus

#endif
