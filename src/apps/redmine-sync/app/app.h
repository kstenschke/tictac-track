
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_APP
#define CLASS_TIMETRACKERCL_APP

//#include "app_commands.h"
//#include "app_arguments.h"
//#include "apps/client/report/report_html_parser.h"
//#include "../report/report.h"

namespace tictac_rms {

  class App
  {
    public:
      // Meta constants
      static const std::string kAppName;
      static const std::string kAppExecutableName;
      static const std::string kAppVersion;

      // Process command + arguments
      static bool Process();

      // Get absolute path to executable binary
      static std::string GetBinaryPath(char** argv);

    private:
      AppCommand *command_;
      AppArguments *arguments_;
  };
} // namespace tictac_rms

#endif
