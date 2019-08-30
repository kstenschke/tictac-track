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

#include <cstring>
#include "app_locale.h"
#include "app_config.h"
#include "../helper/helper_string.h"
#include "../helper/helper_html.h"

namespace tictac_track {
AppLocale &AppLocale::GetInstance() {
  // Instantiated on first use
  static AppLocale instance;

  if (!instance.is_initialized_) instance.Init();

  return instance;
}

void AppLocale::Init() {
  AppConfig config = AppConfig::GetInstance();

  locale_key_ = config.GetConfigValue("locale").c_str();
  InitActiveDictionary();
}

void AppLocale::InitActiveDictionary() {
  if (0 == std::strcmp("de", locale_key_)) active_dictionary_ = dictionary_de_;
  else if (0 == std::strcmp("dk", locale_key_)) active_dictionary_ = dictionary_dk_;
  else if (0 == std::strcmp("es", locale_key_)) active_dictionary_ = dictionary_es_;
  else if (0 == std::strcmp("fi", locale_key_)) active_dictionary_ = dictionary_fi_;
  else if (0 == std::strcmp("fr", locale_key_)) active_dictionary_ = dictionary_fr_;
  else if (0 == std::strcmp("hu", locale_key_)) active_dictionary_ = dictionary_hu_;
  else if (0 == std::strcmp("it", locale_key_)) active_dictionary_ = dictionary_it_;
  else if (0 == std::strcmp("lv", locale_key_)) active_dictionary_ = dictionary_lv_;
  else if (0 == std::strcmp("nl", locale_key_)) active_dictionary_ = dictionary_nl_;
  else if (0 == std::strcmp("no", locale_key_)) active_dictionary_ = dictionary_no_;
  else if (0 == std::strcmp("pl", locale_key_)) active_dictionary_ = dictionary_pl_;
  else if (0 == std::strcmp("pt", locale_key_)) active_dictionary_ = dictionary_pt_;
  else if (0 == std::strcmp("ro", locale_key_)) active_dictionary_ = dictionary_ro_;
  else if (0 == std::strcmp("sv", locale_key_)) active_dictionary_ = dictionary_sv_;
  else if (0 == std::strcmp("tr", locale_key_)) active_dictionary_ = dictionary_tr_;
  else active_dictionary_ = dictionary_en_;
}

bool AppLocale::IsSupportedLanguageKey(std::string language_key) {
  return helper::String::Contains(supported_locale_keys_, language_key);
}

const char *AppLocale::Translate(const char *label) {
  std::map<std::string, std::string>::const_iterator pos = active_dictionary_.find(label);
  return pos == active_dictionary_.end() ? label : pos->second.c_str();
}

/**
 * Translate and encode HTML entities
 */
std::string AppLocale::Translate(const char *label, bool html_encode) {
  std::string translated = Translate(label);

  return helper::Html::Encode(translated);
}
} // namespace tictac_lib
