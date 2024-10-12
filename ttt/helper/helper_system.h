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

#ifndef TTT_HELPER_HELPER_SYSTEM_H_
#define TTT_HELPER_HELPER_SYSTEM_H_

#include <ttt/helper/helper_string.h>

#include <cstring>
#include <clocale>
#include <cstdlib>
#include <sys/ioctl.h>
#include <string>
#include <unistd.h>

namespace helper {
namespace System {

// OS, newline
#ifdef _WIN32
const std::string kOsName = "win32";
const std::string kNewline = "\r\n";
#ifdef _WIN64
const std::string kOsName = "win64";
const std::string kNewline = "\r\n";
#else
const std::string kOsName = "win";
const std::string kNewline = "\r\n";
#endif
#elif __APPLE__
const std::string kOsName = "macOs";
const std::string kNewline = "\n";
#elif __linux__
const std::string kOsName = "linux";
const std::string kNewline = "\n";
#elif __unix__  // all unixes not caught above
const std::string kOsName = "unix";
const std::string kNewline = "\n";
#else
const std::string kOsName = "unknown";
const std::string kNewline = "\n";
#endif

// Get absolute path to application executable
extern std::string GetBinaryPath(char **argv, size_t strLenExecutableName);

// Get amount of columns in current terminal
int GetMaxCharsPerTerminalRow();

// Get language key from system default locale
extern std::string GetLanguageKey();

bool GetYesOrNoKeyPress();
bool GetNoOrYesKeyPress();

void WaitForEnterKeyPress();

}  // namespace System
}  // namespace helper

#endif  // TTT_HELPER_HELPER_SYSTEM_H_
