
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_APP
#define CLASS_TIMETRACKERCL_APP

#include <string>
//#include "apps/client/report/report_html_parser.h"
//#include "../report/report.h"

namespace tictac_rms {
class App {
 public:
  // Meta constants
  static const std::string kAppName;
  static const std::string kAppExecutableName;
  static const std::string kAppVersion;

  // Constructor
  App(int argc,  char** argv);

  // Process command + arguments
  bool Process();
};
} // namespace tictac_rms

#endif
