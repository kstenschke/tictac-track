
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "app/config.h"
#include "report/crud.h"
#include "app/app.h"

/**
  * @param argc Amount of arguments received
  * @param argv Array of arguments received, argv[0] is name and path of executable
  */
int main(int argc,  char** argv) {
   // Ensure config and timesheet HTML files exist
   timesheetplus::AppConfig::GetInstance(argv);
   timesheetplus::ReportCrud report_crud = timesheetplus::ReportCrud::GetInstance();
   if (!report_crud.ReportExists()) {
     return -1;
   }

   // Process command arguments, display help if no valid command given
   auto * app = new timesheetplus::App(argc, argv);
   if (argc > 1) app->Process();

   return 0;
}
