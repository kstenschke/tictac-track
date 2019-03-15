/*
  Copyright (c) 2018, Kay Stenschke
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

#ifndef CLASS_TIMETRACKERCL_REPORT_RENDERER
#define CLASS_TIMETRACKERCL_REPORT_RENDERER

#include <vector>
#include "report.h"

namespace tictac_track {

class ReportRenderer : public Report {
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
} // namespace tictac_lib

#endif
