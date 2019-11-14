/*
  Copyright (c) 2018-2019, Kay Stenschke
  All rights reserved.
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
   * Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   * Neither the name of  nor the names of its contributors may be used to
     endorse or promote products derived from this software without specific
     prior written permission.
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "report_renderer_csv.h"
#include "report_parser.h"
#include "../../helper/helper_date_time.h"
#include "report_file.h"
#include "../app/app_locale.h"
#include "../../helper/helper_file.h"
#include "../../helper/helper_system.h"
#include "../../helper/helper_string.h"

namespace tictac_track {

/**
 * Constructor
 */
ReportRendererCsv::ReportRendererCsv() = default;

/**
   * Get timesheet CSV filename
   */
std::string ReportRendererCsv::GetFilename(std::string html) {
  ReportParser *parser = new ReportParser(std::move(html));

  std::string date_first = parser->GetColumnContent(0, ColumnIndexes::Index_Date);

  auto *report_date_time = new ReportDateTime();
  std::string report_date_format = report_date_time->GetDateFormat();
  date_first = helper::DateTime::ReformatDateAsYyyyMmDd(date_first, report_date_format);

  std::string date_last;

  int index_last_entry = parser->GetLastIndex();
  date_last = parser->GetColumnContent(index_last_entry, ColumnIndexes::Index_Date);
  date_last = helper::DateTime::ReformatDateAsYyyyMmDd(date_last, report_date_format);

  AppLocale locale = AppLocale::GetInstance();
  std::string title = locale.Translate("timesheet");

  title = title.append("_").append(date_first);
  if (date_first != date_last)
    title = title.append("-").append(date_last);

  std::transform(title.begin(), title.end(), title.begin(), ::tolower);

  return title.append(".csv");
}

std::string ReportRendererCsv::GetFilename() {
  std::string html = ReportFile::GetReportHtml();

  return html.empty() ? "" : GetFilename(html);
}

/**
 * Export timesheet HTML (table) to CSV file
 */
bool ReportRendererCsv::RenderToFile(std::string path, RenderScopes scope) {
  if (!ExtractPartsFromReport(0)) return false;

  std::string csv = RenderCsv(scope);
  if (!csv.empty()) {
    std::string filename = GetFilename();
    if (helper::File::FileExists(filename)) std::remove(ReportFile::kFilenameReport.c_str());

    if (helper::File::WriteToNewFile(filename, csv)) {
      std::cout << "Exported report CSV to: " << path << filename << "\n";
      return true;
    }
  }

  std::cout << "Failed export report to CSV.\n";
  return false;
}

bool ReportRendererCsv::RenderToStdOut(RenderScopes scope) {
  if (!ExtractPartsFromReport(0)) return false;

  std::string csv = RenderCsv(scope);
  if (!csv.empty()) {
    std::cout << csv;
    return true;
  }

  std::cout << "Failed export to CSV.\n";

  return false;
}

/**
 * Render timesheet to CSV
 */
std::string ReportRendererCsv::RenderCsv(RenderScopes scope) {
  render_scope_ = scope;

  std::string csv;
  if (!ExtractPartsFromReport(0)) return csv;

  std::stringstream csv_stream;

  int last_column = amount_columns_ - 1;
  // Render column titles
  for (int index_column = 1; index_column < amount_columns_; index_column++) {
    csv_stream << "\"" << column_titles_[index_column] << "\""
               << (index_column == last_column ? helper::System::kNewline : ",");
  }

  // Render rows
  int index_cell = 0;
  auto amountCells = static_cast<int>(cells_.size());
  std::string previousDay;
  for (int index_row = 0; index_row < amount_rows_ && index_cell < amountCells; index_row++) {
    for (int index_column = 0; index_column < amount_columns_ && index_cell < amountCells; index_column++) {
      if (index_column > 0) {
        csv_stream << "\"" << helper::String::CsvEncode(cells_[index_cell]) << "\"";
        csv_stream << (index_column == last_column ? ("\"" + helper::System::kNewline) : ",");
      }
      ++index_cell;
    }
  }

  return csv_stream.str();
}
} // namespace tictac_track
