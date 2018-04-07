
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstring>
#include "report_parser.h"
#include "report_file.h"
#include "lib/helper/helper_string.h"
#include "apps/redmine-sync/app/app_config.h"
#include "lib/helper/helper_date_time.h"
#include "lib/app/app_error.h"
#include "lib/helper/helper_numeric.h"

namespace tictac_rms {
/**
 * Constructor
 */
ReportParser::ReportParser(std::string html) {
  html_ = std::move(html);
}

bool ReportParser::LoadReportHtml() {
  html_ = ReportFile::GetReportHtml();

  if (html_.empty()) return false;

  html_ = helper::String::ReplaceAll(html_.c_str(), "\n\n", "\n");

  return true;
}

std::string ReportParser::GetHtml() {
  return html_;
}

void ReportParser::SetHtml(std::string html) {
  html_ = std::move(html);
}

std::string ReportParser::GetColumnContent(int row_index, ColumnIndexes index_column) {
  int offset_tr = GetOffsetTrOpenByIndex(html_, row_index);
  if (-1 == offset_tr) return "";

  unsigned long offset_td_start = GetColumnOffset("<td", static_cast<unsigned long>(offset_tr), index_column);
  unsigned long offset_content_start = static_cast<int>(html_.find('>', offset_td_start) + 1);
  auto offset_content_end = static_cast<int>(html_.find("</td>", offset_content_start));

  return html_.substr(offset_content_start, offset_content_end - offset_content_start);
}

int ReportParser::GetOffsetTrOpenByIndex(std::string &html, int index_row) {
  size_t offset_tr;

  if (-1 == index_row) {
    // Get last row entry
    offset_tr = html.rfind("<tr");
    if (std::string::npos != offset_tr) return static_cast<int>(offset_tr);

    std::cout << "Error - Failed finding row.\n";
    return -1;
  }

  // Get entry at given row-index
  offset_tr = static_cast<size_t>(0);
  for (int i = 0; i <= index_row + 1; i++) {
    offset_tr = html.find("<tr", offset_tr + 1);
    if (std::string::npos == offset_tr) return -1;
  }

  return static_cast<int>(offset_tr);
}

/**
 * Get offset of given needle (e.g. "<td>" or "</td>") of given column, after given initial offset
 */
size_t ReportParser::GetColumnOffset(const char *needle, unsigned long offset_initial,
                                     Report::ColumnIndexes column_index) {
  auto column_index_int = static_cast<int>(column_index);
  size_t offset = offset_initial;
  for (int i = 0; i <= column_index_int; i++) {
    offset = html_.find(needle, offset + 1);

    if (std::string::npos == offset) return std::string::npos;
  }

  return offset;
}

std::string ReportParser::GetDateById(int id) {
  return GetColumnContent(0, ColumnIndexes::Index_Date);
}

std::string ReportParser::GetDateLatestEntry() {
  int index_last = GetLastIndex();

  return GetColumnContent(index_last, ColumnIndexes::Index_Date);
}

int ReportParser::GetLastIndex() {
  // Do not count header. Subtract one more, as index is zero-based
  return helper::String::GetSubStrCount(html_.c_str(), "<tr") - 2;
}

} // namespace tictac_lib
