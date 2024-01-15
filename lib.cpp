#pragma once
#include <stdint.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// if you do not plan to implement bonus, you can delete those lines
// or just keep them as is and do not define the macro to 1
#define SUPPORT_IFSTREAM 1
#define SUPPORT_ISQRT 1
#define SUPPORT_EVAL 0  // special bonus

#define CHECK_BIGING_BOUNDS \
  0  // if 1, BigInteger will check bounds on operator[] and throw std::out_of_range if index is out
     // of range
// [digit] - unsigned single digit
using digit = uint32_t;
// [idigit] - MAX value of signed single digit
constexpr auto digit_max = UINT32_MAX;
// [digit_size] - number of bits in digit
constexpr auto digit_size = UINT32_WIDTH;
// [d_digit] - unsigned double digit
using d_digit = uint64_t;
// [d_idigit] - signed double digit
using d_idigit = int64_t;

class BigInteger {
  friend std::ostream &operator<<(std::ostream &lhs, const BigInteger &rhs);
  friend bool operator<(const BigInteger &lhs, const BigInteger &rhs);
  friend bool operator>(const BigInteger &lhs, const BigInteger &rhs);
  friend bool operator<=(const BigInteger &lhs, const BigInteger &rhs);
  friend bool operator>=(const BigInteger &lhs, const BigInteger &rhs);
  friend bool operator==(const BigInteger &lhs, const BigInteger &rhs);
  friend bool operator==(const BigInteger &lhs, digit rhs);
  friend bool operator==(const BigInteger &lhs, const int rhs);
  friend bool operator!=(const BigInteger &lhs, const BigInteger &rhs);
  friend BigInteger operator+(const BigInteger lhs, const BigInteger &rhs);
  friend BigInteger operator+(const BigInteger lhs, const digit rhs);
  friend BigInteger operator+(const BigInteger lhs, const int rhs);
  friend BigInteger operator*(const BigInteger lhs, const BigInteger &rhs);
  friend BigInteger operator*(const BigInteger &lhs, const digit rhs);
  friend BigInteger operator-(BigInteger lhs, const BigInteger &rhs);
  friend BigInteger operator-(BigInteger lhs, digit rhs);
  friend BigInteger operator-(BigInteger lhs, const int rhs);
  friend BigInteger operator/(BigInteger lhs, const BigInteger &rhs);
  friend BigInteger operator/(const BigInteger &lhs, digit rhs);
  friend BigInteger operator%(BigInteger lhs, const BigInteger &rhs);
  friend BigInteger left_shift(BigInteger number, size_t shift);

 public:
  bool negative;
  std::vector<digit> digits;  // Least Significant Digit first in vector

