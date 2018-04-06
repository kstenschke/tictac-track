
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <helper/helper_file.h>
#include <fstream>
#include <apps/redmine-sync/app/app_config.h>
#include "report_file.h"

namespace tictac_rms {

const std::string ReportFile::kFilenameReport = "timesheet.html";

/**
 * Get input stream to report file
 */
std::ifstream ReportFile::GetReportIfStream() {
  AppConfig &config = AppConfig::GetInstance();
  std::string report_file_path = config.GetReportFilePath();
  std::ifstream file(report_file_path);

  return file;
}

std::string ReportFile::GetReportHtml() {
  std::ifstream file = ReportFile::GetReportIfStream();

  return file ? helper::File::GetFileContents(file) : "";
}
} // namespace tictac_rms