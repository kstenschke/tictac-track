/*
  Copyright (c) 2018-2020, Kay Stenschke
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

#ifndef TTT_CLASS_REPORT_REPORT_CRUD_H_
#define TTT_CLASS_REPORT_REPORT_CRUD_H_

#include <ttt/class/report/report_file.h>
#include <ttt/class/report/report_date_time.h>

#include <cstring>
#include <iostream>
#include <string>

namespace tictac_track {

class ReportCrud : public ReportFile {
 public:
  // Get object instance. Initialize at 1st call
  static ReportCrud &GetInstance(bool clear = false);

  bool ReportExists();

  // Add timesheet entry: start of work w/ comment
  bool StartEntry(const char *comment = "", const char *task_number = "");
  bool StartEntry(const char *comment = "", int task_number = 0);

  // Add end-time to ongoing entry, append comment if any
  bool StopEntry(const char *comment = "");

  static bool AddFullDayEntry(
      int offset_days = 0,
      std::string comment = "",
      const std::string& task_number = "");

  // Append given text to comment of given or latest entry
  static bool AppendComment(
      std::string &comment,
      int row_index = -1,
      bool starts_with_space = false);

  // Set task number of latest or given entry
  static bool UpdateIssueNumber(int task_number, int row_index = -1);

  // Insert stopped entry w/ given attributes, after entry w/ given index.
  // If given index == -1: insert before 1st entry
  static bool InsertEntryAfter(
      std::string html,
      int row_index,
      std::string meta,
      const std::string& week_number,
      const std::string& weekday,
      const std::string& day_date,
      const std::string& time_start,
      const std::string& time_end,
      const std::string& task_number,
      const std::string& comment = "");

  // Merge (time, task, comment of) given entry w/ next one
  // (and remove next one)
  static bool Merge(int row_index);

  // Remove given amount of latest entries (but not more than exist)
  static bool RemoveEntries(int amount);

  // Remove 1 entry at given index
  static bool RemoveEntryById(int id);

  static bool RemoveEntryById(std::string &html, int id);

  static bool Reset();

  static bool IsAnyEntryOngoing();

  bool CurrentDayHasTasks();

 private:
  enum EntryStatus {
    Status_Started,
    Status_Stopped
  };

  ReportDateTime *report_date_time_;

  bool is_initialized_ = false;

  bool report_exists_ = false;

  // Prevent construction from outside (singleton)
  ReportCrud() = default;

  // Create initial report file if missing.
  void Init(bool clear);
  static bool EnsureReportExists(bool clear);

  static std::string RenderEntryHtml(
      bool is_new_day,
      const std::string& meta,
      const std::string& week_number,
      const std::string& weekday,
      const std::string& day_date,
      const std::string& time_start,
      const std::string& time_end,
      const std::string& task_number,
      const std::string& comment,
      const std::string& duration = "",
      const std::string& task_day_total = "",
      const std::string& day_total = "",
      const std::string& balance = "");

  // Insert/update timesheet entry to report file
  bool UpsertEntry(
      EntryStatus status,
      const char *comment = "",
      const char *task_number = "");

  // Update ongoing entry: add to comment, set stop-time, set stopped
  static void UpdateOngoingEntry(
      std::string &html,
      const std::string& add_to_comment,
      bool do_stop,
      const std::string& time_stopped);

  static bool IsMergeableAmountMinutes(int amount_minutes);

  static void SafeguardToAddIssueNumber();
  static void SafeguardToAddComment(std::string html);
};

}  // namespace tictac_track

#endif  // TTT_CLASS_REPORT_REPORT_CRUD_H_
