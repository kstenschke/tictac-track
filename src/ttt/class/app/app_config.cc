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

#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>

#include "app_config.h"
#include "app.h"
#include "../../helper/helper_file.h"
#include "../report/report_file.h"
#include "../../helper/helper_string.h"
#include "../../helper/helper_system.h"
#include "app_locale.h"
#include "../../helper/helper_numeric.h"
#include "../report/report_crud.h"

namespace tictac_track {
const std::string AppConfig::kFilename = ".ttt.ini";

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
 * Create default ".tictac-track.conf" if it does not exist, store to instance property
 */
void AppConfig::Init(char **argv) {
  is_initialized_ = true;

  if (argv != nullptr) argv_ = argv;

  path_binary_ = helper::System::GetBinaryPath(argv_, std::strlen(App::kAppExecutableName.c_str()));
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
  content << ";tictac-track Configuration"
          << "\n;;;;;;;;;;;;;;;;;;;;;;;;;;;;"
          << "\n"
          << "\n; Absolute directory path to timesheet.html. If not set: directory where executable is"
          << "\n;report_path=" << helper::System::GetBinaryPath(argv_, std::strlen(App::kAppExecutableName.c_str()))
          << "\n"
          << "\n; Supported languages: de, dk, en, es, fi, fr, hu, it, lt, lv, nl, no, pl, pt, ro, sv, tr"
          << "\nlocale=" << GetDefaultLanguageKey()
          << "\n"
          << "\n; Debit: Amount of expected working time per day. Used for overall saldo calculation"
          << "\ndebit_per_day=8:24"
          << "\n; Default daily starting time. Used for insertion of all-day entries. Format: HH:MM"
          << "\ndefault_daily_start_time=09:00"
          << "\n; First task of day, used when starting w/o given other task number"
          << "\n;first_task_of_day=1234"
          << "\n"
          << "\n; Maximum gap between entries to be allowed to be merged. Format: In minutes (if not set: 0)"
          << "\n; Also used for detection before emphasis of lunch- and other longer breaks in console timesheet view"
          << "\n; When viewed in console, end/start-times of timesheet entries around such breaks are displayed bold"
          << "\nmax_mergeable_minutes_gap=2"
          << "\n"
          << "\n; External task action URLs:"
          << "\n; Actions other than \"default\" can arbitrarily be added here."
          << "\n;  Example 1: ttt u          -> opens url.default in web browser"
          << "\n;  Example 2: ttt u 123      -> opens url.default, with \"#TASK#\" replaced by \"123\" in web browser"
          << "\n;  Example 3: ttt u d        -> opens url.default, for all tasks in current day"
          << "\n;  Example 4: ttt u d -1     -> opens url.default, for all tasks in previous day"
          << "\n;  Example 5: ttt u 123 edit -> opens url.edit, with \"#TASK#\" replaced by \"123\" in web browser"
          << "\n;  Example 6: ttt u d edit   -> opens url.edit, with \"#TASK#\" replaced by \"123\" in web browser, etc."
          << "\nurl.default=http://redmine.yourdomain.net/task/#TASK#"
          << "\nurl.edit=http://redmine.yourdomain.net/task/#TASK#/edit"
          << "\nurl.log=http://redmine.yourdomain.net/task/#TASK#/log"
          << "\n"
          << "\n; Clear console before viewing timesheet via view (v) command?"
          << "\nclear_before_view=0"
          << "\n"
          << "\n; ANSI colors theme for report display in CLI"
          << "\n; Commented-out or not given = Auto-select"
          << "\n; 0                          = Mac OS Terminal \"Visor\""
          << "\n; 1                          = Linux Bash default theme"
          << "\n; 2                          = Linux Bash alternative theme"
          << "\n; 3                          = Linux Bash Gogh/Dracula theme"
          << "\n; 4                          = Mac OS Terminal Dracula theme"
          << "\n;cli_theme=" << GetDefaultThemeIdByOs()
          << "\n"
          << "\n;;;;;;;;;;;;;;;;;;;;;"
          << "\n; Date/time formats ;"
          << "\n;;;;;;;;;;;;;;;;;;;;;"
          << "\n"
          << "\n; %W = Week starting w/ monday, %U = Week starting w/ sunday"
          << "\nformat_week_of_year=" << GetConfigValueDefault("format_week_of_year")
          << "\nformat_date=" << GetConfigValueDefault("format_date")
          << "\n; %a = Abbreviated day of week, %A = Full day of week"
          << "\nformat_day_of_week=" << GetConfigValueDefault("format_day_of_week")
          << "\n"
          << "\n; Offset of ID-column (e.g.: 0 = left-most, 5 = left of task, 6 = left of comment, 10 = right-most)"
          << "\n; The ID-column is shown dynamically, in the CLI only, its configured offset can be changed"
          << "\n; w/o updating existing data."
          << "\nid_column=5"
          << "\n";

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
    if (helper::String::StartsWith(line.c_str(), ";")) continue;

    size_t offset_equals = line.find('=');
    if (std::string::npos == offset_equals) continue;

    std::string key = line.substr(0, offset_equals);
    std::string value = line.substr(offset_equals + 1, std::string::npos);

    config_map_.insert(std::make_pair(key, value));
  }
}

