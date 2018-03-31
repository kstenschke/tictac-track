
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iostream>
#include "browser.h"
#include "../app/config.h"
#include "../helper/system.h"
#include "../app/error.h"
#include "html_parser.h"
#include "../helper/helper_string.h"
#include "../helper/numeric.h"

namespace timesheetplus {

  /**
   * Constructor
   */
  ReportBrowser::ReportBrowser() = default;

/**
   * Open URL (dafault: timehseet) in web browser
   */
  bool ReportBrowser::Browse(std::string url) {
    AppConfig& config = AppConfig::GetInstance();
    std::string report_file_path = config.GetReportFilePath();

    if (url.empty()) url = "file://" + report_file_path;

    if (HelperSystem::kOsName == "linux" || HelperSystem::kOsName == "unix") {
      // Linux/Unix
      BrowseOnLinux(url);
    } else if (HelperSystem::kOsName == "macOs") {
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

  /**
   * Open configured task action URL in web browser
   * If in day-scope: all tasks of day
   */
  bool ReportBrowser::BrowseTaskUrl(ReportRenderer::RenderScopes render_scope, int offset, int task_number,
                                    std::string url_command) {
    AppConfig config = AppConfig::GetInstance();

    std::string url_raw = config.GetConfigValue(url_command);
    if (url_raw.empty()) return AppError::PrintError("Invalid URL.");

    if (render_scope == ReportRenderer::RenderScopes::Scope_Day) {
        ReportHtmlParser *parser = new ReportHtmlParser();
        if (!parser->LoadReportHtml()) return false;

        InitScopeFilter(render_scope, offset);
        std::vector<std::string> tasks = parser->GetTasksOfDay(rows_filter_);
        if (tasks.empty()) return AppError::PrintError(
          std::string("No tasks found in day (").append(rows_filter_).append(").").c_str());

        for (auto const& task : tasks) {
          std::cout << task << " ";
          std::string url = HelperString::ReplaceAll(url_raw.c_str(), "#TASK#", task.c_str());
          Browse(url);
        }

        return true;
    }

    std::string task = task_number > 0 ? HelperNumeric::ToString(task_number) : "";

    return Browse(HelperString::ReplaceAll(url_raw, "#TASK#", task.c_str()));
  }

} // namespace timesheetplus
