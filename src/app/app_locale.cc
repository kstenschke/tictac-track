
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstring>
#include "app_locale.h"
#include "config.h"
#include "../helper/helper_string.h"
#include "../helper/html.h"

namespace timesheetplus {
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
  else if (0 == std::strcmp("pl", locale_key_)) active_dictionary_ = dictionary_pl_;
  else if (0 == std::strcmp("pt", locale_key_)) active_dictionary_ = dictionary_pt_;
  else if (0 == std::strcmp("ro", locale_key_)) active_dictionary_ = dictionary_ro_;
  else if (0 == std::strcmp("sv", locale_key_)) active_dictionary_ = dictionary_sv_;
  else if (0 == std::strcmp("tr", locale_key_)) active_dictionary_ = dictionary_tr_;
  else active_dictionary_ = dictionary_en_;
}

bool AppLocale::IsSupportedLanguageKey(std::string language_key) {
  return HelperString::Contains(supported_locale_keys_, language_key);
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

  return HelperHtml::Encode(translated);
}
} // namespace timesheetplus
