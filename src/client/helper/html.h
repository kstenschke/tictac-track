
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_HELPER_HTML
#define CLASS_TIMETRACKERCL_HELPER_HTML

namespace tictac_track {
namespace HelperHtml {
extern std::string Encode(std::string str);
extern std::wstring ReplaceWideCharByEntity(std::wstring wstr, int str_len, int offset, int offset_left,
                                            int offset_right, std::wstring replacement);
extern std::wstring ReplaceWideCharByEntity(std::wstring wstr, int str_len, int offset, std::wstring replacement);

extern std::string Decode(std::string str);
}
} // namespace tictac_track

#endif
