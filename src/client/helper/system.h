
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_HELPER_SYSTEM
#define CLASS_TIMETRACKERCL_HELPER_SYSTEM

namespace tictac_track {
namespace HelperSystem {
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
#elif __unix__ // all unices not caught above
const std::string kOsName = "unix";
const std::string kNewline = "\n";
#else
const std::string kOsName = "unknown";
const std::string kNewline = "\n";
#endif

// Get language key from system default locale
extern std::string GetLanguageKey();

bool GetYesOrNoKeyPress();

}
} // namespace tictac_track

#endif
