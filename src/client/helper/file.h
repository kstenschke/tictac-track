
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_HELPER_FILE
#define CLASS_TIMETRACKERCL_HELPER_FILE

namespace tictac_track {
namespace HelperFile {
// Check whether given file exists
extern bool FileExists(const std::string &name);

// Get contents of given file
extern std::string GetFileContents(std::string &filename);
extern std::string GetFileContents(std::ifstream &file);

extern bool WriteToNewFile(const std::string &filename, std::string &content);
}
} // namespace tictac_track

#endif
