
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "apps/client/app/app_config.h"
#include "apps/client/report/report_crud.h"
#include "app/app.h"

/**
  * @param argc Amount of arguments received
  * @param argv Array of arguments received, argv[0] is name and path of executable
  */
int main(int argc,  char** argv) {
   // Ensure config and timesheet HTML files exist
   tictac_track::AppConfig::GetInstance(argv);
   tictac_track::ReportCrud report_crud = tictac_track::ReportCrud::GetInstance();
   if (!report_crud.ReportExists()) {
     return -1;
   }

   // Process command arguments, display help if no valid command given
   auto * app = new tictac_track::App(argc, argv);
   if (argc > 1) app->Process();

   return 0;
}
