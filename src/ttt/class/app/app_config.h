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

#ifndef CLASS_TTT_CONFIG
#define CLASS_TTT_CONFIG

#include <string>
#include <map>

namespace tictac_track {
class AppConfig {
 public:
  static const std::string kFilename;

  enum ConfigKeys {
    Option_Cli_Theme,
    Option_Default_Daily_Start_Time,
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
  static ConfigKeys ResolveOption(const std::string &input);

  // Get value for given config option
  std::string GetConfigValue(const std::string &key);
  // Get instance of config, than get value for given config option
  static std::string GetConfigValueStatic(const std::string &key);

  static const char *GetDefaultFirstTaskOfDay();

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
  void SaveConfig(std::string filename, std::string &content);

  // Read config from .ttt.ini into associative map
  void InitConfigMap();

  static std::string GetDefaultLanguageKey();

  std::string GetConfigValueDefault(const std::string &key);

  static int GetDefaultThemeIdByOs();
};

} // namespace tictac_track

#endif
