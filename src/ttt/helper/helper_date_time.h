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

#ifndef CLASS_TTT_HELPER_DATE_TIME
#define CLASS_TTT_HELPER_DATE_TIME

namespace helper {
namespace DateTime {

const int kMinutesPerDay = 1440;

// Get string of current time, w/ given offset in days added, in given format
extern std::string GetCurrentTimeFormatted(const char *format, int offset_days = 0);

// Adjust date by given number of days
extern void AddDaysToDate(struct tm *date, int days);

extern std::string ReformatDateAsYyyyMmDd(std::string date_first, std::string &source_format);

// Calculate minutes since 0:00 from given formatted time label
extern int GetSumMinutesFromTime(std::string time_str = "", const char *separator = ":");

// Format given amount of minutes like "hh:mm". Hours >= 24 are treated as being in next day (begin from 0 again)
extern std::string GetHoursFormattedFromMinutes(int minutes);

// Get index of day of week: sunday == 0, monday == 1, etc.
extern int GetWeekdayIndexByDate(int year, int month, int day);
// Get index of day of week from english name of weekday: sunday == 0, monday == 1, etc.
int GetWeekdayIndexByName(const char *weekday_name_en);
std::string GetWeekdayEnByIndex(int day_index);

extern bool IsTime(std::string str);

static constexpr const char *FORMAT_TIME = "%H:%M";

} // namespace DateTime
} // namespace helper

#endif
