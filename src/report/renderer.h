
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_REPORT_RENDERER
#define CLASS_TIMETRACKERCL_REPORT_RENDERER

#include <vector>
#include "report.h"

namespace timesheetplus {

class ReportRenderer : public Report {
 public:

 protected:
  // Amount of <tr> report rows
  int amount_rows_;
  // Amount of <td> columns per row in report table
  int amount_columns_;
  // Title texts of report columns
  std::vector<std::string> column_titles_;
  // Maximum length per (1 line of) column content
  std::vector<int> column_content_max_len_;

  // Content of cells
  std::vector<std::string> cells_;

  int id_first_row_rendered_ = 0;

  // Extract from timesheet HTML: amount columns, column titles, column max. length, rows amount, cells content
  bool ExtractPartsFromReport(int filter_offset);

 private:
  std::string ExtractTheadFromTable(std::string &table);
  // Reduce HTML to pipe-separated columns, than split and assign to attribute: column_titles_
  void SetColumnTitlesExtractedFromTHead(std::string t_head);
  std::vector<std::string> ExtractRowsFromTable(std::string table);
};
} // namespace timesheetplus

#endif
