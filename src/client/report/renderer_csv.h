
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_REPORT_RENDERER_CSV
#define CLASS_TIMETRACKERCL_REPORT_RENDERER_CSV

#include "renderer.h"

namespace tictac_track {

class ReportRendererCsv : public ReportRenderer {
 public:
  // Constructor
  ReportRendererCsv();

  static std::string GetFilename();

  // Export timesheet HTML (table) to CSV file
  bool RenderToFile(std::string path, RenderScopes scope);

 private:
  static std::string GetFilename(std::string html);

  // Render timesheet to CSV
  std::string RenderCsv(RenderScopes scope);

 protected:

};
} // namespace tictac_track

#endif
