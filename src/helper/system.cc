
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <clocale>
#include "system.h"
#include "helper_string.h"

namespace timesheetplus {
/**
 * Get language key from system default locale
 */
std::string HelperSystem::GetLanguageKey() {
  setlocale(LC_ALL, "");

  return HelperString::Explode(setlocale(LC_ALL, nullptr), '_')[0];
}
} // namespace timesheetplus

