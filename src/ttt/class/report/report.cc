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

#include <string>
#include "report.h"

namespace tictac_track {

/**
 * Init filter value for time scope (all rows or filtered by: day / week / month)
 */
void Report::InitScopeFilter(int scope, int offset) {
  render_scope_ = scope;
  InitScopeFilter(offset);
}

void Report::InitScopeFilter(int offset) {
  switch (render_scope_) {
    case Scope_Day:rows_filter_ = GetFilterValueForDayScope(offset);
      break;
    case Scope_Week:rows_filter_ = GetFilterValueForWeekScope(offset);
      break;
    case Scope_Month:rows_filter_ = GetFilterValueForMonthScope(offset);
      break;
    case Scope_Invalid:
    default:
//        std::cout << "Invalid scope";
      break;
  }
}

std::string Report::GetFilterValueForDayScope(int offset) {
  return report_date_time_instance_.GetDateFormatted(offset);
}

std::string Report::GetFilterValueForWeekScope(int offset) {
  return report_date_time_instance_.GetCurrentWeekOfYear(offset);
}

std::string Report::GetFilterValueForMonthScope(int offset) {
  return report_date_time_instance_.GetDateFormatted(offset);
}
} // namespace tictac_track
