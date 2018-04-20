
//          Copyright Kay Stenschke 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include "helper_numeric.h"

namespace helper {
unsigned int Numeric::ToUnsignedInt(int number) {
  return static_cast<unsigned int>(number);
}

std::string Numeric::ToString(int number) {
  return std::to_string(number);
}
std::string Numeric::ToString(int number, unsigned int amount_digits_min) {
  std::string str = std::to_string(number);
  std::string zero = "0";
  while (str.size() < amount_digits_min) str = zero.append(str);

  return str;
}

/**
 * @note Supports positive numbers only
 */
unsigned long Numeric::GetAmountDigits(int number) {
  if (number == 0) return 1;

  unsigned long digits = 0;
  while (number) {
    number /= 10;
    digits++;
  }

  return digits;
}
} // namespace helper
