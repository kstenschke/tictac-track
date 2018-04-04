
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_TIMETRACKERCL_HELPER_NUMERIC
#define CLASS_TIMETRACKERCL_HELPER_NUMERIC

namespace tictac_track {
namespace HelperNumeric {
extern unsigned int ToUnsignedInt(int number);

extern unsigned long GetAmountDigits(int number);

extern std::string ToString(int number);
extern std::string ToString(int number, unsigned int amount_digits_min);
}
} // namespace tictac_track

#endif
