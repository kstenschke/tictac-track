
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>
#include <sstream>
#include <vector>
#include <apps/redmine-sync/report/report_file.h>
#include <iostream>
#include "app_config.h"
#include "app.h"
#include "helper/helper_file.h"
#include "helper/helper_string.h"
#include "helper/helper_system.h"

namespace tictac_rms {
const std::string AppConfig::kFilename = ".ttt-redmine_sync.ini";

/**
 * Get object instance. Initialize at 1st call: ensure config file exists, load contents to attribute
 */
AppConfig &AppConfig::GetInstance(char **argv) {
  // Instantiated on first use
  static AppConfig instance;
  if (!instance.is_initialized_) instance.Init(argv);

  return instance;
}

/**
 * Create default ".ttt-rms.ini" if it does not exist, store to instance property
 */
void AppConfig::Init(char **argv) {
  is_initialized_ = true;

  if (argv != nullptr) argv_ = argv;

  path_binary_ = helper::System::GetBinaryPath(argv_, std::strlen("ttt-redmine_sync")); //App::kAppExecutableName.c_str()));
  std::string path_config_file = path_binary_ + AppConfig::kFilename;

  if (helper::File::FileExists(path_config_file)) {
    // Load existing config
    config_file_content_ = helper::File::GetFileContents(path_config_file);
  } else {
    // Config does not exist: Create default config
    config_file_content_ = GetDefaultConfig();
    SaveConfig(path_config_file, config_file_content_);
//      std::cout << "Saved config: " << path_config_file << "\n\n";
  }

  InitConfigMap();
}

/**
 * Save given content to file of given name in directory where also the binary is
 */
void AppConfig::SaveConfig(std::string path_config_file, std::string content) {
  std::ofstream outfile(path_config_file);
  outfile << content;
  outfile.close();
}

std::string AppConfig::GetDefaultConfig() {
    std::stringstream content;

    content << ";tictac-track Redmine-Sync Configuration"
            << "\n;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;"
            << "\n"
            << "\n; Absolute directory path to timesheet.html. If not set: directory where executable is"
            << "\n;report_path=" + helper::System::GetBinaryPath(argv_, std::strlen(App::kAppExecutableName.c_str()))
            << "\n"
            << "\n; Redmine URL and credentials"
            << "\nredmine_url=\"http://redmine.com\""
            << "\nredmine_username=\"yourname\""
            << "\nredmine_password=\"password\""
            << "\n"
            << "\n; Round times to next full minutes"
            << "\nround_minutes=15"
            << "\n"
            << "\n; Date until last entries were synced, format: yyyy-mm-dd"
            << "\ndate_synced_until=-1";

  return content.str();
}

/**
 * Read config from ttt.ini into associative map
 */
void AppConfig::InitConfigMap() {
  config_map_.erase(config_map_.begin(), config_map_.end());

  std::vector<std::string> lines = helper::String::Explode(config_file_content_, '\n');
  for (std::vector<int>::size_type i = 0; i != lines.size(); i++) {
    std::string line = lines[i];
    if (!helper::String::StartsWith(line.c_str(), ";")) {
      size_t offset_equals = line.find('=');
      if (std::string::npos != offset_equals) {
        std::string key = line.substr(0, offset_equals);
        std::string value = line.substr(offset_equals + 1, std::string::npos);

        config_map_.insert(std::make_pair(key, value));
      }
    }
  }
}

/**
 * Resolve config option string to related enum item (which allows e.g. switch)
 */
AppConfig::ConfigKeys AppConfig::ResolveOption(std::string input) {
  /*if (input == "cli_theme") return Option_Cli_Theme;
  if (input == "format_week_of_year") return Option_Format_Week_Of_Year;
  if (input == "format_date") return Option_Format_Date;
  if (input == "format_day_of_week") return Option_Format_Day_Of_Week;
  if (input == "id_column") return Option_Id_Column;
  if (input == "locale") return Option_Locale_Key;
  if (input == "max_mergeable_minutes_gap") return Option_Max_Mergeable_Gap;
  if (input == "report_path") return Option_Report_File_Path;*/

  return Option_Invalid;
}

/**
 * Get value of given key from .tictac-track.conf, or default value
 */
std::string AppConfig::GetConfigValue(const std::string &key) {
  return 0 == config_map_.count(key) ? GetConfigValueDefault(key) : config_map_[key];
}

/**
 * Get default value for given config option
 */
std::string AppConfig::GetConfigValueDefault(const std::string &key) {
  switch (ResolveOption(key)) {
    case Option_Report_File_Path:
      return helper::System::GetBinaryPath(argv_, std::strlen(App::kAppExecutableName.c_str()));
    case Option_Invalid:
    default:
      return "";
  }
}

/**
 * Get absolute path to timesheet.html (including filename)
 */
std::string AppConfig::GetReportFilePath() {
  AppConfig config = GetInstance();
  std::string report_dir_path = config.GetConfigValue("report_path");

  return report_dir_path + ReportFile::kFilenameReport;
}
} // namespace tictac_rms
