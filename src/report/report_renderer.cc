
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>
#include <cstring>
#include "report_renderer.h"
#include "app/app_config.h"
#include "helper/helper_file.h"
#include "helper/helper_string.h"
#include "helper/helper_html.h"
#include "report_parser.h"
#include "helper/helper_numeric.h"

namespace tictac_track {

/**
 * Extract from timesheet HTML: amount columns, column titles, column max. length, rows amount, cells content
 */
bool ReportRenderer::ExtractPartsFromReport(int filter_offset) {
  AppConfig &config = AppConfig::GetInstance();
  std::ifstream file(config.GetReportFilePath());
  if (!file) return false;

  std::string html = helper::File::GetFileContents(file);
  if (html.empty()) return false;

  html = helper::String::ReplaceAll(html, "\n", "");
  std::string table = helper::String::GetSubStrBetween(html, "<table border=\"1\">", "</table>");
  if (table.empty()) return false;

  std::string tHead = ExtractTheadFromTable(table);
  if (tHead.empty()) return false;

  SetColumnTitlesExtractedFromTHead(tHead);

  for (int index_column = 0; index_column < amount_columns_; index_column++) {
    // Store content-length as initial maximum length for the rel. column
    column_content_max_len_.push_back(helper::String::GetUtf8Size(helper::Html::Decode(column_titles_[index_column])));
  }

  // Collect content of <td> cells and maximum content-length per column
  unsigned long cut_off_lhs_offset = table.find("</thead>") + 8;
  table = table.substr(cut_off_lhs_offset, table.size() - cut_off_lhs_offset - strlen("</table>"));
  amount_rows_ = helper::String::GetSubStrCount(table.c_str(), "<tr");

  std::vector<std::string> rows = ExtractRowsFromTable(table);

  std::string row;
  int contentLen;

  InitScopeFilter(filter_offset);

  int amount_rows_filtered = 0;
  for (int indexRow = 0; indexRow < amount_rows_; indexRow++) {
    row = rows[indexRow];

#pragma clang diagnostic push
#pragma ide diagnostic ignored "missing_default_case"
    switch (render_scope_) {
      case Scope_Day:
        if (!helper::String::Contains(row, rows_filter_)) {
          // Row does NOT contain the date to filter for: skip to next row
          id_first_row_rendered_++;
          continue;
        }
        break;
      case Scope_Week:ReportParser *parser = new ReportParser(html);
        std::string week_number = parser->GetColumnContent(indexRow, ColumnIndexes::Index_Week);
        std::string zero = "0";
        if (1 == week_number.size() && 2 == rows_filter_.size()) week_number = zero.append(week_number);

        if (0 != std::strcmp(week_number.c_str(), rows_filter_.c_str())) {
          // Week-column of row does NOT contain the week to filter for: skip to next row
          id_first_row_rendered_++;
          continue;
        }
        break;
    }
#pragma clang diagnostic pop

    amount_rows_filtered++;

    row = helper::String::ReplaceAll(row, "<td>", "");
    row = helper::String::ReplaceAll(row, "<td class=\"meta\">", "");
    row = helper::String::ReplaceAll(row, "</td>", "|");
    row = helper::String::ReplaceAll(row, "||", "| |");

    std::vector<std::string> columns = helper::String::Explode(row, '|');
    int amount_columns_current = columns.size();

    if (columns.size() >= helper::Numeric::ToUnsignedInt(amount_columns_)) {
      for (int indexColumn = 0; indexColumn < amount_columns_; indexColumn++) {
        std::string cellContent = indexColumn < amount_columns_current ? columns[indexColumn] : "";

        cells_.push_back(std::move(cellContent));
        contentLen = indexColumn == Report::ColumnIndexes::Index_Comment
                     ? helper::Numeric::ToUnsignedInt(helper::Html::Decode(columns[indexColumn]).size())
                     : helper::Numeric::ToUnsignedInt(columns[indexColumn].size());

        if (contentLen > column_content_max_len_[indexColumn]) {
          // Cell content is longest string in this column: store as maximum length
          column_content_max_len_[indexColumn] = contentLen;
        }
      }
    }
  }

  return true;
}

std::string ReportRenderer::ExtractTheadFromTable(std::string &table) {
  std::string tHead = helper::String::GetSubStrBetween(table, "<thead>", "</thead>");

  unsigned long cut_off_lhs_Len = strlen("<thead><tr>");
  tHead = tHead.substr(cut_off_lhs_Len, tHead.size() - cut_off_lhs_Len - strlen("</tr></thead>"));
  if (tHead.empty()) return "";

  amount_columns_ = helper::String::GetSubStrCount(tHead.c_str(), "<th");

  return amount_columns_ == 0 ? "" : tHead;
}

/**
 * Reduce HTML to pipe-separated columns, than split and assign to attribute: column_titles_
 */
void ReportRenderer::SetColumnTitlesExtractedFromTHead(std::string t_head) {
  t_head = helper::String::ReplaceAll(t_head, "<th class=\"meta\">", "");
  t_head = helper::String::ReplaceAll(t_head, "<th>", "");
  t_head = helper::String::ReplaceAll(t_head, "</th>", "|");

  column_titles_ = helper::String::Explode(t_head, '|');
}

std::vector<std::string> ReportRenderer::ExtractRowsFromTable(std::string table) {
  table = helper::String::ReplaceAll(table, "<tr>", "");
  table = helper::String::ReplaceAll(table, "<tr class=\"new-day\">", "");
  table = helper::String::ReplaceAll(table, "</tr>", "|");

  return helper::String::Explode(table, '|');
}
} // namespace tictac_lib
