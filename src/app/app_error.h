
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_APP_ERROR
#define CLASS_TIMETRACKERCL_APP_ERROR

namespace tictac_track {

  class AppError {
    
  public:
    // Print error message
    static bool PrintError(const char *message);
    static bool PrintError(const char *message, int n);
  };
} // namespace tictac_shared

#endif
