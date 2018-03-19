
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <clocale>
#include <iostream>
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

/**
 * Wait for keys being pressed: y / Y / n / N / ENTER. CTRL+c aborts
 */
bool HelperSystem::GetYesOrNoKeyPress() {
  // Suppress echo, go to RAW mode
  system("stty -echo");
  system("stty cbreak");

  int ch = getchar();
  while (10 != ch && 78 != ch && 89 != ch && 110 != ch && 121 != ch) {
    ch = getchar();
  }

  // Make echo work, go to COOKED mode
  system("stty echo");
  system("stty -cbreak");

  // Everything but "n" / "N" means yes
  return ch != 78 && ch != 110;
}

} // namespace timesheetplus

