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

#ifndef TICTAC_TRACK_TTT_CLASS_REPORT_BACKUP_MANAGER_H_
#define TICTAC_TRACK_TTT_CLASS_REPORT_BACKUP_MANAGER_H_

#include <ttt/class/app/app_commands.h>
#include <ttt/class/app/app_config.h>
#include <ttt/class/report/report_file.h>

#include <iostream>
#include <string>

namespace tictac_track {

class ReportBackup {
 public:
  static bool BackupReportBeforeProcessCommand(AppCommand::Commands kCommand);

  // Remove timesheet.html.bak, rename timesheet.html.bak to timesheet.html.bak
  static bool ActivateTemporaryBackup();

  // Remove timesheet.html.bak.tmp
  static bool RemoveTemporaryBackup();

  // Overwrite timesheet.html with timesheet.html.bak
  static bool RestoreBackup();

  // Backup timesheet.html to timesheet.html.bak.tmp
  static bool BackupReportTemporary();
};

}  // namespace tictac_track

#endif //TICTAC_TRACK_TTT_CLASS_REPORT_BACKUP_MANAGER_H_
