
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_HELPER_STRING
#define CLASS_TIMETRACKERCL_HELPER_STRING

#include <vector>

namespace tictac_track {
namespace HelperString {
// Check whether given string ends w/ given string
bool EndsWith(std::string const &value, std::string const &ending);
// Check whether given string starts w/ given prefix
extern bool StartsWith(const char *str, const char *prefix);

// Check whether given haystack contains given straw
extern bool Contains(std::string &haystack, std::string &needle);
extern bool Contains(std::string &haystack, const char *needle);
extern bool Contains(const char *haystack, std::string &needle);
extern bool Contains(char *haystack, const char *needle);

// Get utf8 string length (special chars like umlauts would otherwise be two bytes, not one)
extern int GetUtf8Size(std::string str);

// Get amount of sub string occurrences
extern int GetSubStrCount(const char *str, const char *sub);

// Replace all needle occurrences in haystack
extern std::string ReplaceAll(const char *haystack, const char *needle, const char *replacement);
extern std::string ReplaceAll(std::string &haystack, const char *needle, const char *replacement);
extern void ReplaceAllByReference(std::string &str, const char *from, const char *to);

extern std::string CsvEncode(std::string str);

// Get sub string between given surrounding left- and right-hand-side delimiters
extern std::string GetSubStrBetween(std::string &str, const char *lhs, const char *rhs);

// Split given string by given character delimiter into vector of strings
extern std::vector<std::string> Explode(std::string const &str, char delimiter);

extern bool IsNumeric(const std::string &str, bool allow_negative = false);

extern int ToInt(const char *str, int defaultValue = 0);
extern int ToInt(std::string str, int defaultValue = 0);

// Get amount characters in given string, counting wide-ascii characters as 1 character (not 2 or 3)
extern int GetAmountChars(std::string str);
extern std::wstring ReduceWideChar(std::wstring wstr, int str_len, int offset, int offset_left, int offset_right);
extern std::wstring ReduceWideChar(std::wstring wstr, int str_len, int offset);

extern void LTrim(std::string &s);
extern void RTrim(std::string &s);
extern void Trim(std::string &s);
}
} // namespace tictac_track

#endif
