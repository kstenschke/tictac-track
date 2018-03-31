
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_LOCALE
#define CLASS_TIMETRACKERCL_LOCALE

#include <string>
#include <map>

namespace timesheetplus {
  class AppLocale {
    public:
      const char* locale_key_{};

      static AppLocale& GetInstance();

      bool IsSupportedLanguageKey(std::string language_key);

      // Translate and encode HTML entities
      const char* Translate(const char* label);
      std::string Translate(const char* label, bool html_encode);

    private:
      std::map<std::string, std::string> active_dictionary_;
      bool is_initialized_ = false;

      // Prevent construction from outside (singleton)
      AppLocale() = default;;

      void Init();
      void InitActiveDictionary();

      std::string supported_locale_keys_ = "de,dk,en,es,fi,fr,gr,hu,it,lv,nl,no,pl,pt,ro,ru,sv,tr";

      std::map<std::string, std::string> dictionary_de_ = {
        {"balance",        "Saldo"},
        {"comment",        "Kommentar"},
        {"date",           "Datum"},
        {"day",            "Tag"},
        {"duration",       "Dauer"},
        {"end",            "Ende"},
        {"start",          "Start"},
        {"task",           "Task"},
        {"timesheet",      "Stundenblatt"},
        {"week",           "KW"}};

      std::map<std::string, std::string> dictionary_dk_ = {
        {"balance",        "Balance"},
        {"comment",        "Kommentar"},
        {"date",           "Dato"},
        {"day",            "Dag"},
        {"duration",       "Varighed"},
        {"end",            "Ende"},
        {"start",          "Start"},
        {"task",           "Opgave"},
        {"timesheet",      "Timer ark"},
        {"week",           "Uge"}};

      std::map<std::string, std::string> dictionary_en_ = {
        {"balance",        "Balance"},
        {"comment",        "Comment"},
        {"date",           "Date"},
        {"day",            "Day"},
        {"duration",       "Duration"},
        {"start",          "Start"},
        {"end",            "End"},
        {"task",           "Task"},
        {"timesheet",      "Timesheet"},
        {"week",           "Week"}};

      std::map<std::string, std::string> dictionary_es_ = {
        {"balance",        "Equilibrio"},
        {"comment",        "Comentario"},
        {"date",           "Fecha"},
        {"day",            "Día"},
        {"duration",       "Duración"},
        {"start",          "Comienzo"},
        {"end",            "Fin"},
        {"task",           "Tarea"},
        {"timesheet",      "Parte de horas"},
        {"week",           "Semana"}};

      std::map<std::string, std::string> dictionary_fi_ = {
        {"balance",        "Tasapaino"},
        {"comment",        "Kommentti"},
        {"date",           "Päiväys"},
        {"day",            "Päivä"},
        {"duration",       "Duración"},
        {"start",          "Alku"},
        {"end",            "Kuin"},
        {"task",           "Tehtävä"},
        {"timesheet",      "Tuntiarkki"},
        {"week",           "Viikko"}};

      std::map<std::string, std::string> dictionary_fr_ = {
        {"balance",        "Équilibre"},
        {"comment",        "Commentaire"},
        {"date",           "Date"},
        {"day",            "Jour"},
        {"duration",       "Durée"},
        {"start",          "Début"},
        {"end",            "Fin"},
        {"task",           "Tâche"},
        {"timesheet",      "Feuille de présence"},
        {"week",           "Semaine"}};

      std::map<std::string, std::string> dictionary_hu_ = {
        {"balance",        "Egyensúly"},
        {"comment",        "Megjegyzés"},
        {"date",           "Dátum"},
        {"day",            "Nap"},
        {"duration",       "Tartam"},
        {"start",          "Rajt"},
        {"end",            "Vég"},
        {"task",           "Feladat"},
        {"timesheet",      "Jelenléti ív"},
        {"week",           "Hét"}};

