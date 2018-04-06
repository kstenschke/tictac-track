
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "app.h"

namespace tictac_rms {

// Meta constants
const std::string App::kAppName = "tictac-rms";
const std::string App::kAppExecutableName = "rms";
const std::string App::kAppVersion = "v1.0";

/**
 * Process
 */
static bool App::Process() {
  // Check file: until what date was synced last time?

  // Ask for start/end dates to be synced

  // load report, iterate over days/tasks
  // per task/day: suggest comment and time-sum
  // store to redmine

  return true;
}
} // namespace tictac_track
