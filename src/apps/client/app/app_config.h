
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_CONFIG
#define CLASS_TIMETRACKERCL_CONFIG

#include <string>
#include <map>

namespace tictac_track {
class AppConfig {
 public:
  static const std::string kFilename;

  enum ConfigKeys {
    Option_Cli_Theme,
    Option_Format_Date,
    Option_Format_Day_Of_Week,
    Option_Format_Week_Of_Year,
    Option_Id_Column,
    Option_Locale_Key,
    Option_Max_Mergeable_Gap,
    Option_Report_File_Path,
    Option_First_Task_Of_Day,
    Option_Invalid
  };

  // Get object instance. Initialize at 1st call.
  static AppConfig &GetInstance(char **argv = nullptr);

  // Resolve config option string to related enum item (which allows e.g. switch)
  static ConfigKeys ResolveOption(std::string input);

  // Get value for given config option
  std::string GetConfigValue(const std::string &key);
  // Get instance of config, than get value for given config option
  static std::string GetConfigValueStatic(const std::string &key);

  static const char* GetDefaultFirstTaskOfDay();

  // Get absolute path to timesheet.html (including filename)
  std::string GetReportFilePath();

 private:
  char **argv_{};
  std::string path_binary_;
  bool is_initialized_ = false;
  std::string config_file_content_;
  std::map<std::string, std::string> config_map_;

  // Prevent construction from outside (singleton)
  AppConfig() = default;;

  // Create default ".tictac-track.conf" if it does not exist, store in property
  void Init(char **argv = nullptr);

  std::string GetDefaultConfig();

  // Save given content to file of given name in directory where also the binary is
  void SaveConfig(std::string filename, std::string content);

  // Read config from .ttt.ini into associative map
  void InitConfigMap();

  static std::string GetDefaultLanguageKey();

  std::string GetConfigValueDefault(const std::string &key);

  static int GetDefaultThemeIdByOs();
};
} // namespace tictac_lib

#endif
