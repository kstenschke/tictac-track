/*
  Copyright (c) 2018, Kay Stenschke
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

/**
 * Wait for key-press: ENTER. CTRL+c aborts
 */
void System::WaitForEnterKeyPress() {
  // Suppress echo, go to RAW mode
  system("stty -echo");
  system("stty cbreak");

  int ch = getchar();
  while (10 != ch) {
    ch = getchar();
  }

  // Make echo work, go to COOKED mode
  system("stty echo");
  system("stty -cbreak");
}

void System::ClearConsole() {
  system("clear");
}
} // namespace helper

