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

#ifndef TTT_CLASS_REPORT_REPORT_DATE_TIME_H_
#define TTT_CLASS_REPORT_REPORT_DATE_TIME_H_

#include <cstring>
#include <string>
#include <utility>
#include <vector>

namespace tictac_track {

class ReportDateTime {
 public:
  ReportDateTime();

  std::string GetDateFormat();

  // Get number of current week of year as string
  std::string GetCurrentWeekOfYear(int offset_weeks = 0);

  // Get current date as string
  std::string GetDateFormatted(int offset_days = 0);

  static std::string GetTimestampForMeta();

  // Get day of week as string, localized into configured language
  std::string GetCurrentDayOfWeek(int offset_days = 0);
  // Get localized name of day of week, from given meta data
  std::string GetWeekdayByMeta(const std::string& meta);

  // Get current time of day as string
  static std::string GetCurrentTime();

  static std::string GetDurationFormatted(
      std::string time_started,
      std::string time_stopped);

  // Check whether date1 is after date2
  static bool IsMetaDateAfter(
      const std::string& meta_date1,
      const std::string& meta_date2);

  // Translate e.g. "Monday" to "Montag", using configured locale language
  std::string TranslateWeekday(const char *weekday_name_en);

 private:
  std::string format_week_of_year_;
  std::string format_date_;
  std::string format_day_of_week_;
  std::string format_time_;

  std::string locale_key_;

  const char *weekDayLabelsDe[7] =
      {"Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag",
       "Sonntag"};
  const char *weekDayLabelsDk[7] =
      {"Mandag", "Tirsdag", "Onsdag", "Torsdag", "Fredag", "Lørdag", "Søndag"};
  const char *weekDayLabelsEn[7] =
      {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday",
       "Sunday"};
  const char *weekDayLabelsEs[7] =
      {"Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado",
       "Domingo"};
  const char *weekDayLabelsFi[7] =
      {"Maanantai", "Tiistai", "Keskiviikko", "Torstai", "Perjantai",
       "Lauantai", "Sunnuntai"};
  const char *weekDayLabelsFr[7] =
      {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"};
  const char *weekDayLabelsHu[7] =
      {"Hétfő", "Kedd", "Szerda", "Csütörtök", "Péntek", "Szombat", "Vasárnap"};
  const char *weekDayLabelsIt[7] =
      {"lunedì", "Martedì", "Mercoledì", "Giovedì", "Venerdì", "Sabato",
       "Domenica"};
  const char *weekDayLabelsLt[7] =
      {"Pirmadienis", "Antradienis", "Trečiadienis", "Ketvirtadienis",
       "Penktadienis", "Šeštadienis", "Sekmadienis"};
  const char *weekDayLabelsLv[7] =
      {"Pirmdiena", "Otrdiena", "Trešdiena", "Ceturtdiena", "Piektdiena",
       "Sestdiena", "Svētdiena"};
  const char *weekDayLabelsNl[7] =
      {"Maandag", "Dinsdag", "Woensdag", "Donderdag", "Vrijdag", "Zaterdag",
       "Zondag"};
  const char *weekDayLabelsNo[7] =
      {"Mandag", "Tirsdag", "Onsdag", "Torsdag", "Fredag", "Lørdag", "Søndag"};
  const char *weekDayLabelsPl[7] =
      {"Poniedziałek", "Wtorek", "środa", "Czwartek", "Piątek", "Sobota",
       "Niedziela"};
  const char *weekDayLabelsPt[7] =
      {"Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado", "Domingo"};
  const char *weekDayLabelsRo[7] =
      {"Luni", "Marti", "Miercuri", "Joi", "Vineri", "Sambata", "Duminica"};
  const char *weekDayLabelsSv[7] =
      {"Måndag", "Tisdag", "Onsdag", "Torsdag", "Fredag", "Lördag", "Söndag"};
  const char *weekDayLabelsTr[7] =
      {"Pazartesi", "Salı", "Çarşamba", "Perşembe", "Cuma", "Cumartesi",
       "Pazar"};
};

}  // namespace tictac_track

#endif  // TTT_CLASS_REPORT_REPORT_DATE_TIME_H_
