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

#include <vendor/zlib/zconf.h>
#include <ttt/helper/helper_file.h>

namespace helper {
// Check whether given file exists
bool File::FileExists(const std::string &name) {
  return access(name.c_str(), F_OK) != -1;
}

// Get contents of given file
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

bool File::Remove(const char *file_path) {
  return remove(file_path) == 0;
}

}  // namespace helper
