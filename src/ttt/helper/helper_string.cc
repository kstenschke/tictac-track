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
#include <string>
#include <codecvt>
#include <locale>
#include <vector>
#include <sstream>
#include <algorithm>
#include "helper_string.h"

namespace helper {

/**
 * Check whether given string starts w/ given prefix
 */
bool String::StartsWith(const char *str, const char *prefix) {
  return 0 == strncmp(str, prefix, strlen(prefix));
}

/**
 * Check whether given string ends w/ given string
 */
bool String::EndsWith(std::string const &value, std::string const &ending) {
  return ending.size() <= value.size() && std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

/**
 * Check whether given haystack contains given straw
 */
bool String::Contains(std::string &haystack, std::string &needle) {
  return std::string::npos != haystack.find(needle);
}

bool String::Contains(std::string &haystack, const char *needle) {
  return std::string::npos != haystack.find(needle);
}

bool String::Contains(const char *haystack, std::string &needle) {
  return std::string::npos != std::string(haystack, strlen(haystack)).find(needle);
}

bool String::Contains(char *haystack, const char *needle) {
  return std::string::npos != std::string(haystack, strlen(haystack)).find(needle);
}

/**
 * Get UTF-8 string length (special chars like umlauts would otherwise be two bytes, not one)
 */
int String::GetUtf8Size(std::string str) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  std::wstring wide = converter.from_bytes(str);

  return static_cast<int>(std::strlen(str.c_str()));
}

/**
 * Get amount of sub string occurrences
 */
int String::GetSubStrCount(const char *str, const char *sub) {
  auto length = static_cast<int>(strlen(sub));
  if (length == 0) return 0;

  int count = 0;
  for (str = strstr(str, sub); str; str = strstr(str + length, sub)) ++count;

  return count;
}

/**
 * Replace all needle occurrences in haystack
 */
std::string String::ReplaceAll(const char *haystack, const char *needle, const char *replacement) {
  std::string str = std::string(haystack);
  return ReplaceAll(str, needle, replacement);
}

std::string String::ReplaceAll(std::string &haystack, const char *needle, const char *replacement) {
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

void String::ReplaceAllByReference(std::string &str, const char *needle, const char *replacement) {
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
std::string String::CsvEncode(std::string str) {
  ReplaceAllByReference(str, "\"", "\"\"");
  ReplaceAllByReference(str, "'", "''");
  ReplaceAllByReference(str, "\\", "\\\\");

  return str;
}

/**
 *  Get sub string inbetween given surrounding left- and right-hand-side delimiters
 */
std::string String::GetSubStrBetween(std::string &str, const char *lhs, const char *rhs) {
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
std::vector<std::string> String::Explode(std::string const &str, char delimiter) {
  std::vector<std::string> result;
  std::istringstream iss(str);

  for (std::string token; std::getline(iss, token, delimiter);)
    result.push_back(std::move(token));

  return result;
}

bool String::IsNumeric(const std::string &str, bool allow_negative) {
  if (str.empty()) return false;

  std::string::const_iterator it = str.begin();
  int index = 0;
  while (it != str.end() && (std::isdigit(*it) || (index == 0 && str[0] == '-'))) {
    ++it;
    ++index;
  }

  return it == str.end();
}

int String::ToInt(const char *str, int defaultValue) {
  return String::IsNumeric(str) ? std::stoi(str) : defaultValue;
}

int String::ToInt(std::string str, int defaultValue) {
  return String::IsNumeric(str) ? std::stoi(str) : defaultValue;
}

/**
 * Get amount characters in given string, counting wide-ascii characters as 1 character (not 2 or 3)
 */
int String::GetAmountChars(std::string str) {
  // Reduce extended ascii characters to 1 character: a
  std::wstring wide_str = std::wstring(str.begin(), str.end());
  auto str_len = static_cast<int>(wide_str.size());
  for (int i = str_len; i >= 0; i--) {
    char ch = str[i];
    if (ch & 128) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "missing_default_case"
      switch ((int) str[i - 1]) {
        case -49:
          if ((int) str[i] >= -127
              && (int) str[i] <= -123)
            wide_str = ReduceWideChar(wide_str, str_len, i);
          break;
        case -50:
          if ((int) str[i] >= -93
              && (int) str[i] <= -65)
            wide_str = ReduceWideChar(wide_str, str_len, i);
          break;
        case -53:
          if ((int) str[i] >= -122
              && (int) str[i] <= -100)
            wide_str = ReduceWideChar(wide_str, str_len, i);
          break;
        case -59:
          if ((int) str[i] >= -110
              && (int) str[i] <= -72)
            wide_str = ReduceWideChar(wide_str, str_len, i);
          break;
        case -61:
          if ((int) str[i] >= -128
              && (int) str[i] <= -65)
            wide_str = ReduceWideChar(wide_str, str_len, i);
          break;
        case -62:
          if ((int) str[i] >= -95
              && (int) str[i] <= -65)
            wide_str = ReduceWideChar(wide_str, str_len, i);
          break;
        case -126:
          switch ((int) str[i]) {
            case -84:
              if ((int) str[i - 2]==-30)
                wide_str = ReduceWideChar(wide_str, str_len, i, i - 2, i + 1);
              break;
          }
          break;
        case -129:
          switch ((int) str[i]) {
            case -71:
              if ((int) str[i - 2] >= -30
                  && (int) str[i - 2] <= -29)
                wide_str = ReduceWideChar(wide_str, str_len, i, i - 2, i + 1);
              break;
            case -80:
              if ((int) str[i - 2]==-30)
                wide_str = ReduceWideChar(wide_str, str_len, i, i - 2, i + 1);
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

std::wstring String::ReduceWideChar(std::wstring wstr,
                                          int str_len,
                                          int offset,
                                          int offset_left,
                                          int offset_right) {
  return offset < str_len
         ? wstr.substr(0, offset_left) + L"X" + wstr.substr(offset_right, std::string::npos)
         : wstr.substr(0, offset_left) + L"x";
}

std::wstring String::ReduceWideChar(std::wstring wstr, int str_len, int offset) {
  return ReduceWideChar(std::move(wstr), str_len, offset, offset - 1, offset + 1);
}

/**
 * Trim from start (in place)
 */
void String::LTrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                  std::not1(std::ptr_fun<int, int>(std::isspace))));
}

/**
 * Trim from end (in place)
 */
void String::RTrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

/**
 * Trim from both ends (in place)
 */
void String::Trim(std::string &s) {
  LTrim(s);
  RTrim(s);
}
} // namespace helper
