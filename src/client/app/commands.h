
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_APP_COMMAND
#define CLASS_TIMETRACKERCL_APP_COMMAND

namespace tictac_track {

class AppCommand {
 public:
  enum Commands {
    Command_Browse,
    Command_Comment,
    Command_Csv,
    Command_Day,
    Command_Date,
    Command_ExternalTaskUrl,
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
    Command_Invalid
  };

  // Constructor
  explicit AppCommand(std::string argc);

  static Commands ResolveCommandByName(std::string command);

  Commands GetResolved();

 private:
  // Argument value as received from CLI
  std::string argc_;

  // Argument resolved to Commands-enum
  Commands resolved_;

  // Resolve command string (1st argument) to related command enum item (which allows e.g. switch)
  Commands Resolve();

};
} // namespace tictac_track

#endif