/**
 * Resolve config option string to related enum item (which allows e.g. switch)
 */
AppConfig::ConfigKeys AppConfig::ResolveOption(std::string input) {
  if ("cli_theme" == input) return Option_Cli_Theme;
  if ("format_week_of_year" == input) return Option_Format_Week_Of_Year;
  if ("format_date" == input) return Option_Format_Date;
  if ("format_day_of_week" == input) return Option_Format_Day_Of_Week;
  if ("id_column" == input) return Option_Id_Column;
  if ("locale" == input) return Option_Locale_Key;
  if ("max_mergeable_minutes_gap" == input) return Option_Max_Mergeable_Gap;
  if ("report_path" == input) return Option_Report_File_Path;
  if ("first_task_of_day" == input) return Option_First_Task_Of_Day;
  if ("default_daily_start_time" == input) return Option_Default_Daily_Start_Time;

  return Option_Invalid;
}

/**
 * Get system language, if translations not available in app: default to english
 */
std::string AppConfig::GetDefaultLanguageKey() {
  std::string languageKey = helper::System::GetLanguageKey();

  AppLocale &locale = AppLocale::GetInstance();

  return locale.IsSupportedLanguageKey(languageKey) ? languageKey : "en";
}

/**
 * Get value of given key from .tictac-track.conf, or default value
 */
std::string AppConfig::GetConfigValue(const std::string &key) {
  return 0 == config_map_.count(key) ? GetConfigValueDefault(key) : config_map_[key];
}
/**
 * Get instance of config, than get value for given config option
 */
std::string AppConfig::GetConfigValueStatic(const std::string &key) {
  return AppConfig::GetInstance().GetConfigValue(key);
}

const char* AppConfig::GetDefaultFirstTaskOfDay() {
  return AppConfig::GetInstance().GetConfigValue("first_task_of_day").c_str();
}

/**
 * Get default value for given config option
 */
std::string AppConfig::GetConfigValueDefault(const std::string &key) {
  switch (ResolveOption(key)) {
    case Option_Cli_Theme:
      return helper::Numeric::ToString(GetDefaultThemeIdByOs());
    case Option_Default_Daily_Start_Time:
      return "09:00";
    case Option_First_Task_Of_Day:
      return "";
    case Option_Format_Week_Of_Year:
      // %W = Week starting w/ monday, %U = Week starting w/ sunday
      return "%W";
    case Option_Format_Date:
      return "%d.%m.%Y";
    case Option_Format_Day_Of_Week:
      return "%A";
    case Option_Locale_Key:
      return "en";
    case Option_Id_Column:
    case Option_Max_Mergeable_Gap:
      return "0";
    case Option_Report_File_Path:
      return helper::System::GetBinaryPath(argv_, std::strlen(App::kAppExecutableName.c_str()));
    case Option_Invalid:
    default:return "";
  }
}

/**
 * Get absolute path to timesheet.html (including filename)
 */
std::string AppConfig::GetReportFilePath() {
  AppConfig config = GetInstance();
  std::string report_dir_path = config.GetConfigValue("report_path");

  return report_dir_path + ReportCrud::kFilenameReport;
}

/**
 * Get ID of default (ANSI coloring in CLI) theme by operating system
 */
int AppConfig::GetDefaultThemeIdByOs() {
  return helper::System::kOsName == "macOs"
         // Blue theme, optimized for Mac
         ? 1
         // Blue theme w/ zebra-look, optimized for Linux
         : 0;
}
} // namespace tictac_track
