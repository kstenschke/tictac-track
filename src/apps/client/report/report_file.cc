
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <fstream>
#include "report_file.h"
#include "../app/app.h"
#include "apps/client/app/app_config.h"
#include "lib/helper/helper_file.h"
#include "lib/app/app_error.h"

namespace tictac_track {

  const std::string ReportFile::kFilenameReport = "timesheet.html";

  /**
   * Get input stream to report file
   */
  std::ifstream ReportFile::GetReportIfStream() {
    AppConfig& config = AppConfig::GetInstance();
    std::string report_file_path = config.GetReportFilePath();
    std::ifstream file(report_file_path);

    return file;
  }

  std::string ReportFile::GetReportHtml() {
    std::ifstream file = ReportFile::GetReportIfStream();

    return file ? helper::File::GetFileContents(file) : "";
  }

  /**
   * Save given HTML to timesheet, replacing any previous content
   */
  bool ReportFile::SaveReport(std::string html) {
    AppConfig& config = AppConfig::GetInstance();
    std::string report_file_path = config.GetReportFilePath();

    std::ofstream outfile;
    outfile.open(report_file_path, std::ios_base::trunc);
    outfile << html;
    outfile.close();

    return true;
  }

  /**
   * Backup timesheet.html to timesheet.html.bak.tmp
   */
  bool ReportFile::BackupReportTemporary() {
    AppConfig& config = AppConfig::GetInstance();
    std::string report_file_path = config.GetReportFilePath();

    if (!helper::File::FileExists(report_file_path)) return false;

    //std::string filename_bak = ReportFile::kFilenameReport + ".bak";
    std::string filename_bak = report_file_path + ".bak";

    std::ifstream src(report_file_path, std::ios::binary);
    std::ofstream dst(filename_bak, std::ios::binary);

    dst << src.rdbuf();

    return true;
  }

  /**
   * Remove timesheet.html.bak, rename timesheet.html.bak to timesheet.html.bak
   */
  bool ReportFile::ActivateTemporaryBackup() {
    AppConfig& config = AppConfig::GetInstance();
    std::string path_report_file = config.GetReportFilePath();

    if (!helper::File::FileExists(path_report_file)) return false;

    std::string path_backup_file = path_report_file + ".bak";
    std::string path_backup_tmp  = path_backup_file + ".tmp";

    if (!helper::File::FileExists(path_backup_tmp)) return false;

    if (helper::File::FileExists(path_backup_file)) std::remove(path_backup_file.c_str());

    return static_cast<bool>(std::rename(path_backup_tmp.c_str(), path_backup_file.c_str()));
  }

  /**
   * Remove timesheet.html.bak.tmp
   */
  bool ReportFile::RemoveTemporaryBackup() {
    AppConfig& config = AppConfig::GetInstance();
    std::string path_report_file = config.GetReportFilePath();

    std::string path_tmp_backup = path_report_file + ".bak.tmp";
    std::remove(path_tmp_backup.c_str());

    return true;
  }

  /**
   * Overwrite timesheet.html with timesheet.html.bak
   */
  bool ReportFile::RestoreBackup() {
    AppConfig& config = AppConfig::GetInstance();
    std::string path_report_file = config.GetReportFilePath();
    std::string path_backup      = path_report_file + ".bak";
    if (!helper::File::FileExists(path_backup)) return tictac_lib::AppError::PrintError("Cannot undo.");

    std::ifstream src(path_backup, std::ios::binary);
    std::ofstream dst(path_report_file, std::ios::binary);
    dst << src.rdbuf();

    std::remove(path_backup.c_str());

    return true;
  }

  /**
   * Create initial timesheet HTML file
   */
  bool ReportFile::InitReportFile(bool removeIfExists) {
    AppConfig& config = AppConfig::GetInstance();
    std::string report_file_path = config.GetReportFilePath();

    //if (removeIfExists && helper::File::FileExists(ReportFile::kFilenameReport)) {
    if (helper::File::FileExists(report_file_path) && removeIfExists) std::remove(report_file_path.c_str());

    std::string content = ReportHtmlParser::GetInitialReportHtml();
//    std::cout << "Created report: " << report_file_path << "\n\n";

    return helper::File::WriteToNewFile(report_file_path, content);
  }

} // namespace tictac_lib
