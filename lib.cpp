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

using WORD = uint32_t;
constexpr auto WORD_MAX = UINT32_MAX;
using DWORD = uint64_t;
using HALF_WORD = uint16_t;
constexpr auto WORD_SIZE = 32;
constexpr auto HALF_WORD_SIZE = 16;

class BigInteger {
 public:
  std::vector<WORD> digits;  // Least Significant Digit first in vector
  bool negative;

  BigInteger() : digits(1, 0), negative(false) {}
  BigInteger(int64_t value) : negative(value < 0), digits(1, 0) {
    value = std::abs(value);
    digits[0] = static_cast<WORD>(value & WORD_MAX);
    WORD upper = static_cast<WORD>(value >> WORD_SIZE);
    if (upper > 0) {
      digits.push_back(upper);
    }
  }
  explicit BigInteger(const std::string &number) : BigInteger() {
    if (number.empty()) throw std::invalid_argument("Empty string is not a valid BigInteger");

    size_t startPos = 0;
    if (number[0] == '-' || number[0] == '+') {
      startPos = 1;
    }

    for (size_t i = startPos; i < number.size(); ++i) {
      char digit = number[i];
      if (std::isdigit(digit) == 0)
        throw std::invalid_argument("Invalid character in BigInteger string");

      *this *= BigInteger(10);
      *this += BigInteger(digit - '0');
    }

    if (number[0] == '-') {
      negative = true;
    }
  }
  BigInteger(const BigInteger &other) = default;
  BigInteger &operator=(const BigInteger &rhs) = default;
  void addToPosDigit(WORD value, size_t pos) {
    if (pos >= digits.size()) {
      digits.resize(pos + 1, 0);
    }
    DWORD sum = (DWORD)digits[pos] + (DWORD)value;
    if (sum > WORD_MAX) {
      digits[pos] = sum & WORD_MAX;
      addToPosDigit(sum >> WORD_SIZE, pos + 1);
    } else {
      digits[pos] = sum;
    }
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
    WORD x = digits.back();
    if (x == 0) return 0;
    return __builtin_clz(x);
  }
  void normalize() {
    remove_leading_zero_digits();
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
    for (int i = digits.size() - 1; i >= 0; i--) {
      WORD current = digits[i];
      for (int j = 0; j < WORD_SIZE; ++j) {
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
    result.reserve(digits.size() * (std::log10(WORD_MAX) + 1));  // Estimated reserve

    while (temp.digits.size() > 1 || temp.digits[0] != 0) {
      WORD remainder = temp.divideBySingleDigit(10);
      result.push_back('0' + remainder);
    }

    if (negative) {
      result.push_back('-');
    }

    std::reverse(result.begin(), result.end());
    return result;
  }
  inline BigInteger &remove_leading_zero_digits() {
    while (!digits.empty() && digits.back() == 0) {
      digits.pop_back();
    }

    if (digits.empty()) {
      digits.push_back(0);
    }

    return *this;
  }
  inline BigInteger &divideByTwo() {
    WORD remainder = 0;
    for (int i = digits.size() - 1; i >= 0; --i) {
      WORD current = digits[i];
      digits[i] = (current >> 1) | remainder;
      remainder = (current & 1) << (WORD_SIZE - 1);
    }

    remove_leading_zero_digits();

    return *this;
  }
  inline BigInteger &multiplyByTwo() {
    WORD carry = 0;
    for (size_t i = 0; i < digits.size(); ++i) {
      WORD current = digits[i];
      digits[i] = (current << 1) | carry;
      carry = current >> (WORD_SIZE - 1);
    }

    if (carry != 0) {
      digits.push_back(carry);
    }

    return *this;
  }
  BigInteger multiplyByTwo() const {
    BigInteger result = *this;
    result.multiplyByTwo();
    return result;
  }
  double sqrt() const {
    if (negative) {
      throw std::domain_error("Square root of a negative number is not defined");
    }
    return std::sqrt(static_cast<double>(*this));
  }
  friend bool operator<(const BigInteger &lhs, const BigInteger &rhs);
  friend bool operator>(const BigInteger &lhs, const BigInteger &rhs);
  friend bool operator<=(const BigInteger &lhs, const BigInteger &rhs);
  friend bool operator>=(const BigInteger &lhs, const BigInteger &rhs);
  friend bool operator==(const BigInteger &lhs, const BigInteger &rhs);
  friend BigInteger operator+(BigInteger lhs, const BigInteger &rhs);
  friend BigInteger operator*(BigInteger lhs, const BigInteger &rhs);
  friend BigInteger operator-(BigInteger lhs, const BigInteger &rhs);
  friend BigInteger operator/(BigInteger lhs, const BigInteger &rhs);
  friend BigInteger operator%(BigInteger lhs, const BigInteger &rhs);
  friend BigInteger left_shift(BigInteger number, size_t shift);

  operator double() const {
    double result = 0;
    for (int i = digits.size() - 1; i >= 0; --i) {
      result += result * (1LL << WORD_SIZE) + digits[i];
    }
    return negative ? -result : result;
  }

  WORD operator[](size_t index) const {
    if (index >= digits.size()) {
      return 0;
    }
    return digits[index];
  }

  size_t size() const { return digits.size(); }
#if SUPPORT_ISQRT == 1
  // Integer square root function
  BigInteger isqrt() const {
    if (negative) {
      throw std::domain_error("Square root of a negative number is not defined");
    }
    if (is_zero()) return BigInteger(0);
    if (*this == BigInteger(1)) return BigInteger(1);

    BigInteger left = BigInteger(1);
    BigInteger right = *this;
    BigInteger mid;
    while (left < right) {
      mid = (left + right) / BigInteger(2);
      BigInteger square = mid * mid;
      if (square == *this) {
        return mid;
      } else if (square < *this) {
        left = mid + BigInteger(1);
      } else {
        right = mid - BigInteger(1);
      }
    }

    return left;
  }
#endif
  // Return lhs - rhs assuming lhs >= rhs in absolute value of both numbers
  static BigInteger subAbs(const BigInteger &lhs, const BigInteger &rhs) {
    BigInteger result;
    result.digits.resize(lhs.size(), 0);
    bool borrow = false;

    for (size_t i = 0; i < lhs.size(); ++i) {
      int64_t diff = int64_t(lhs[i]) - (i < rhs.size() ? rhs[i] : 0) - borrow;
      borrow = false;

      if (diff < 0) {
        diff += (1LL << WORD_SIZE);  // Assuming WORD_SIZE is 32 for WORD
        borrow = true;
      }

      result.digits[i] = static_cast<WORD>(diff);
    }

    // Remove leading zeros from the result
    while (!result.digits.empty() && result.digits.back() == 0) {
      result.digits.pop_back();
    }

    return result;
  }

  void clear() {
    // Set all digits to zero
    std::fill(digits.begin(), digits.end(), 0);
  }
  BigInteger addAbs(const BigInteger &lhs, const BigInteger &rhs) const {
    BigInteger result;
    size_t maxLength = std::max(lhs.size(), rhs.size());
    result.digits.resize(maxLength + 1, 0);

    for (size_t i = 0; i < maxLength; ++i) {
      WORD a = i < lhs.size() ? lhs[i] : 0;
      WORD b = i < rhs.size() ? rhs[i] : 0;
      WORD res = result[i];
      DWORD sum = (DWORD)a + (DWORD)b + (DWORD)res;
      if (sum > WORD_MAX) {
        result.digits[i] = sum & WORD_MAX;
        if (i + 1 < maxLength) {
          result.digits[i + 1] = 1;
        } else {
          result.digits.push_back(1);
        }
      } else {
        result.digits[i] = sum;
      }
    }

    return result.remove_leading_zero_digits();
  }
  bool is_zero() const { return digits.size() == 1 && digits[0] == 0; }

 private:
  // Returns remainder, changes the number to the quotient
  WORD divideBySingleDigit(WORD divisor) {
    if (divisor == 0) throw std::runtime_error("Division by zero");
    if (divisor == 1) return 0;
    if (divisor == 2) return divideByTwo();

    DWORD remainder = 0;
    for (int i = digits.size() - 1; i >= 0; --i) {
      DWORD current = remainder * ((DWORD)WORD_MAX + 1) + digits[i];
      digits[i] = current / divisor;
      remainder = current % divisor;
    }

    remove_leading_zero_digits();

    return remainder;
  }

  static int compareMagnitude(const BigInteger &a, const BigInteger &b) {
    if (a.size() != b.size()) {
      return a.size() < b.size() ? -1 : 1;
    }
    for (int i = a.size() - 1; i >= 0; --i) {
      if (a[i] != b[i]) {
        return a[i] < b[i] ? -1 : 1;
      }
    }
    return 0;  // Magnitudes are equal
  }
};

inline BigInteger ABS(const BigInteger &number) {
  BigInteger result = number;
  result.negative = false;
  return result;
}
// Returns new BigInteger with digits shifted left by shift, if number is zero return zero
BigInteger left_shift(const BigInteger number, const size_t shift) {
  if (number.is_zero()) return number;
  if (shift == 0) return number;

  WORD digit_shift = shift / WORD_SIZE;
  WORD bit_shift = shift % WORD_SIZE;
  BigInteger result;
  result.negative = number.negative;
  result.digits.resize(number.size() + digit_shift + 1, 0);

  WORD carry = 0;
  for (size_t i = 0; i < number.size() + digit_shift; ++i) {
    WORD current = number[i];
    result.digits[i + digit_shift] = (current << bit_shift) | carry;
    carry = current >> (WORD_SIZE - bit_shift);
  }
  if (carry != 0) {
    result.digits[number.size() + digit_shift] = carry;
  }

  result.remove_leading_zero_digits();
  return result;
}

inline BigInteger operator+(const BigInteger lhs, const BigInteger &rhs) {
  if (lhs.is_zero()) {
    return rhs;
  } else if (rhs.is_zero()) {
    return lhs;
  }

  if (lhs.negative && !rhs.negative) {  // -a + b = b - a
    return rhs - (-lhs);
  } else if (!lhs.negative && rhs.negative) {  // a + (-b) = a - b
    return lhs - (-rhs);
  } else {  // a + b or -a + (-b)
    BigInteger temp = rhs, temp2 = lhs;
    temp.negative = false;
    temp2.negative = false;
    BigInteger result = lhs.addAbs(temp2, temp);
    result.negative = lhs.negative && rhs.negative;
    return result;
  }
}

inline BigInteger operator-(const BigInteger lhs, const BigInteger &rhs) {
  if (lhs.is_zero()) {
    return -rhs;
  } else if (rhs.is_zero()) {
    return lhs;
  }

  BigInteger result;
  result.digits.resize(std::max(lhs.size(), rhs.size()), 0);

  if (lhs.negative && rhs.negative) {  // -a - (-b) = -a + b = b - a
    return (-rhs) - (-lhs);
  } else if (lhs.negative && !rhs.negative) {  // -a - b = -(a + b)
    return -((-lhs) + rhs);
  } else if (!lhs.negative && rhs.negative) {  // a - (-b) = a + b
    return lhs + (-rhs);
  } else {  // a - b
    if (lhs < rhs) {
      result = rhs - lhs;
      result.negative = true;
      return result.remove_leading_zero_digits();
    } else {
      result = BigInteger::subAbs(lhs, rhs);
      return result.remove_leading_zero_digits();
    }
  }
}

inline BigInteger operator*(const BigInteger lhs, const WORD rhs) {
  if (lhs.is_zero() || rhs == 0) return BigInteger(0);
  if (lhs == BigInteger(1)) return BigInteger(rhs);
  if (rhs == 1) return lhs;
  if (rhs == 2) return lhs.multiplyByTwo();

  BigInteger result;
  result.digits.resize(lhs.size() + 1, 0);

  for (size_t i = 0; i < lhs.size(); ++i) {
    DWORD product = (DWORD)lhs[i] * (DWORD)rhs;
    result.addToPosDigit(product & WORD_MAX, i);
    result.addToPosDigit(product >> WORD_SIZE, i + 1);
  }

  // Remove leading zeros
  while (result.size() > 1 && result.digits.back() == 0) {
    result.digits.pop_back();
  }
  result.negative = lhs.negative;
  return result;
}
inline BigInteger operator*(const BigInteger lhs, const BigInteger &rhs) {
  if (lhs.is_zero() || rhs.is_zero()) return BigInteger(0);
  if (lhs == BigInteger(1)) return rhs;
  if (rhs == BigInteger(1)) return lhs;

  BigInteger result, carries;
  result.digits.resize(lhs.size() + rhs.size(), 0);

  for (size_t i = 0; i < lhs.size(); ++i) {
    for (size_t j = 0; j < rhs.digits.size(); ++j) {
      DWORD product = (DWORD)lhs.digits[i] * (DWORD)rhs.digits[j];
      result.addToPosDigit(product & WORD_MAX, i + j);
      result.addToPosDigit(product >> WORD_SIZE, i + j + 1);
    }
  }

  // Remove leading zeros
  while (result.size() > 1 && result.digits.back() == 0) {
    result.digits.pop_back();
  }
  result.negative = lhs.negative ^ rhs.negative;

  return result;
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
  auto remainder = ABS(lhs) - ABS((quotient * rhs));

  remainder.negative = lhs.negative;

  return remainder;
}
inline BigInteger operator<(const BigInteger lhs, WORD rhs) {
  if (lhs.size() > 1) return false;
  return lhs[0] < rhs;
}
inline BigInteger operator>(const BigInteger lhs, WORD rhs) {
  if (lhs.size() > 1) return true;
  return lhs[0] > rhs;
}
WORD normalize(WORD number) {
  size_t count = __builtin_clz(number);
  return number << count;
}
BigInteger operator/(const BigInteger lhs, WORD rhs) {
  if (rhs == 0) throw std::runtime_error("Division by zero");
  size_t shift = __builtin_clz(rhs);
  rhs = normalize(rhs);
  auto dividend = left_shift(lhs, shift);
  BigInteger result;
  result.digits.resize(dividend.size(), 0);
  DWORD remainder = 0;
  for (int i = dividend.size() - 1; i >= 0; --i) {
    DWORD current_digit = (remainder * ((DWORD)WORD_MAX + 1)) + (DWORD)dividend[i];
    result.digits[i] = current_digit / rhs;
    remainder = current_digit % rhs;
  }

  return result.remove_leading_zero_digits();
}
static inline WORD digit_div(WORD high, WORD low, WORD divisor, WORD *remainder) {
  DWORD dividend = ((DWORD)high << WORD_SIZE) + (DWORD)low;
  DWORD quotient = dividend / divisor;
  *remainder = dividend % divisor;
  return quotient;
}
inline BigInteger operator/(const BigInteger lhs, const BigInteger &rhs) {
  if (rhs.is_zero()) throw std::runtime_error("Division by zero");
  if (lhs.is_zero()) return BigInteger(0);
  if (lhs == BigInteger(1)) return BigInteger(1);
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

  BigInteger left_q(2);
  BigInteger right_q(dividend);

  BigInteger current_q;
  while (left_q < right_q) {
    current_q = (left_q + right_q).divideByTwo();
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
      left_q = current_q + BigInteger(1);
    }
  }

  left_q.negative = lhs.negative ^ rhs.negative;
  return left_q;
}

inline bool operator==(const BigInteger &lhs, const BigInteger &rhs) {
  if (lhs.is_zero() && rhs.is_zero()) return true;
  if (lhs.size() != rhs.size()) return false;
  if (lhs.negative != rhs.negative) return false;

  for (size_t i = lhs.size(); i-- > 0;) {
    if (lhs[i] != rhs[i]) return false;
  }

  return true;
};
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
inline std::ostream &operator<<(std::ostream &lhs, const BigInteger &rhs) {
  lhs << rhs.toString();
  return lhs;
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
  } else if (rhs.numerator.is_zero()) {
    return lhs;
  }

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

// alternatively you can implement
// std::strong_ordering operator<=>(const BigRational& lhs, const BigRational&
// rhs); idea is, that all comparison should work, it is not important how you
// do it
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
};
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
