
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <lib/helper/helper_date_time.h>
#include <apps/redmine-sync/report/report_file.h>
#include "app.h"
#include "app_config.h"

namespace tictac_rms {

// Meta constants
const std::string App::kAppName = "ttt-redmine_sync";
const std::string App::kAppExecutableName = "ttt-redmine_sync";
const std::string App::kAppVersion = "v1.0";

/**
 * Constructor
 */
App::App(int argc, char **argv) {
  //arguments_ = new AppArguments(argc, argv, *command_);
}

/**
 * Process
 */
bool App::Process() {
  std::cout << "Today is: " + helper::DateTime::GetCurrentTimeFormatted("%Y-%m-%d") + "\n";

  AppConfig config = tictac_rms::AppConfig::GetInstance();

  std::string date_synced_until = config.GetConfigValue("date_synced_until");
  if ("-1" == date_synced_until) date_synced_until = "not yet (using this sync tool)";
  std::cout << "Entries have been synced to Redmine until: " + date_synced_until + ".\n";

  std::string report_html = ReportFile::GetReportHtml();

  std::string date_first_entry = "";
  std::string date_last_entry = "";
  std::cout << "The given timesheet contains entries from" + date_first_entry + " to " + date_last_entry + ".\n";

  std::cout << "\n";

  // Check file: until what date was synced last time?

  // Ask for start/end dates to be synced

  // load report, iterate over days/tasks
  // per task/day: suggest comment and time-sum
  // store to redmine

  return true;
}
} // namespace tictac_rms
