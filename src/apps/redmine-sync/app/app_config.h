
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_CONFIG
#define CLASS_TIMETRACKERCL_CONFIG

#include <string>
#include <map>

namespace tictac_rms {
class AppConfig {
 public:
  static const std::string kFilename;

  enum ConfigKeys {
    Option_Redmine_Url,
    Option_Redmine_Username,
    Option_Remine_Password,
    Option_Round_Minutes,
    Option_Report_File_Path,
    Option_Invalid,
  };

  // Get object instance. Initialize at 1st call.
  static AppConfig &GetInstance(char **argv = nullptr);

  // Resolve config option string to related enum item (which allows e.g. switch)
  static ConfigKeys ResolveOption(std::string input);

  // Get value for given config option
  std::string GetConfigValue(const std::string &key);

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

  // Create default ".ttt-rms.ini" if it does not exist, store in property
  void Init(char **argv = nullptr);

  std::string GetDefaultConfig();

  // Save given content to file of given name in directory where also the binary is
  void SaveConfig(std::string filename, std::string content);

  // Read config from .ttt.ini into associative map
  void InitConfigMap();

  std::string GetConfigValueDefault(const std::string &key);
};
} // namespace tictac_track

#endif
