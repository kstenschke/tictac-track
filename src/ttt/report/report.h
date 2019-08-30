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

#ifndef CLASS_TTT_REPORT
#define CLASS_TTT_REPORT

#include "report_date_time.h"

namespace tictac_track {
class Report {
 public:
  enum RenderScopes {
    Scope_All,
    Scope_Day,
    Scope_Month,
    Scope_Week,
    Scope_Invalid
  };

  enum ColumnIndexes {
    Index_Meta = 0,
    Index_Week = 1,
    Index_Day = 2,
    Index_Date = 3,
    Index_Start = 4,
    Index_End = 5,
    Index_Task = 6,
    Index_Comment = 7,
    Index_Duration = 8,
    Index_SumTaskDay = 9,
    Index_SumDay = 10,
    Index_Balance = 11
  };

 protected:
  // Render scope (all or filtered by current: day / week / month)
  int render_scope_;
  std::string rows_filter_ = "";

  ReportDateTime report_date_time_instance_;

  void InitScopeFilter(int scope, int offset);
  void InitScopeFilter(int offset = 0);

 private:
  // Get filter value: string that row must contain for being display w/ active day- / week- / month- filter
  std::string GetFilterValueForDayScope(int offset = 0);
  std::string GetFilterValueForWeekScope(int offset = 0);
  std::string GetFilterValueForMonthScope(int offset = 0);
};
} // namespace tictac_lib

#endif
