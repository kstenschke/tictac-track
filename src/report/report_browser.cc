
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iostream>
#include "report_browser.h"
#include "app/app_config.h"
#include "helper/helper_system.h"
#include "app/app_error.h"
#include "report_parser.h"
#include "helper/helper_string.h"
#include "helper/helper_numeric.h"

namespace tictac_track {

  /**
   * Constructor
   */
  ReportBrowser::ReportBrowser() = default;

/**
   * Open URL (dafault: timehseet) in web browser
   */
  bool ReportBrowser::BrowseTimesheet(std::string url) {
    AppConfig& config = AppConfig::GetInstance();
    std::string report_file_path = config.GetReportFilePath();

    if (url.empty()) url = "file://" + report_file_path;

    if (helper::System::kOsName == "linux" || helper::System::kOsName == "unix") {
      // Linux/Unix
      BrowseOnLinux(url);
    } else if (helper::System::kOsName == "macOs") {
      // Mac OS
      BrowseOnMac(url);
    } else {
      // Windows
      // TODO implement open in browser for windows
    }

    return true;
  }

  void ReportBrowser::BrowseOnLinux(std::string url) {
      std::string cmd = "firefox " + url;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
      system(cmd.c_str());
#pragma clang diagnostic pop
  }

  void ReportBrowser::BrowseOnMac(std::string url) {
      std::string cmd = "open " + url;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
      system(cmd.c_str());
#pragma clang diagnostic pop
  }

  bool ReportBrowser::BrowseTaskUrl(int task_number, std::string url_command) {
    AppConfig config = AppConfig::GetInstance();
    std::string url_raw = config.GetConfigValue(url_command);
    if (url_raw.empty()) return tictac_track::AppError::PrintError("Invalid URL.");
    
    std::string task = task_number > 0 ? helper::Numeric::ToString(task_number) : "";

    return BrowseTimesheet(helper::String::ReplaceAll(url_raw, "#TASK#", task.c_str()));
  }
  
  /**
   * Open configured task action URL in web browser. If in day-scope: all tasks of day
   */
  bool ReportBrowser::BrowseTaskUrlsInScope(
    ReportRenderer::RenderScopes render_scope, int offset, int task_number,
    std::string url_command
  ) {
    AppConfig config = AppConfig::GetInstance();

    std::string url_raw = config.GetConfigValue(url_command);
    if (url_raw.empty()) return tictac_track::AppError::PrintError("Invalid URL.");

    if (render_scope == ReportRenderer::RenderScopes::Scope_Day) {
        ReportParser *parser = new ReportParser();
        if (!parser->LoadReportHtml()) return false;

        InitScopeFilter(render_scope, offset);
        std::vector<std::string> tasks = parser->GetTasksOfDay(rows_filter_);
        if (tasks.empty()) return tictac_track::AppError::PrintError(
          std::string("No tasks found in day (").append(rows_filter_).append(").").c_str());

        for (auto const& task : tasks) {
          std::cout << task << " ";
          std::string url = helper::String::ReplaceAll(url_raw.c_str(), "#TASK#", task.c_str());
          BrowseTimesheet(url);
        }

        return true;
    }

    std::string task = task_number > 0 ? helper::Numeric::ToString(task_number) : "";

    return BrowseTimesheet(helper::String::ReplaceAll(url_raw, "#TASK#", task.c_str()));
  }

} // namespace tictac_lib