  BigInteger() : negative(false), digits(1, 0) {}
  BigInteger(int64_t value) : negative(value < 0), digits(1, 0) {
    if (value == std::numeric_limits<int64_t>::min()) {
      digits[0] = 1;
      *this = left_shift(*this, 63);
      return;
    }
    value = std::abs(value);
    digits.resize(64 / digit_size, 0);
    for (size_t i = 0; value != 0; ++i) {
      digits[i] = static_cast<digit>(value & static_cast<int64_t>(digit_max));
      value >>= digit_size;
    }
    trim();
  }
  explicit BigInteger(const std::string &number) : BigInteger() {
    if (number.empty()) throw std::invalid_argument("Empty string is not a valid BigInteger");

    size_t startPos = 0;
    if (number[0] == '-' || number[0] == '+') startPos = 1;

    for (size_t i = startPos; i < number.size(); ++i) {
      char digit = number[i];
      if (std::isdigit(digit) == 0)
        throw std::invalid_argument("Invalid character in BigInteger string");

      *this *= BigInteger(10);
      *this += BigInteger(digit - '0');
    }

    if (number[0] == '-') negative = true;
    trim();
  }
  BigInteger(const BigInteger &other) = default;
  BigInteger &operator=(const BigInteger &rhs) = default;
  const BigInteger &operator+() const { return *this; }
  digit to_digit() const {
    if (negative) throw std::runtime_error("Cannot convert negative number to digit");
    if (digits.size() > 1) throw std::runtime_error("Number too large to be represented as digit");
    return digits[0];
  }
  BigInteger operator-() const {
    BigInteger result = *this;
    result.negative = !result.negative;
    return result;
  }
  BigInteger &operator+=(const BigInteger &rhs) {
    *this = *this + rhs;
    return *this;
  }
  BigInteger &operator-=(const BigInteger &rhs) {
    *this = *this - rhs;
    return *this;
  }
  BigInteger &operator*=(const BigInteger &rhs) {
    *this = *this * rhs;
    return *this;
  }
  BigInteger &operator/=(const BigInteger &rhs) {
    *this = *this / rhs;
    return *this;
  }
  BigInteger &operator%=(const BigInteger &rhs) {
    *this = *this % rhs;
    return *this;
  }
  // NOTE: This function assumes that the number have no leading zeros and not zero
  static size_t count_leading_zeros(digit x) {
    size_t count = 0;
    while (x != 0) {
      x >>= 1;
      ++count;
    }
    return digit_size - count;
  }
  size_t count_leading_zeros() const {
    digit x = digits.back();
    size_t count = 0;
    while (x != 0) {
      x >>= 1;
      ++count;
    }
    return digit_size - count;
  }
  void normalize() {
    trim();
    if (is_zero())
      throw std::runtime_error(
          "Cannot normalize zero. Something went wrong if zero is being normalized");
    *this = left_shift(*this, count_leading_zeros());
  }
  BigInteger toNormalized() const {
    BigInteger result = *this;
    result.normalize();
    return result;
  }
  std::string toBitString() const {
    std::string result;
    digit current = 0;
    // Pring first digit
    current = digits[size() - 1];
    for (size_t j = 0; j < digit_size; j++) {
      result.push_back('0' + (current & 1));
      current >>= 1;
    }
    if (size() != 1) {
      // Print other digits
      for (size_t i = size() - 2; i > 0; i--) {
        result.push_back(' ');
        current = digits[i];
        for (size_t j = 0; j < digit_size; j++) {
          result.push_back('0' + (current & 1));
          current >>= 1;
        }
      }
      // Print last digit
      current = digits[0];
      for (size_t j = 0; j < digit_size; j++) {
        result.push_back('0' + (current & 1));
        current >>= 1;
      }
    }
    std::reverse(result.begin(), result.end());
    return result;
  }
  std::string toString() const {
    if (is_zero()) return "0";
    auto temp = *this;
    std::string result;
    result.reserve(digits.size() * (std::log10(digit_max) + 1));  // Estimated reserve

    while (temp.size() > 1 || temp[0] != 0) {
      digit remainder = temp.divideBySingleDigit(10);
      result.push_back('0' + remainder);
    }
    if (negative) result.push_back('-');

    std::reverse(result.begin(), result.end());
    return result;
  }
  inline void trim() {
    while (!digits.empty() && digits.back() == 0) digits.pop_back();
    if (digits.empty()) digits.push_back(0);
  }
  /* Remove leading zero digits */
  inline BigInteger rlz() {
    trim();
    return *this;
  }
  // Shift digits to the left by {digit_shift} digits. If {digit_shift} is 0, do nothing.
  inline void left_shift_digit(size_t digit_shift = 1) {
    digits.resize(digits.size() + digit_shift, 0);
    for (size_t shift = 0; shift < digit_shift; ++shift) {
      digit temp = 0;
      for (size_t i = shift; i < size(); i += digit_shift) {
        digit temp2 = digits[i];
        digits[i] = temp;
        temp = temp2;
      }
    }
  }
  inline BigInteger &multiplyByTwo() {
    digit carry = 0;
    for (size_t i = 0; i < size(); ++i) {
      digit current = digits[i];
      digits[i] = (current << 1) | carry;
      carry = current >> (digit_size - 1);
    }
    if (carry != 0) digits.push_back(carry);
    return *this;
  }
  BigInteger multiplyByTwo() const {
    BigInteger result = *this;
    result.multiplyByTwo();
    return result;
  }
  double sqrt() const {
    if (negative) throw std::domain_error("Square root of a negative number is not real");
    return std::sqrt(static_cast<double>(*this));
  }
  explicit operator double() const {
    int num_bits = 0;
    for (int i = digits.size() - 1; i >= 0; --i) {
      if (digits[i] != 0) {
        num_bits = i * digit_size;
        digit temp = digits[i];
        while (temp != 0) {
          temp >>= 1;
          num_bits++;
        }
        break;  // Break after finding the most significant non-zero digit
      }
    }
    if (num_bits > 53)
      throw std::overflow_error("BigInteger value too large to be represented as a double");
    double result = 0.0;
    for (int i = digits.size() - 1; i >= 0; --i) {
      result *= static_cast<double>((d_digit)1 << digit_size);
      result += digits[i];
    }
    if (negative) result = -result;
    return result;
  }
  // Note: if you access digit that is not in the number - you will get Undefined Behavior
  inline digit operator[](size_t index) const {
#if CHECK_BIGING_BOUNDS == 1
    if (index >= digits.size()) throw std::out_of_range("Index out of range");
#endif
    return digits[index];
  }
  inline size_t size() const { return digits.size(); }
#if SUPPORT_ISQRT == 1
  BigInteger isqrt() const {
    if (negative) throw std::domain_error("Square root of a negative number is not defined");
    if (is_zero()) return BigInteger(0);
    if (*this == BigInteger(1)) return BigInteger(1);

    BigInteger left(0), right(*this + BigInteger(1)), mid;
    while (left != right - BigInteger(1)) {
      mid = (left + right) / (digit)2;
      if (mid * mid <= *this) {
        left = mid;
      } else {
        right = mid;
      }
    }

    return left;
  }
#endif
  // Return lhs - rhs assuming lhs >= rhs in absolute value of both numbers
  static BigInteger sub_abs(const BigInteger &lhs, const BigInteger &rhs) {
    BigInteger result;
    result.digits.resize(lhs.size(), 0);
    bool borrow = false;

    for (size_t i = 0; i < lhs.size(); ++i) {
      d_idigit diff = d_idigit(lhs[i]) - (i < rhs.size() ? rhs[i] : 0) - borrow;
      borrow = false;
      if (diff < 0) {
        diff += ((d_idigit)1 << digit_size);
        borrow = true;
      }
      result.digits[i] = static_cast<digit>(diff);
    }

    return result.rlz();
  }
  static BigInteger subAbs(const BigInteger &lhs, const digit rhs) {
    BigInteger result;
    result.digits.resize(lhs.size(), 0);
    bool borrow = false;

    for (size_t i = 0; i < lhs.size(); ++i) {
      d_idigit lhs_digit = lhs[i];
      d_idigit rhs_digit =
          (i == 0) ? rhs : 0;  // Only subtract rhs from the least significant digit
      d_idigit sub = lhs_digit - rhs_digit - borrow;

      if (lhs_digit < rhs_digit + borrow) {
        borrow = true;
        sub += ((d_idigit)1 << digit_size);
      } else {
        borrow = false;
      }

      result.digits[i] = static_cast<digit>(sub);
    }

    return result.rlz();
  }
  static BigInteger add_abs(const BigInteger &lhs, const digit rhs) {
    BigInteger result;
    result.digits.resize(lhs.size(), 0);
    digit carry = rhs;

    for (size_t i = 0; i < lhs.size(); ++i) {
      d_digit sum = (d_digit)lhs[i] + (d_digit)carry;
      carry = sum >> digit_size;
      result.digits[i] = sum & digit_max;
    }
    if (carry != 0) result.digits.push_back(carry);

    return result.rlz();
  }
  inline static digit mul2(digit a, digit b, digit *carry) {
    d_digit product = (d_digit)a * (d_digit)b;
    *carry = product >> digit_size;
    return product & digit_max;
  }
  inline static digit add3(digit a, digit b, digit c, digit *carry) {
    d_digit sum = (d_digit)a + (d_digit)b + (d_digit)c;
    *carry = sum >> digit_size;
    return sum & digit_max;
  }
  inline static digit add2(digit a, digit b, digit *carry) {
    d_digit sum = (d_digit)a + (d_digit)b;
    *carry = sum >> digit_size;
    return sum & digit_max;
  }
  // Add two numbers assuming they are positive
  static BigInteger addAbs(const BigInteger &lhs, const BigInteger &rhs) {
    if (lhs.size() < rhs.size()) return addAbs(rhs, lhs);
    BigInteger result;
    size_t maxLength = std::max(lhs.size(), rhs.size() + 1);
    result.digits.resize(maxLength + 1, 0);

    size_t i = 0;
    digit carry = 0;
    for (; i < rhs.size(); i++) result.digits[i] = add3(lhs[i], rhs[i], carry, &carry);
    for (; i < lhs.size(); i++) result.digits[i] = add2(lhs[i], carry, &carry);
    if (carry != 0) result.digits[i] = carry;

    return result.rlz();
  }

