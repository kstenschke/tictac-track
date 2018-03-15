
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstring>
#include <string>
#include <codecvt>
#include <locale>
#include <vector>
#include <sstream>
#include <algorithm>
#include "helper_string.h"

namespace timesheetplus {
/**
 * Check whether given string starts w/ given prefix
 */
bool HelperString::StartsWith(const char *str, const char *prefix) {
  return 0 == strncmp(str, prefix, strlen(prefix));
}
/**
 * Check whether given string ends w/ given string
 */
bool HelperString::EndsWith(std::string const &value, std::string const &ending) {
  return ending.size() > value.size()
         ? false
         : std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

/**
 * Check whether given haystack contains given straw
 */
bool HelperString::Contains(std::string &haystack, std::string &needle) {
  return std::string::npos != haystack.find(needle);
}
bool HelperString::Contains(std::string &haystack, const char *needle) {
  return std::string::npos != haystack.find(needle);
}
bool HelperString::Contains(const char *haystack, std::string &needle) {
  return std::string::npos != std::string(haystack, strlen(haystack)).find(needle);
}
bool HelperString::Contains(char *haystack, const char *needle) {
  return std::string::npos != std::string(haystack, strlen(haystack)).find(needle);
}

/**
 * Get UTF-8 string length (special chars like umlauts would otherwise be two bytes, not one)
 */
int HelperString::GetUtf8Size(std::string str) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  std::wstring wide = converter.from_bytes(str);

  return static_cast<int>(std::strlen(str.c_str()));
}

/**
 * Get amount of sub string occurrences
 */
int HelperString::GetSubStrCount(const char *str, const char *sub) {
  auto length = static_cast<int>(strlen(sub));
  if (length == 0) return 0;

  int count = 0;
  for (str = strstr(str, sub); str; str = strstr(str + length, sub)) {
    ++count;
  }

  return count;
}

/**
 * Replace all needle occurrences in haystack
 */
std::string HelperString::ReplaceAll(const char *haystack, const char *needle, const char *replacement) {
  std::string str = std::string(haystack);
  return ReplaceAll(str, needle, replacement);
}

std::string HelperString::ReplaceAll(std::string &haystack, const char *needle, const char *replacement) {
  size_t needle_len = strlen(needle);

  size_t index = 0;
  while (true) {
    index = haystack.find(needle, index);
    if (std::string::npos == index) break;

    haystack.replace(index, needle_len, replacement);
  }
  // TODO ensure avoid endless looping (when e.g. replacing "a" by "aa")
  return haystack;
}

void HelperString::ReplaceAllByReference(std::string &str, const char *needle, const char *replacement) {
  std::string from = std::string(needle);
  std::string to = std::string(replacement);

  if (str.empty() || to.empty()) return;

  std::string::size_type start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
  }
}

/**
 * Encode text as CSV value
 */
std::string HelperString::CsvEncode(std::string str) {
  ReplaceAllByReference(str, "\"", "\"\"");
  ReplaceAllByReference(str, "'", "''");
  ReplaceAllByReference(str, "\\", "\\\\");

  return str;
}

/**
 *  Get sub string inbetween given surrounding left- and right-hand-side delimiters
 */
std::string HelperString::GetSubStrBetween(std::string &str, const char *lhs, const char *rhs) {
  size_t offsetStart = str.find(lhs);
  if (std::string::npos != offsetStart) {
    size_t offsetEnd = str.find(rhs, offsetStart);
    if (std::string::npos != offsetEnd) {
      return str.substr(offsetStart, offsetEnd - offsetStart + strlen(rhs));
    }
  }

  return "";
}

/**
 * Split given string by given character delimiter into vector of strings
 */
std::vector<std::string> HelperString::Explode(std::string const &str, char delimiter) {
  std::vector<std::string> result;
  std::istringstream iss(str);

  for (std::string token; std::getline(iss, token, delimiter);) {
    result.push_back(std::move(token));
  }

  return result;
}