      std::map<std::string, std::string> dictionary_it_ = {
        {"balance",        "Equilibrio"},
        {"comment",        "Commento"},
        {"date",           "Data"},
        {"day",            "Giorno"},
        {"duration",       "Durata"},
        {"start",          "Inizio"},
        {"end",            "Fine"},
        {"task",           "Compito"},
        {"timesheet",      "Timesheet"},
        {"week",           "Settimana"}};

      std::map<std::string, std::string> dictionary_lv_ = {
        {"balance",        "Atlikums"},
        {"comment",        "Komentārs"},
        {"date",           "Datums"},
        {"day",            "Diena"},
        {"duration",       "Ilgums"},
        {"start",          "Sākt"},
        {"end",            "Beigt"},
        {"task",           "Uzdevums"},
        {"timesheet",      "Laika kontrolsaraksts"},
        {"week",           "Nedēļa"}};

      std::map<std::string, std::string> dictionary_nl_ = {
        {"balance",        "Balans"},
        {"comment",        "Commentaar"},
        {"date",           "Datum"},
        {"day",            "Dag"},
        {"duration",       "Looptijd"},
        {"start",          "Begin"},
        {"end",            "Einde"},
        {"task",           "Taak"},
        {"timesheet",      "Rooster"},
        {"week",           "Week"}};

      std::map<std::string, std::string> dictionary_no_ = {
        {"balance",        "Balanse"},
        {"comment",        "Kommentar"},
        {"date",           "Dato"},
        {"day",            "Dag"},
        {"duration",       "Varighet"},
        {"start",          "Start"},
        {"end",            "Slutt"},
        {"task",           "Oppgave"},
        {"timesheet",      "Tids Skjema"},
        {"week",           "Uke"}};

      std::map<std::string, std::string> dictionary_pl_ = {
        {"balance",        "Równowaga"},
        {"comment",        "Kommentarz"},
        {"date",           "Data"},
        {"day",            "Dzień"},
        {"duration",       "Trwanie"},
        {"start",          "Początek"},
        {"end",            "Koniec"},
        {"task",           "Zadanie"},
        {"timesheet",      "Lista obecności"},
        {"week",           "Tydzień"}};

      std::map<std::string, std::string> dictionary_pt_ = {
        {"balance",        "Equilíbrio"},
        {"comment",        "Comente"},
        {"date",           "Encontro"},
        {"day",            "Dia"},
        {"duration",       "Duração"},
        {"start",          "Começar"},
        {"end",            "Fim"},
        {"task",           "Tarefa"},
        {"timesheet",      "Planilha de horário"},
        {"week",           "Semana"}};

      std::map<std::string, std::string> dictionary_ro_ = {
        {"balance",        "Echilibru"},
        {"comment",        "Cometariu"},
        {"date",           "Data"},
        {"day",            "Zi"},
        {"duration",       "Durată"},
        {"start",          "Start"},
        {"end",            "Sfârşit"},
        {"task",           "Sarcină"},
        {"timesheet",      "Pontaj"},
        {"week",           "Săptămână"}};

      std::map<std::string, std::string> dictionary_sv_ = {
        {"balance",        "Balans"},
        {"comment",        "Kommentar"},
        {"date",           "Datum"},
        {"day",            "Dag"},
        {"duration",       "Varaktighet"},
        {"start",          "Start"},
        {"end",            "Slutet"},
        {"task",           "Uppgift"},
        {"timesheet",      "Tidrapport"},
        {"week",           "Vecka"}};

      std::map<std::string, std::string> dictionary_tr_ = {
        {"balance",        "Denge"},
        {"comment",        "Yorum Yap"},
        {"date",           "Tarih"},
        {"day",            "Gün"},
        {"duration",       "Süre"},
        {"start",          "Başla"},
        {"end",            "Son"},
        {"task",           "Görev"},
        {"timesheet",      "Zaman planı"},
        {"week",           "Hafta"}};
  };
} // namespace timesheetplus

#endif
