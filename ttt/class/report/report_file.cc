/*
  Copyright (c) Kay Stenschke
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

#include <ttt/class/app/app.h>
#include <ttt/class/app/app_config.h>
#include <ttt/class/report/report_file.h>
#include <ttt/helper/helper_tui.h>

namespace tictac_track {
const char ReportFile::kFilenameReport[15] = "timesheet.html";

// Get input stream to report file
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

// Save given HTML to timesheet, replacing any previous content
bool ReportFile::SaveReport(const std::string &html) {
  AppConfig &config = AppConfig::GetInstance();
  std::string report_file_path = config.GetReportFilePath();

  std::ofstream outfile;
  outfile.open(report_file_path, std::ios_base::trunc);
  outfile << html;
  outfile.close();

  return true;
}

// Create initial timesheet HTML file
bool ReportFile::InitReportFile(bool removeIfExists) {
  AppConfig &config = AppConfig::GetInstance();
  std::string report_file_path = config.GetReportFilePath();

  if (helper::File::FileExists(report_file_path)
      && removeIfExists) {
    std::remove(report_file_path.c_str());
  }

  std::string content = ReportParser::GetInitialReportHtml();

  return helper::File::WriteToNewFile(report_file_path, content);
}

}  // namespace tictac_track
