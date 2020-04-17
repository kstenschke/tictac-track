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

#ifndef TTT_CLASS_APP_APP_LOCALE_H_
#define TTT_CLASS_APP_APP_LOCALE_H_

#include <ttt/class/app/app_config.h>
#include <ttt/helper/helper_html.h>

#include <string>
#include <map>

namespace tictac_track {

class AppLocale {
 public:
  const char *locale_key_{};

  static AppLocale &GetInstance();

  bool IsSupportedLanguageKey(std::string language_key);

  // Translate and encode HTML entities
  const char *Translate(const char *label);
  std::string Translate(const char *label, bool html_encode);

 private:
  std::map<std::string, std::string> active_dictionary_;
  bool is_initialized_ = false;

  // Prevent construction from outside (singleton)
  AppLocale() = default;;

  void Init();
  std::map<std::string, std::string> GetDictionaryByLocaleKey();

  std::string supported_locale_keys_ =
      "de,dk,en,es,fi,fr,gr,hu,it,lv,nl,no,pl,pt,ro,ru,sv,tr";

  std::map<std::string, std::string> dictionary_de_ = {
      {"balance", "Saldo"},
      {"comment", "Kommentar"},
      {"date", "Datum"},
      {"day", "Tag"},
      {"duration", "Dauer"},
      {"end", "Ende"},
      {"start", "Start"},
      {"task", "Task"},
      {"timesheet", "Stundenblatt"},
      {"week", "KW"}};

  std::map<std::string, std::string> dictionary_dk_ = {
      {"balance", "Balance"},
      {"comment", "Kommentar"},
      {"date", "Dato"},
      {"day", "Dag"},
      {"duration", "Varighed"},
      {"end", "Ende"},
      {"start", "Start"},
      {"task", "Opgave"},
      {"timesheet", "Timer ark"},
      {"week", "Uge"}};

  std::map<std::string, std::string> dictionary_en_ = {
      {"balance", "Balance"},
      {"comment", "Comment"},
      {"date", "Date"},
      {"day", "Day"},
      {"duration", "Duration"},
      {"start", "Start"},
      {"end", "End"},
      {"task", "Task"},
      {"timesheet", "Timesheet"},
      {"week", "Week"}};

  std::map<std::string, std::string> dictionary_es_ = {
      {"balance", "Equilibrio"},
      {"comment", "Comentario"},
      {"date", "Fecha"},
      {"day", "Día"},
      {"duration", "Duración"},
      {"start", "Comienzo"},
      {"end", "Fin"},
      {"task", "Tarea"},
      {"timesheet", "Parte de horas"},
      {"week", "Semana"}};

  std::map<std::string, std::string> dictionary_fi_ = {
      {"balance", "Tasapaino"},
      {"comment", "Kommentti"},
      {"date", "Päiväys"},
      {"day", "Päivä"},
      {"duration", "Duración"},
      {"start", "Alku"},
      {"end", "Kuin"},
      {"task", "Tehtävä"},
      {"timesheet", "Tuntiarkki"},
      {"week", "Viikko"}};

  std::map<std::string, std::string> dictionary_fr_ = {
      {"balance", "Équilibre"},
      {"comment", "Commentaire"},
      {"date", "Date"},
      {"day", "Jour"},
      {"duration", "Durée"},
      {"start", "Début"},
      {"end", "Fin"},
      {"task", "Tâche"},
      {"timesheet", "Feuille de présence"},
      {"week", "Semaine"}};

  std::map<std::string, std::string> dictionary_hu_ = {
      {"balance", "Egyensúly"},
      {"comment", "Megjegyzés"},
      {"date", "Dátum"},
      {"day", "Nap"},
      {"duration", "Tartam"},
      {"start", "Rajt"},
      {"end", "Vég"},
      {"task", "Feladat"},
      {"timesheet", "Jelenléti ív"},
      {"week", "Hét"}};

  std::map<std::string, std::string> dictionary_it_ = {
      {"balance", "Equilibrio"},
      {"comment", "Commento"},
      {"date", "Data"},
      {"day", "Giorno"},
      {"duration", "Durata"},
      {"start", "Inizio"},
      {"end", "Fine"},
      {"task", "Compito"},
      {"timesheet", "Timesheet"},
      {"week", "Settimana"}};

  std::map<std::string, std::string> dictionary_lv_ = {
      {"balance", "Atlikums"},
      {"comment", "Komentārs"},
      {"date", "Datums"},
      {"day", "Diena"},
      {"duration", "Ilgums"},
      {"start", "Sākt"},
      {"end", "Beigt"},
      {"task", "Uzdevums"},
      {"timesheet", "Laika kontrolsaraksts"},
      {"week", "Nedēļa"}};

  std::map<std::string, std::string> dictionary_nl_ = {
      {"balance", "Balans"},
      {"comment", "Commentaar"},
      {"date", "Datum"},
      {"day", "Dag"},
      {"duration", "Looptijd"},
      {"start", "Begin"},
      {"end", "Einde"},
      {"task", "Taak"},
      {"timesheet", "Rooster"},
      {"week", "Week"}};

  std::map<std::string, std::string> dictionary_no_ = {
      {"balance", "Balanse"},
      {"comment", "Kommentar"},
      {"date", "Dato"},
      {"day", "Dag"},
      {"duration", "Varighet"},
      {"start", "Start"},
      {"end", "Slutt"},
      {"task", "Oppgave"},
      {"timesheet", "Tids Skjema"},
      {"week", "Uke"}};

  std::map<std::string, std::string> dictionary_pl_ = {
      {"balance", "Równowaga"},
      {"comment", "Kommentarz"},
      {"date", "Data"},
      {"day", "Dzień"},
      {"duration", "Trwanie"},
      {"start", "Początek"},
      {"end", "Koniec"},
      {"task", "Zadanie"},
      {"timesheet", "Lista obecności"},
      {"week", "Tydzień"}};

  std::map<std::string, std::string> dictionary_pt_ = {
      {"balance", "Equilíbrio"},
      {"comment", "Comente"},
      {"date", "Encontro"},
      {"day", "Dia"},
      {"duration", "Duração"},
      {"start", "Começar"},
      {"end", "Fim"},
      {"task", "Tarefa"},
      {"timesheet", "Planilha de horário"},
      {"week", "Semana"}};

  std::map<std::string, std::string> dictionary_ro_ = {
      {"balance", "Echilibru"},
      {"comment", "Cometariu"},
      {"date", "Data"},
      {"day", "Zi"},
      {"duration", "Durată"},
      {"start", "Start"},
      {"end", "Sfârşit"},
      {"task", "Sarcină"},
      {"timesheet", "Pontaj"},
      {"week", "Săptămână"}};

  std::map<std::string, std::string> dictionary_sv_ = {
      {"balance", "Balans"},
      {"comment", "Kommentar"},
      {"date", "Datum"},
      {"day", "Dag"},
      {"duration", "Varaktighet"},
      {"start", "Start"},
      {"end", "Slutet"},
      {"task", "Uppgift"},
      {"timesheet", "Tidrapport"},
      {"week", "Vecka"}};

  std::map<std::string, std::string> dictionary_tr_ = {
      {"balance", "Denge"},
      {"comment", "Yorum Yap"},
      {"date", "Tarih"},
      {"day", "Gün"},
      {"duration", "Süre"},
      {"start", "Başla"},
      {"end", "Son"},
      {"task", "Görev"},
      {"timesheet", "Zaman planı"},
      {"week", "Hafta"}};
};

}  // namespace tictac_track

#endif  // TTT_CLASS_APP_APP_LOCALE_H_
