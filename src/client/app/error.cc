
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include "error.h"

namespace tictac_track {

bool AppError::PrintError(const char *message) {
  std::cout << message << "\n";
  return false;
}

bool AppError::PrintError(const char *message, int n) {
  std::cout << message << n << "\n";
  return false;
}
} // namespace tictac_track
