
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_REPORT_HTML_ANALYZER
#define CLASS_TIMETRACKERCL_REPORT_HTML_ANALYZER

#include <vector>
#include <apps/redmine-sync/report/report.h>
#include <apps/client/report/report.h>
#include "apps/client/app/app_commands.h"

namespace tictac_rms {
class ReportParser : public Report {
 public:
  // Constructor
  explicit ReportParser(std::string html = "");

  bool LoadReportHtml();
  std::string GetHtml();
  void SetHtml(std::string html);

  std::string GetColumnContent(int row_index, ColumnIndexes index_column);

  int GetOffsetTrOpenByIndex(std::string &html, int index_row);

  int GetLastIndex();

  size_t GetColumnOffset(const char *needle, unsigned long offset_initial, Report::ColumnIndexes column_index);

  std::string GetDateLatestEntry();

 protected:
  std::string html_;
};
} // namespace tictac_rms

#endif
