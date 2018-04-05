
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_REPORT_FILE
#define CLASS_TIMETRACKERCL_REPORT_FILE

namespace tictac_track {
  class ReportFile {
    public:
      static const std::string kFilenameReport;

      // Get input stream to report file
      static std::ifstream GetReportIfStream();
      static std::string GetReportHtml();

      // Save given HTML to timesheet, replacing any previous content
      static bool SaveReport(std::string html);

      // Backup timesheet.html to timesheet.html.bak.tmp
      static bool BackupReportTemporary();
      // Remove timesheet.html.bak, rename timesheet.html.bak to timesheet.html.bak
      static bool ActivateTemporaryBackup();
      // Remove timesheet.html.bak.tmp
      static bool RemoveTemporaryBackup();
      // Overwrite timesheet.html with timesheet.html.bak
      static bool RestoreBackup();

    protected:
      // Create initial timesheet HTML file
      static bool InitReportFile(bool removeIfExists);
  };
} // namespace tictac_track

#endif