  bool is_zero() const { return digits.size() == 1 && digits[0] == 0; }

 private:
  // Returns remainder, changes the number to the quotient
  digit divideBySingleDigit(digit divisor) {
    if (divisor == 0) throw std::runtime_error("Division by zero");
    if (divisor == 1) return 0;

    d_digit remainder = 0;
    for (int i = digits.size() - 1; i >= 0; --i) {
      d_digit current = remainder * ((d_digit)digit_max + 1) + digits[i];
      digits[i] = current / divisor;
      remainder = current % divisor;
    }

    trim();
    return remainder;
  }
};
inline std::ostream &operator<<(std::ostream &lhs, const BigInteger &rhs) {
  lhs << rhs.toString();
  return lhs;
}
inline std::ostream &operator<<(std::ostream &lhs, BigInteger rhs) {
  lhs << rhs.toString();
  return lhs;
}
inline BigInteger ABS(const BigInteger &number) {
  BigInteger result = number;
  result.negative = false;
  return result;
}
// Returns new BigInteger with digits shifted left by shift, if number is zero return zero
BigInteger left_shift(const BigInteger number, const size_t shift) {
  if (number.is_zero()) return number;
  if (shift == 0) return number;

  digit digit_shift = shift / digit_size;
  digit bit_shift = shift % digit_size;
  BigInteger result;
  result.negative = number.negative;
  result.digits.resize(number.size() + digit_shift + 1, 0);
  size_t i = 0;
  for (; i < digit_shift; ++i) result.digits[i] = 0;

  if (bit_shift == 0) {
    for (; i < number.size() + digit_shift; ++i) {
      result.digits[i] = number[i - digit_shift];
    }
  } else {
    digit carry = 0;
    for (; i < number.size() + digit_shift; ++i) {
      digit current = number[i - digit_shift];
      result.digits[i] = (current << bit_shift) | carry;
      carry = current >> (digit_size - bit_shift);
    }
    if (carry != 0) {
      result.digits[number.size() + digit_shift] = carry;
    }
  }

  return result.rlz();
}
inline BigInteger operator+(const BigInteger lhs, digit rhs) {
  if (lhs.is_zero()) return BigInteger(rhs);
  if (rhs == 0) return lhs;
  if (lhs.negative) return BigInteger(rhs) - (-lhs);
  return BigInteger::add_abs(lhs, rhs);
}
inline BigInteger operator+(const BigInteger lhs, const int rhs) {
  if (lhs.is_zero()) return BigInteger(rhs);
  if (rhs == 0) return lhs;
  if (lhs.negative && rhs > 0) return BigInteger(rhs) - (-lhs);  // (-a) + b = b - a
  if (!lhs.negative && rhs < 0) return lhs - (-rhs);             // a + (-b) = a - b
  if (lhs.negative && rhs < 0) return -((-lhs) + (-rhs));        // (-a) + (-b) = -(a + b)
  return BigInteger::add_abs(lhs, rhs);
}
inline BigInteger operator+(const BigInteger lhs, const BigInteger &rhs) {
  if (lhs.is_zero()) return rhs;
  if (rhs.is_zero()) return lhs;
  if (lhs.negative && !rhs.negative) return rhs - (-lhs);
  if (!lhs.negative && rhs.negative) return lhs - (-rhs);

  BigInteger result = BigInteger::addAbs(ABS(lhs), ABS(rhs));
  result.negative = lhs.negative && rhs.negative;
  return result;
}
inline BigInteger operator-(const BigInteger &lhs, const digit rhs) {
  if (lhs.is_zero()) return BigInteger(-rhs);
  if (rhs == 0) return lhs;
  if (lhs.negative) return -((-lhs) + rhs);
  return BigInteger::subAbs(lhs, rhs);
}
inline digit operator-(const digit lhs, const BigInteger rhs) {
  if (rhs.is_zero()) return BigInteger(lhs).to_digit();
  if (lhs == 0) return -rhs.to_digit();
  if (rhs.negative) return -((-rhs) + BigInteger(static_cast<int64_t>(lhs))).to_digit();

  BigInteger result = BigInteger::subAbs(ABS(rhs), lhs);
  result.negative = true;
  return result.rlz().to_digit();
}
inline BigInteger operator-(const BigInteger lhs, const int rhs) {
  if (lhs.is_zero()) return BigInteger(-rhs);
  if (rhs == 0) return lhs;
  if (lhs.negative && rhs > 0) return -((-lhs) + rhs);     // (-a) - b = -(a + b)
  if (!lhs.negative && rhs < 0) return lhs + (-rhs);       // a - (-b) = a + b
  if (lhs.negative && rhs < 0) return -((-lhs) - (-rhs));  // (-a) - (-b) = b - a
  return BigInteger::subAbs(lhs, static_cast<digit>(rhs));
}
inline BigInteger operator-(BigInteger lhs, const BigInteger &rhs) {
  if (lhs.is_zero()) return -rhs;
  if (rhs.is_zero()) return lhs;
  if (lhs == rhs) return BigInteger(0);

  if (lhs.negative && rhs.negative) return (-rhs) - (-lhs);
  if (lhs.negative && !rhs.negative) return -((-lhs) + rhs);
  if (!lhs.negative && rhs.negative) return lhs + (-rhs);

  BigInteger result;
  result.digits.resize(std::max(lhs.size(), rhs.size()), 0);
  if (lhs < rhs) {
    result = BigInteger::sub_abs(rhs, lhs);
    result.negative = true;
  } else {
    result = BigInteger::sub_abs(lhs, rhs);
  }
  return result.rlz();
}
inline BigInteger operator*(const BigInteger &lhs, const digit rhs) {
  if (lhs.is_zero() || rhs == 0) return 0;
  if (lhs == 1) return BigInteger(rhs);
  if (rhs == 1) return lhs;
  if (rhs == 2) return lhs.multiplyByTwo();

  BigInteger result;
  result.digits.resize(lhs.size() + 1, 0);

  digit sum_carry = 0;
  digit old_product_carry = 0;
  for (size_t i = 0; i < lhs.size(); ++i) {
    digit new_product_carry = 0;
    d_digit product = BigInteger::mul2(lhs[i], rhs, &new_product_carry);
    result.digits[i] = BigInteger::add3(product, old_product_carry, sum_carry, &sum_carry);
    old_product_carry = new_product_carry;
  }
  result.digits[lhs.size()] = old_product_carry + sum_carry;
  result.negative = lhs.negative;
  return result.rlz();
}
/* } */
inline BigInteger operator*(const BigInteger lhs, const BigInteger &rhs) {
  if (lhs.is_zero() || rhs.is_zero()) return BigInteger(0);
  if (lhs == 1) return rhs;
  if (rhs == 1) return lhs;
  if (lhs > rhs) return rhs * lhs;
  BigInteger result, temp;
  result.digits.resize(lhs.size() + rhs.size(), 0);
  for (size_t i = 0; i < lhs.size(); ++i) {
    temp = ABS(rhs) * lhs[i];
    temp.left_shift_digit(i);
    result += temp;
  }
  result.negative = lhs.negative ^ rhs.negative;
  return result.rlz();
}
inline BigInteger operator^(const BigInteger lhs, const size_t rhs) {
  if (rhs == 0) return BigInteger(1);
  if (rhs == 1) return lhs;
  BigInteger result = lhs;
  for (size_t i = 0; i < rhs - 1; ++i) {
    result *= lhs;
  }
  return result;
}
inline BigInteger operator%(const BigInteger lhs, const BigInteger &rhs) {
  if (rhs.is_zero()) throw std::runtime_error("Division by zero");
  auto quotient = lhs / rhs;
  auto remainder = ABS(lhs) - ABS(quotient * rhs);
  remainder.negative = lhs.negative;
  return remainder;
}
inline BigInteger operator<(const BigInteger lhs, digit rhs) {
  if (lhs.size() > 1) return false;
  return lhs[0] < rhs;
}
inline BigInteger operator>(const BigInteger lhs, digit rhs) {
  if (lhs.size() > 1) return true;
  return lhs[0] > rhs;
}
digit normalize(digit number) { return number << BigInteger::count_leading_zeros(number); }
BigInteger operator/(const BigInteger &lhs, digit rhs) {
  if (rhs == 0) throw std::runtime_error("Division by zero");
  size_t shift = BigInteger::count_leading_zeros(rhs);
  rhs = normalize(rhs);
  auto dividend = left_shift(lhs, shift);
  BigInteger result;
  result.digits.resize(dividend.size(), 0);
  d_digit remainder = 0;
  for (int i = dividend.size() - 1; i >= 0; --i) {
    d_digit current_digit = (remainder * ((d_digit)digit_max + 1)) + (d_digit)dividend[i];
    result.digits[i] = current_digit / rhs;
    remainder = current_digit % rhs;
  }

  return result.rlz();
}
static inline digit digit_div(digit high, digit low, digit divisor, digit *remainder) {
  d_digit dividend = ((d_digit)high << digit_size) + (d_digit)low;
  d_digit quotient = dividend / divisor;
  *remainder = dividend % divisor;
  return quotient;
}
inline BigInteger operator/(const BigInteger lhs, const BigInteger &rhs) {
  if (rhs.is_zero()) throw std::runtime_error("Division by zero");
  if (lhs.is_zero()) return BigInteger(0);
  if (lhs == 1) return BigInteger(1);
  if (ABS(lhs) < ABS(rhs)) return BigInteger(0);
  if (rhs.size() == 1) {
    BigInteger result = lhs / rhs[0];
    result.negative = lhs.negative ^ rhs.negative;
    return result;
  }

  BigInteger divisor(rhs);
  divisor.normalize();
  divisor.negative = false;

  BigInteger current_product;

  auto dividend = left_shift(lhs, rhs.count_leading_zeros());
  dividend.negative = false;

  size_t shift = (dividend.size() - divisor.size()) * digit_size;
  if (shift == 0) shift = digit_size;
  BigInteger left_q = left_shift(BigInteger(1), shift - digit_size);
  BigInteger right_q = left_shift(BigInteger(1), shift + digit_size);

  BigInteger current_q;
  while (left_q < right_q) {
    current_q = (left_q + right_q) / (digit)2;
    current_product = current_q * divisor;
    auto diff = dividend - current_product;
    if (diff.is_zero()) {
      current_q.negative = lhs.negative ^ rhs.negative;
      return current_q;
    } else if (diff.negative) {
      right_q = current_q;
    } else {
      if (diff < divisor) {
        current_q.negative = lhs.negative ^ rhs.negative;
        return current_q;
      }
      left_q = current_q + 1;
    }
  }

  left_q.negative = lhs.negative ^ rhs.negative;
  return left_q;
}
inline bool operator==(const BigInteger &lhs, const digit rhs) {
  if (lhs.size() > 1) return false;
  if (lhs.negative) return false;
  return lhs[0] == rhs;
}
inline bool operator==(const BigInteger &lhs, const int rhs) {
  if (lhs.size() > 1) return false;
  if (lhs.is_zero() && rhs == 0) return true;
  if (lhs.negative && rhs > 0) return false;
  return lhs[0] == std::abs(rhs);
}
inline bool operator==(const BigInteger &lhs, const BigInteger &rhs) {
  if (lhs.is_zero() && rhs.is_zero()) return true;
  if (lhs.size() != rhs.size()) return false;
  if (lhs.negative != rhs.negative) return false;
  for (size_t i = lhs.size(); i-- > 0;) {
    if (lhs[i] != rhs[i]) return false;
  }
  return true;
}
inline bool operator!=(const BigInteger &lhs, const BigInteger &rhs) { return !(lhs == rhs); }
inline bool operator<(const BigInteger &lhs, const BigInteger &rhs) {
  return rhs > lhs && rhs != lhs;
}
inline bool operator>(const BigInteger &lhs, const BigInteger &rhs) {
  if (lhs.negative && !rhs.negative) return false;
  if (!lhs.negative && rhs.negative) return true;
  if (lhs.negative && rhs.negative) return -lhs < -rhs;
  if (lhs.size() != rhs.size()) return lhs.size() > rhs.size();

  for (size_t i = lhs.size(); i-- > 0;) {
    if (lhs[i] != rhs[i]) return lhs[i] > rhs[i];
  }
  return false;
}
inline bool operator<=(const BigInteger &lhs, const BigInteger &rhs) {
  return lhs < rhs || lhs == rhs;
}
inline bool operator>=(const BigInteger &lhs, const BigInteger &rhs) {
  return lhs > rhs || lhs == rhs;
}

