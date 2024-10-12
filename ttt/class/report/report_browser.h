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

#ifndef TTT_CLASS_REPORT_REPORT_BROWSER_H_
#define TTT_CLASS_REPORT_REPORT_BROWSER_H_

#include <ttt/class/report/report.h>
#include <ttt/class/report/report_renderer.h>
#include <ttt/class/report/report_parser.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace tictac_track {

class ReportBrowser : public Report {
 public:
  // Constructor
  ReportBrowser();

  // Open URL (default: timesheet) in web browser
  static void BrowseTimesheet(std::string url = "");

  // Open configured task action URL in web browser
  static bool BrowseTaskUrl(
      int task_number,
      const std::string& url_command = "url.default");

  // Open configured task action URL in web browser.
  // If in day-scope: all tasks of day
  bool BrowseIssueUrlsInScope(
      ReportRenderer::RenderScopes render_scope =
          ReportRenderer::RenderScopes::Scope_Invalid,
      int offset = 0,
      int task_number = 0,
      const std::string& url_command = "url.default");

 private:
  static void BrowseOnLinux(std::string &url);
  static void BrowseOnMac(std::string &url);
};

}  // namespace tictac_track

#endif  // TTT_CLASS_REPORT_REPORT_BROWSER_H_
