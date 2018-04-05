
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <clocale>
#include <iostream>
#include <cstdlib>
#include "helper_system.h"
#include "helper_string.h"

namespace helper {

/**
 * Get absolute path to application executable
 */
std::string System::GetBinaryPath(char **argv, size_t strLenExecutableName) {
  char *path_relative = argv[0];
  char absolute_path[255];
  char *ptr;
  ptr = realpath(path_relative, absolute_path);

  unsigned long len_without_binary = std::strlen(absolute_path) - strLenExecutableName;

  return std::string(ptr).substr(0, len_without_binary);
}

/**
 * Get language key from system default locale
 */
std::string System::GetLanguageKey() {
  setlocale(LC_ALL, "");

  return String::Explode(setlocale(LC_ALL, nullptr), '_')[0];
}

/**
 * Wait for keys being pressed: y / Y / n / N / ENTER. CTRL+c aborts
 */
bool System::GetYesOrNoKeyPress() {
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
} // namespace helper