#if SUPPORT_IFSTREAM == 1
// this should behave exactly the same as reading int with respect to
// whitespace, consumed characters etc...
inline std::istream &operator>>(std::istream &lhs, BigInteger &rhs) {
  std::string input;
  lhs >> input;

  try {
    rhs = BigInteger(input);
  } catch (const std::invalid_argument &e) {
    lhs.setstate(std::ios::failbit);
  }

  return lhs;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BigRational {
 public:
  BigInteger numerator;
  BigInteger denominator;
  bool negative;
  // constructors
  BigRational() {
    numerator = BigInteger(0);
    denominator = BigInteger(1);
    negative = false;
  };
  BigRational(int64_t a, int64_t b) {
    negative = (a < 0) ^ (b < 0);
    numerator = ABS(BigInteger(a));
    denominator = ABS(BigInteger(b));
    if (denominator.is_zero()) throw std::runtime_error("Denominator cannot be zero");
    *this = to_normalized();
  };
  BigRational(const std::string &a, const std::string &b) {
    BigInteger a_BigInt(a), b_BigInt(b);
    negative = (a_BigInt.negative) ^ (b_BigInt.negative);
    numerator = ABS(a_BigInt);
    denominator = ABS(b_BigInt);
    if (denominator.is_zero()) throw std::runtime_error("Denominator cannot be zero");
    *this = to_normalized();
  };
  BigRational(const BigRational &other) {
    numerator = other.numerator;
    denominator = other.denominator;
    negative = other.negative;
  };
  BigRational &operator=(const BigRational &rhs) {
    numerator = rhs.numerator;
    denominator = rhs.denominator;
    negative = rhs.negative;
    return *this;
  };
  const BigRational &operator+() const { return *this; };
  BigRational operator-() const {
    BigRational result = *this;
    result.negative = !result.negative;
    return result.to_normalized();
  };
  // binary arithmetics operators
  BigRational &operator+=(const BigRational &rhs) {
    *this = *this + rhs;
    return *this;
  }
  BigRational &operator-=(const BigRational &rhs);
  BigRational &operator*=(const BigRational &rhs);
  BigRational &operator/=(const BigRational &rhs);

  friend BigRational operator+(BigRational lhs, const BigRational &rhs);
  friend BigRational operator-(BigRational lhs, const BigRational &rhs);
  friend BigRational operator*(BigRational lhs, const BigRational &rhs);
  friend BigRational operator/(BigRational lhs, const BigRational &rhs);

  double sqrt() const;

  bool isZero() const { return numerator.is_zero(); }
  // Find greatest common divisor
  static BigInteger gcd(const BigInteger &a, const BigInteger &b) {
    BigInteger x = a;
    BigInteger y = b;
    while (!y.is_zero()) {
      BigInteger temp = y;
      y = x % y;
      x = temp;
    }
    return x;
  }

  inline BigRational to_normalized() const {
    BigRational result = *this;
    if (denominator.is_zero()) throw std::runtime_error("Denominator cannot be zero");
    if (numerator.is_zero()) {
      result.numerator = BigInteger(0);
      result.denominator = BigInteger(1);
      return result;
    }
    BigInteger gcdValue = gcd(result.numerator, result.denominator);
    result.numerator /= gcdValue;
    result.denominator /= gcdValue;
    if (result.denominator < BigInteger(0)) {
      // Ensure the denominator is positive
      result.denominator = -result.denominator;
      result.numerator = -result.numerator;
    }

    return result;
  }

  bool is_zero() const { return numerator.is_zero(); }

  BigRational addAbs(const BigRational &lhs, const BigRational &rhs) const {
    BigRational result;
    result.denominator = lhs.denominator * rhs.denominator;
    result.numerator = lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator;
    return result.to_normalized();
  }

  friend bool operator<(const BigRational &lhs, const BigRational &rhs);
#if SUPPORT_ISQRT == 1
  BigInteger isqrt() const;
#endif
 private:
  // here you can add private data and members, but do not add stuff to
  // public interface, also you can declare friends here if you want
};
inline BigRational operator+(BigRational lhs, const BigRational &rhs) {
  if (lhs.numerator.is_zero()) {
    return rhs;
  } else if (rhs.numerator.is_zero()) {
    return lhs;
  }
  if (lhs.negative && !rhs.negative) {  // -a + b = b - a
    return rhs - (-lhs);
  } else if (!lhs.negative && rhs.negative) {  // a + (-b) = a - b
    return lhs - (-rhs);
  } else {  // a + b or -a + (-b)
    BigRational temp = rhs, temp2 = lhs;
    temp.negative = false;
    temp2.negative = false;
    BigRational result = lhs.addAbs(temp2, temp);
    result.negative = lhs.negative && rhs.negative;
    return result.to_normalized();
  }
};
inline BigRational operator-(BigRational lhs, const BigRational &rhs) {
  if (lhs.numerator.is_zero()) {
    BigRational result = rhs;
    result.negative = !result.negative;
    return result.to_normalized();
  }
  if (rhs.numerator.is_zero()) return lhs;

  BigRational result;
  result.denominator = lhs.denominator * rhs.denominator;
  result.numerator = lhs.numerator * rhs.denominator - rhs.numerator * lhs.denominator;
  result.negative = lhs.negative ^ rhs.negative;
  return result.to_normalized();
};
inline BigRational operator*(BigRational lhs, const BigRational &rhs) {
  BigRational result;
  result.numerator = lhs.numerator * rhs.numerator;
  result.denominator = lhs.denominator * rhs.denominator;
  result.negative = lhs.negative ^ rhs.negative;
  return result.to_normalized();
};
inline BigRational operator/(const BigRational lhs, const BigRational &rhs) {
  if (rhs.numerator.is_zero()) throw std::runtime_error("Division by zero");
  BigRational result;
  result.numerator = lhs.numerator * rhs.denominator;
  result.denominator = lhs.denominator * rhs.numerator;
  result.negative = lhs.negative ^ rhs.negative;
  return result.to_normalized();
};
inline bool operator==(const BigRational &lhs, const BigRational &rhs) {
  auto lhs_norm = lhs.to_normalized();
  auto rhs_norm = rhs.to_normalized();
  if (lhs_norm.isZero() && rhs_norm.isZero()) return true;
  if (lhs_norm.negative != rhs_norm.negative) return false;
  if (lhs_norm.numerator != rhs_norm.numerator) return false;
  if (lhs_norm.denominator != rhs_norm.denominator) return false;
  return true;
}
inline bool operator!=(const BigRational &lhs, const BigRational &rhs) { return !(lhs == rhs); };
inline bool operator<(const BigRational &lhs, const BigRational &rhs) {
  if (lhs.negative && !rhs.negative) return true;
  if (!lhs.negative && rhs.negative) return false;

  auto lhs_norm = lhs.to_normalized();
  auto rhs_norm = rhs.to_normalized();
  if (lhs.negative && rhs.negative) {
    return lhs_norm.numerator * rhs_norm.denominator > rhs_norm.numerator * lhs_norm.denominator;
  } else {
    return lhs_norm.numerator * rhs_norm.denominator < rhs_norm.numerator * lhs_norm.denominator;
  }
}
inline bool operator>(const BigRational &lhs, const BigRational &rhs) {
  if (lhs.negative && !rhs.negative) return false;
  if (!lhs.negative && rhs.negative) return true;

  auto lhs_norm = lhs.to_normalized();
  auto rhs_norm = rhs.to_normalized();
  if (lhs.negative && rhs.negative) {
    return lhs_norm.numerator * rhs_norm.denominator < rhs_norm.numerator * lhs_norm.denominator;
  } else {
    return lhs_norm.numerator * rhs_norm.denominator > rhs_norm.numerator * lhs_norm.denominator;
  }
};
inline bool operator<=(const BigRational &lhs, const BigRational &rhs) {
  return lhs < rhs || lhs == rhs;
};
inline bool operator>=(const BigRational &lhs, const BigRational &rhs) {
  return lhs > rhs || lhs == rhs;
};

inline std::ostream &operator<<(std::ostream &lhs, const BigRational &rhs) {
  lhs << rhs.numerator.toString() << "/" << rhs.denominator.toString();
  return lhs;
}

#if SUPPORT_IFSTREAM == 1
// this should behave exactly the same as reading int with respect to
// whitespace, consumed characters etc...
// NOTE: I expect the input to be in the form of "numerator/denominator"
inline std::istream &operator>>(std::istream &lhs, BigRational &rhs){
  std::string input;
  lhs >> input;
  try {
    size_t slash_pos = input.find('/');
    if (slash_pos == std::string::npos) throw std::invalid_argument("Invalid BigRational string");
    std::string numerator = input.substr(0, slash_pos);
    std::string denominator = input.substr(slash_pos + 1);
    rhs = BigRational(numerator, denominator);
  } catch (const std::invalid_argument &e) {
    lhs.setstate(std::ios::failbit);
  }
  return lhs;
}
#endif

#if SUPPORT_EVAL == 1
inline BigInteger eval(const std::string &);
#endif
