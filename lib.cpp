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

// [digit] - unsigned single digit
using digit = uint32_t;
constexpr auto digit_max = UINT32_MAX;
// [d_digit] - unsigned double digit
using d_digit = uint64_t;
// [d_idigit] - signed double digit
using d_idigit = int64_t;
constexpr auto digit_size = 32;

class BigInteger {
  friend std::ostream &operator<<(std::ostream &lhs, const BigInteger &rhs);
  friend bool operator<(const BigInteger &lhs, const BigInteger &rhs);
  friend bool operator>(const BigInteger &lhs, const BigInteger &rhs);
  friend bool operator<=(const BigInteger &lhs, const BigInteger &rhs);
  friend bool operator>=(const BigInteger &lhs, const BigInteger &rhs);
  friend bool operator==(const BigInteger &lhs, const BigInteger &rhs);
  friend bool operator==(const BigInteger &lhs, digit rhs);
  friend bool operator==(const BigInteger &lhs, const int rhs);
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
  friend BigInteger left_shift(BigInteger number, size_t shift);

 public:
  std::vector<digit> digits;  // Least Significant Digit first in vector
  bool negative;

  BigInteger() : digits(1, 0), negative(false) {}
  BigInteger(int64_t value) : negative(value < 0), digits(1, 0) {
    value = std::abs(value);
    digits[0] = static_cast<digit>(value & digit_max);
    digit upper = static_cast<digit>(value >> digit_size);
    if (upper > 0) digits.push_back(upper);
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
  // Add {value} to the digit at position {pos} in the number. Handle carry. Position is from least
  // significant digit.
  void add_to_digit(digit value, size_t pos) {
    if (pos >= digits.size()) {
      digits.resize(pos + 1, 0);
    }

    d_digit sum = 0;
    digit carry = value;
    for (size_t i = pos; i < digits.size() || carry != 0; ++i) {
      if (i >= digits.size()) digits.push_back(0);

      sum = (d_digit)digits[i] + (d_digit)carry;
      digits[i] = sum & digit_max;
      carry = sum >> digit_size;
    }

    trim();
  }
  const BigInteger &operator+() const { return *this; }
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
  // NOTE: This function assumes that the number have no leading zeros
  size_t count_leading_zeros() const {
    digit x = digits.back();
    if (x == 0) return 0;
    return __builtin_clz(x);
  }
  void normalize() {
    trim();
    if (is_zero()) return;
    *this = left_shift(*this, count_leading_zeros());
  }
  BigInteger toNormalized() const {
    BigInteger result = *this;
    result.normalize();
    return result;
  }
  std::string toBitString() const {
    std::string result;
    // NOTE: Overflows if digit_size >= 2^32
    for (int i = digits.size() - 1; i >= 0; i--) {
      digit current = digits[i];
      for (int j = 0; j < digit_size; ++j) {
        result.push_back('0' + (current & 1));
        current >>= 1;
      }
      if (i != digits.size() - 1) result.push_back(' ');
    }
    std::reverse(result.begin(), result.end());
    return result;
  }
  std::string toString() const {
    if (is_zero()) return "0";
    auto temp = *this;
    std::string result;
    // TODO: Improve this. What if this will fail?
    result.reserve(digits.size() * (std::log10(digit_max) + 1));  // Estimated reserve

    while (temp.digits.size() > 1 || temp.digits[0] != 0) {
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
  inline BigInteger rlz() {
    trim();
    return *this;
  }
  inline BigInteger &divideByTwo() {
    digit remainder = 0;
    for (int i = digits.size() - 1; i >= 0; --i) {
      digit current = digits[i];
      digits[i] = (current >> 1) | remainder;
      remainder = (current & 1) << (digit_size - 1);
    }
    rlz();
    return *this;
  }
  inline BigInteger &multiplyByTwo() {
    digit carry = 0;
    for (size_t i = 0; i < digits.size(); ++i) {
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
    if (negative) throw std::domain_error("Square root of a negative number is not defined");
    return std::sqrt(static_cast<double>(*this));
  }
  operator digit() const {
    if (digits.size() > 1) throw std::overflow_error("BigInteger value too large to be represented as a digit");
    return digits[0];
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
    if (num_bits > 53) throw std::overflow_error("BigInteger value too large to be represented as a double");
    double result = 0.0;
    for (int i = digits.size() - 1; i >= 0; --i) {
      result *= static_cast<double>((d_digit)1 << digit_size);
      result += digits[i];
    }
    if (negative) result = -result;
    return result;
  }
  digit operator[](size_t index) const {
    if (index >= digits.size()) return 0;
    return digits[index];
  }
  size_t size() const { return digits.size(); }
#if SUPPORT_ISQRT == 1
  BigInteger isqrt() const {
    if (negative) throw std::domain_error("Square root of a negative number is not defined");
    if (is_zero()) return BigInteger(0);
    if (*this == BigInteger(1)) return BigInteger(1);

    BigInteger left(0), right(*this + BigInteger(1)), mid;
    while (left != right - BigInteger(1)) {
      mid = (left + right) / (digit)2;
      if (mid * mid <= *this) {
        if (mid < left)  {
          std::cout << (left + right)  << std::endl;
          std::cout << mid << std::endl;
          std::cout << left << std::endl;
          std::cout << right << std::endl;
          std::cout << *this << std::endl << std::endl;
          throw std::runtime_error("Overflow");
        }
        left = mid;
      } else {
        if (mid > right) {
          std::cout << (left + right)  << std::endl;
          std::cout << mid << std::endl;
          std::cout << left << std::endl;
          std::cout << right << std::endl;
          std::cout << *this << std::endl << std::endl;
          throw std::runtime_error("Overflow");
        }
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
  static BigInteger addAbs(const BigInteger &lhs, const BigInteger &rhs) {
    BigInteger result;
    size_t maxLength = std::max(lhs.size(), rhs.size() + 1);
    result.digits.resize(maxLength + 1, 0);

    for (size_t i = 0; i < maxLength; ++i) {
      digit a = i < lhs.size() ? lhs[i] : 0;
      digit b = i < rhs.size() ? rhs[i] : 0;
      digit res = result[i];
      d_digit sum = (d_digit)a + (d_digit)b + (d_digit)res;
      if (sum > digit_max) {
        result.digits[i] = sum & digit_max;
        if (i + 1 < maxLength) {
          result.digits[i + 1] = 1;
        } else {
          result.digits.push_back(1);
        }
      } else {
        result.digits[i] = sum;
      }
    }

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
  if (rhs.is_zero()) return BigInteger(lhs);
  if (lhs == 0) return -rhs;
  if (rhs.negative) return -((-rhs) + lhs);

  BigInteger result = BigInteger::subAbs(ABS(rhs), lhs);
  result.negative = true;
  return result.rlz();
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

  for (size_t i = 0; i < lhs.size(); ++i) {
    d_digit product = (d_digit)lhs[i] * (d_digit)rhs;
    result.add_to_digit(product & digit_max, i);
    result.add_to_digit(product >> digit_size, i + 1);
  }

  result.negative = lhs.negative;
  return result.rlz();
}
inline BigInteger operator*(const BigInteger lhs, const BigInteger &rhs) {
  if (lhs.is_zero() || rhs.is_zero()) return BigInteger(0);
  BigInteger result, carries;
  result.digits.resize(lhs.size() + rhs.size(), 0);
  for (size_t i = 0; i < lhs.size(); ++i) {
    for (size_t j = 0; j < rhs.digits.size(); ++j) {
      d_digit product = (d_digit)lhs.digits[i] * (d_digit)rhs.digits[j];
      result.add_to_digit(product & digit_max, i + j);
      result.add_to_digit(product >> digit_size, i + j + 1);
    }
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
digit normalize(digit number) {
  size_t count = __builtin_clz(number);
  return number << count;
}
BigInteger operator/(const BigInteger &lhs, digit rhs) {
  if (rhs == 0) throw std::runtime_error("Division by zero");
  size_t shift = __builtin_clz(rhs);
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
  BigInteger numerator = BigInteger(0);
  BigInteger denominator = BigInteger(1);
  bool isNegative = false;
  // constructors
  BigRational() {
    numerator = BigInteger(0);
    denominator = BigInteger(1);
    isNegative = false;
  };
  BigRational(int64_t a, int64_t b) {
    numerator = BigInteger(a);
    denominator = BigInteger(b);
  };
  BigRational(const std::string &a, const std::string &b) {
    numerator = BigInteger(a);
    denominator = BigInteger(b);
  };
  // copy
  BigRational(const BigRational &other) {
    numerator = other.numerator;
    denominator = other.denominator;
    isNegative = other.isNegative;
  };
  BigRational &operator=(const BigRational &rhs) {
    numerator = rhs.numerator;
    denominator = rhs.denominator;
    isNegative = rhs.isNegative;
    return *this;
  };
  // unary operators
  const BigRational &operator+() const { return *this; };
  BigRational operator-() const {
    BigRational result = *this;
    result.isNegative = !result.isNegative;
    return result;
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

  BigInteger gcd(BigInteger numerator, BigInteger denominator) const {
    while (!denominator.is_zero()) {
      BigInteger temp = denominator;
      denominator = numerator % denominator;
      numerator = temp;
    }

    return numerator;
  }

  BigRational normalize() const {
    BigRational result = *this;
    result.numerator.negative = false;
    result.denominator.negative = false;
    BigInteger gcd = result.gcd(result.numerator, result.denominator);
    result.numerator /= gcd;
    result.denominator /= gcd;
    return result;
  }

  BigRational addAbs(const BigRational &lhs, const BigRational &rhs) const {
    BigRational result;
    result.denominator = lhs.denominator * rhs.denominator;
    result.numerator = lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator;
    return result.normalize();
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
  if (lhs.isNegative && !rhs.isNegative) {  // -a + b = b - a
    return rhs - (-lhs);
  } else if (!lhs.isNegative && rhs.isNegative) {  // a + (-b) = a - b
    return lhs - (-rhs);
  } else {  // a + b or -a + (-b)
    BigRational temp = rhs, temp2 = lhs;
    temp.isNegative = false;
    temp2.isNegative = false;
    BigRational result = lhs.addAbs(temp2, temp);
    result.isNegative = lhs.isNegative && rhs.isNegative;
    return result;
  }
};
inline BigRational operator-(BigRational lhs, const BigRational &rhs) {
  if (lhs.numerator.is_zero()) {
    BigRational result = rhs;
    result.isNegative = !result.isNegative;
    return result.normalize();
  }
  if (rhs.numerator.is_zero()) return lhs;

  BigRational result;
  result.denominator = lhs.denominator * rhs.denominator;
  result.numerator = lhs.numerator * rhs.denominator - rhs.numerator * lhs.denominator;
  result.isNegative = lhs.isNegative ^ rhs.isNegative;
  return result.normalize();
};
inline BigRational operator*(BigRational lhs, const BigRational &rhs) {
  BigRational result;
  result.numerator = lhs.numerator * rhs.numerator;
  result.denominator = lhs.denominator * rhs.denominator;
  result.isNegative = lhs.isNegative ^ rhs.isNegative;
  return result.normalize();
};
inline BigRational operator/(const BigRational lhs, const BigRational &rhs) {
  BigRational result;
  result.numerator = lhs.numerator * rhs.denominator;
  result.denominator = lhs.denominator * rhs.numerator;
  result.isNegative = lhs.isNegative ^ rhs.isNegative;
  return result.normalize();
};
inline bool operator==(const BigRational &lhs, const BigRational &rhs) {
  lhs.normalize();
  rhs.normalize();
  if (lhs.isZero() && rhs.isZero()) {
    return true;
  }

  if (lhs.numerator != rhs.numerator) {
    return false;
  }

  if (lhs.denominator != rhs.denominator) {
    return false;
  }

  return true;
}
inline bool operator!=(const BigRational &lhs, const BigRational &rhs) { return !(lhs == rhs); };
inline bool operator<(const BigRational &lhs, const BigRational &rhs) {
  if (lhs.isNegative && !rhs.isNegative) return true;
  if (!lhs.isNegative && rhs.isNegative) return false;

  if (lhs.isNegative && rhs.isNegative) {
    lhs.normalize();
    rhs.normalize();
    return lhs.numerator * rhs.denominator > rhs.numerator * lhs.denominator;
  } else {
    lhs.normalize();
    rhs.normalize();
    return lhs.numerator * rhs.denominator < rhs.numerator * lhs.denominator;
  }
}
inline bool operator>(const BigRational &lhs, const BigRational &rhs) {
  if (lhs.isNegative && !rhs.isNegative) return false;
  if (!lhs.isNegative && rhs.isNegative) return true;

  if (lhs.isNegative && rhs.isNegative) {
    lhs.normalize();
    rhs.normalize();
    return lhs.numerator * rhs.denominator < rhs.numerator * lhs.denominator;
  } else {
    lhs.normalize();
    rhs.normalize();
    return lhs.numerator * rhs.denominator > rhs.numerator * lhs.denominator;
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
inline std::istream &operator>>(std::istream &lhs, BigRational &rhs);  // bonus
#endif

#if SUPPORT_EVAL == 1
inline BigInteger eval(const std::string &);
#endif
