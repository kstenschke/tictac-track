
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_REPORT_DATE_TIME
#define CLASS_TIMETRACKERCL_REPORT_DATE_TIME

#include <string>

namespace tictac_track {
  class ReportDateTime {
    public:
      // Constructor
      ReportDateTime();

      std::string GetDateFormat();

      // Get number of current week of year as string
      std::string GetCurrentWeekOfYear(int offset_weeks = 0);

      // Get current date as string
      std::string GetCurrentDate(int offset_days = 0);

      std::string GetTimestampForMeta();

      // Get day of week as string, localized into configured language
      std::string GetCurrentDayOfWeek();
      // Get localized name of day of week, from given meta data
      std::string GetWeekdayByMeta(std::string meta);

      // Get current time of day as string
      std::string GetCurrentTime();

      static std::string GetDurationFormatted(std::string time_started, std::string time_stopped);

      // Check whether date1 is after date2
      static bool IsMetaDateAfter(std::string meta_date1, std::string meta_date2);

      // Translate e.g. "Monday" to "Montag", using configured locale language
      std::string TranslateWeekday(const char* weekday_name_en);

    private:
      std::string format_week_of_year_;
      std::string format_date_;
      std::string format_day_of_week_;
      std::string format_time_;

      std::string locale_key_;

      const char* weekDayLabelsDe[7] = {"Montag",       "Dienstag",    "Mittwoch",     "Donnerstag",     "Freitag",      "Samstag",     "Sonntag"};
      const char* weekDayLabelsDk[7] = {"Mandag",       "Tirsdag",     "Onsdag",       "Torsdag",        "Fredag",       "Lørdag",      "Søndag"};
      const char* weekDayLabelsEn[7] = {"Monday",       "Tuesday",     "Wednesday",    "Thursday",       "Friday",       "Saturday",    "Sunday"};
      const char* weekDayLabelsEs[7] = {"Lunes",        "Martes",      "Miércoles",    "Jueves",         "Viernes",      "Sábado",      "Domingo"};
      const char* weekDayLabelsFi[7] = {"Maanantai",    "Tiistai",     "Keskiviikko",  "Torstai",        "Perjantai",    "Lauantai",    "Sunnuntai"};
      const char* weekDayLabelsFr[7] = {"Lundi",        "Mardi",       "Mercredi",     "Jeudi",          "Vendredi",     "Samedi",      "Dimanche"};
      const char* weekDayLabelsHu[7] = {"Hétfő",        "Kedd",        "Szerda",       "Csütörtök",      "Péntek",       "Szombat",     "Vasárnap"};
      const char* weekDayLabelsIt[7] = {"lunedì",       "Martedì",     "Mercoledì",    "Giovedì",        "Venerdì",      "Sabato",      "Domenica"};
      const char* weekDayLabelsLt[7] = {"Pirmadienis",  "Antradienis", "Trečiadienis", "Ketvirtadienis", "Penktadienis", "Šeštadienis", "Sekmadienis"};
      const char* weekDayLabelsLv[7] = {"Pirmdiena",    "Otrdiena",    "Trešdiena",    "Ceturtdiena",    "Piektdiena",   "Sestdiena",   "Svētdiena"};
      const char* weekDayLabelsNl[7] = {"Maandag",      "Dinsdag",     "Woensdag",     "Donderdag",      "Vrijdag",      "Zaterdag",    "Zondag"};
      const char* weekDayLabelsNo[7] = {"Mandag",       "Tirsdag",     "Onsdag",       "Torsdag",        "Fredag",       "Lørdag",      "Søndag"};
      const char* weekDayLabelsPl[7] = {"Poniedziałek", "Wtorek",      "środa",        "Czwartek",       "Piątek",       "Sobota",      "Niedziela"};
      const char* weekDayLabelsPt[7] = {"Segunda",      "Terça",       "Quarta",       "Quinta",         "Sexta",        "Sábado",      "Domingo"};
      const char* weekDayLabelsRo[7] = {"Luni",         "Marti",       "Miercuri",     "Joi",            "Vineri",       "Sambata",     "Duminica"};
      const char* weekDayLabelsSv[7] = {"Måndag",       "Tisdag",      "Onsdag",       "Torsdag",        "Fredag",       "Lördag",      "Söndag"};
      const char* weekDayLabelsTr[7] = {"Pazartesi",    "Salı",        "Çarşamba",     "Perşembe",       "Cuma",         "Cumartesi",   "Pazar"};
  };
} // namespace tictac_lib

#endif
