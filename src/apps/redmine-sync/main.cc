
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <apps/redmine-sync/app/app.h>
#include <apps/redmine-sync/report/report_file.h>
#include "lib/app/app_error.h"
#include "apps/redmine-sync/app/app_config.h"

/**
  * @param argc Amount of arguments received
  * @param argv Array of arguments received, argv[0] is name and path of executable
  */
int main(int argc,  char** argv) {
   // Ensure config file exists
   tictac_rms::AppConfig::GetInstance(argv);

   if (!tictac_rms::ReportFile::ReportExists()) {
     // Ensure timesheet file exists
     std::string report_file_path = tictac_rms::ReportFile::getReportFilePath();
     const char* message = std::string("Error: timesheet file not found (path: " + report_file_path + ").").c_str();
     return tictac_lib::AppError::PrintError(message);
   }

  (new tictac_rms::App(argc, argv))->Process();

   return 0;
}
