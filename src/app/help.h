
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_APP_HELP
#define CLASS_TIMETRACKERCL_APP_HELP

#include "commands.h"
namespace timesheetplus {

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
} // namespace timesheetplus

#endif
