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

#ifndef TTT_CLASS_REPORT_REPORT_RECALCULATOR_H_
#define TTT_CLASS_REPORT_REPORT_RECALCULATOR_H_

#include <ttt/class/app/app_config.h>
#include <ttt/class/report/report_crud.h>
#include <ttt/class/report/report_file.h>
#include <ttt/class/report/report_parser.h>

#include <ttt/helper/helper_date_time.h>

#include <map>
#include <string>
#include <utility>

namespace tictac_track {

class ReportRecalculator : public ReportParser {
 public:
  explicit ReportRecalculator(std::string html = "");

  // Recalculate and update title, column titles, durations, day of week,
  // sum task/day, sum day, balance
  static bool RecalculateAndUpdate();
  bool Recalculate();

  void AddToTaskMaps(
      const std::string& task_number,
      int index_row,
      int duration_minutes);

  // Extract start- and end-time and calculate
  // and update duration column in given row from it
  static void CalculateAndUpdateDuration(std::string &html, int row_index);

  // Load report HTML, than update duration
  static std::string CalculateAndUpdateDuration(int row_index);

 private:
  // Map storing per one day: which entry row where tasks listed lastly?
  // this is where their sum is to be put
  std::map<std::string, int> task_in_day_last_index_;

  // Map storing per one day: sum of durations of that task within that day
  std::map<std::string, int> task_in_day_duration_sum_;

  // Update sums of task/day by current task maps
  void UpdateTaskSumsFromMaps(std::string &html);

  void ClearTaskMaps();
};

}  // namespace tictac_track

#endif  // TTT_CLASS_REPORT_REPORT_RECALCULATOR_H_
