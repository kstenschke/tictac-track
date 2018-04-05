
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <fstream>
#include <vector>
#include <zconf.h>
#include "helper_file.h"

namespace helper {
/**
 * Check whether given file exists
 */
bool File::FileExists(const std::string &name) {
//    if (access(name.c_str(), F_OK) == -1) std::cout << "Report file \"" << name << "\" does not exist.\n";
  return access(name.c_str(), F_OK) != -1;
}

/**
 * Get contents of given file
 */
std::string File::GetFileContents(std::string &filename) {
  std::ifstream file(filename);

  return GetFileContents(file);
}

std::string File::GetFileContents(std::ifstream &file) {
  // Get filesize
  file.seekg(0, std::ios::end);
  std::streampos length = file.tellg();
  file.seekg(0, std::ios::beg);

  // Read the whole file into the buffer
  std::vector<char> buffer(static_cast<unsigned long>(length));
  file.read(&buffer[0], length);

  std::string str(buffer.begin(), buffer.end());

  return str;
}

bool File::WriteToNewFile(const std::string &filename, std::string &content) {
  std::ofstream outfile(filename);
  outfile << content;
  outfile.close();

  return File::FileExists(filename);
}
} // namespace helper