bool HelperString::IsNumeric(const std::string &str, bool allow_negative) {
  if (str.empty()) return false;

  std::string::const_iterator it = str.begin();
  int index = 0;
  while (it != str.end() && (std::isdigit(*it) || (index == 0 && str[0] == '-'))) {
    ++it;
    index++;
  }

  return it == str.end();
}

int HelperString::ToInt(const char *str, int defaultValue) {
  if (!HelperString::IsNumeric(str)) return defaultValue;

  return std::stoi(str);
}
int HelperString::ToInt(std::string str, int defaultValue) {
  if (!HelperString::IsNumeric(str)) return defaultValue;

  return std::stoi(str);
}

/**
 * Get amount characters in given string, counting wide-ascii characters as 1 character (not 2 or 3)
 */
int HelperString::GetAmountChars(std::string str) {
  // Reduce extended ascii characters to 1 character: a
  std::wstring wide_str = std::wstring(str.begin(), str.end());
  auto str_len = static_cast<int>(wide_str.size());
  for (int i = str_len; i >= 0; i--) {
    char ch = str[i];
    if (ch & 128) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "missing_default_case"
      switch ((int) str[i - 1]) {
        case -49:if ((int) str[i] >= -127 && (int) str[i] <= -123) wide_str = ReduceWideChar(wide_str, str_len, i);
          break;
        case -50:if ((int) str[i] >= -93 && (int) str[i] <= -65) wide_str = ReduceWideChar(wide_str, str_len, i);
          break;
        case -53:if ((int) str[i] >= -122 && (int) str[i] <= -100) wide_str = ReduceWideChar(wide_str, str_len, i);
          break;
        case -59:if ((int) str[i] >= -110 && (int) str[i] <= -72) wide_str = ReduceWideChar(wide_str, str_len, i);
          break;
        case -61:if ((int) str[i] >= -128 && (int) str[i] <= -65) wide_str = ReduceWideChar(wide_str, str_len, i);
          break;
        case -62:if ((int) str[i] >= -95 && (int) str[i] <= -65) wide_str = ReduceWideChar(wide_str, str_len, i);
          break;
        case -126:
          switch ((int) str[i]) {
            case -84:if ((int) str[i - 2] == -30) wide_str = ReduceWideChar(wide_str, str_len, i, i - 2, i + 1);
              break;
          }
          break;
        case -129:
          switch ((int) str[i]) {
            case -71:if ((int) str[i - 2] >= -30 && (int) str[i - 2] <= -29) wide_str = ReduceWideChar(wide_str,
                                                                                                       str_len,
                                                                                                       i,
                                                                                                       i - 2,
                                                                                                       i + 1);
              break;
            case -80:if ((int) str[i - 2] == -30) wide_str = ReduceWideChar(wide_str, str_len, i, i - 2, i + 1);
              break;
          }
          break;
      }
#pragma clang diagnostic pop
    }
  }

  str = std::string(wide_str.begin(), wide_str.end());

  return static_cast<int>(str.size());
}

std::wstring HelperString::ReduceWideChar(std::wstring wstr,
                                          int str_len,
                                          int offset,
                                          int offset_left,
                                          int offset_right) {
  return offset < str_len
         ? wstr.substr(0, offset_left) + L"X" + wstr.substr(offset_right, std::string::npos)
         : wstr.substr(0, offset_left) + L"x";
}

std::wstring HelperString::ReduceWideChar(std::wstring wstr, int str_len, int offset) {
  return ReduceWideChar(std::move(wstr), str_len, offset, offset - 1, offset + 1);
}

/**
 * Trim from start (in place)
 */
void HelperString::LTrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                  std::not1(std::ptr_fun<int, int>(std::isspace))));
}

/**
 * Trim from end (in place)
 */
void HelperString::RTrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

/**
 * Trim from both ends (in place)
 */
void HelperString::Trim(std::string &s) {
  LTrim(s);
  RTrim(s);
}
} // namespace timesheetplus
