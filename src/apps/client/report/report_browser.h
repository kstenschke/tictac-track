
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_REPORT_BROWSER
#define CLASS_TIMETRACKERCL_REPORT_BROWSER

#include "report.h"
#include "report_renderer.h"

namespace tictac_track {
  class ReportBrowser : public Report {
    public:
      public:
      // Constructor
      ReportBrowser();

      // Open URL (default: timesheet) in web browser
      static bool Browse(std::string url = "");

      // Open configured task action URL in web browser
      bool BrowseTaskUrl(ReportRenderer::RenderScopes render_scope = ReportRenderer::RenderScopes::Scope_Invalid,
                         int offset = 0, int task_number = 0, std::string url_command = "url.default");

    private:
      static void BrowseOnLinux(std::string url);
      static void BrowseOnMac(std::string url);
  };
} // namespace tictac_track

#endif
