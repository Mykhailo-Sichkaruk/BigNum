#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

// if you do not plan to implement bonus, you can delete those lines
// or just keep them as is and do not define the macro to 1
#define SUPPORT_IFSTREAM 1
#define SUPPORT_ISQRT 1
#define SUPPORT_EVAL 0 // special bonus

#define WORD uint32_t
#define WORD_MAX UINT32_MAX
#define DOUBLE_WORD uint64_t
#define HALF_WORD uint16_t
#define WORD_SIZE 32
#define HALF_WORD_SIZE 16

class BigInteger {
public:
  std::vector<WORD> digits; // Least Significant Digit first in vector
  bool isNegative = false;
  BigInteger() {
    digits = std::vector<WORD>();
    digits.push_back(0);
    isNegative = false;
  };

  BigInteger(int64_t value) {
    isNegative = (value < 0);
    value = std::abs(value);
    digits = std::vector<WORD>();

    digits.push_back(static_cast<WORD>(value & WORD_MAX));  // Lower 32 bits
    WORD upper = static_cast<WORD>(value >> WORD_SIZE); // Upper 32 bits

    if (upper > 0) {
      digits.push_back(upper);
    }
  }

  BigInteger(const std::string &number) : isNegative(false) {
    if (number.empty())
      throw std::invalid_argument("Empty string is not a valid BigInteger");

    digits = std::vector<WORD>();
    digits.push_back(0);
    size_t startPos = 0;
    if (number[0] == '-') {
      startPos = 1;
    } else if (number[0] == '+') {
      startPos = 1;
    }

    for (size_t i = startPos; i < number.size(); ++i) {
      char digit = number[i];
      if (std::isdigit(digit) == 0)
        throw std::invalid_argument("Invalid character in BigInteger string");

      *this *= BigInteger((WORD)10);
      *this += BigInteger((WORD)(digit - '0'));
    }

    if (number[0] == '-') {
      isNegative = true;
    }
  };
  // copy
  BigInteger(const BigInteger &other) {
    digits = std::vector<WORD>();
    digits = other.digits;
    isNegative = other.isNegative;
  };
  BigInteger &operator=(const BigInteger &rhs) {
    digits = std::vector<WORD>();
    digits = rhs.digits;
    isNegative = rhs.isNegative;
    return *this;
  };
  void addToPosDigit(WORD value, size_t pos) {
    if (pos >= digits.size()) {
      digits.resize(pos + 1, 0);
    }
    DOUBLE_WORD sum = (DOUBLE_WORD)digits[pos] + (DOUBLE_WORD)value;
    if (sum > WORD_MAX) {
      digits[pos] = sum & WORD_MAX;
      addToPosDigit(sum >> WORD_SIZE, pos + 1);
    } else {
      digits[pos] = sum;
    }
  };
  // unary operators
  const BigInteger &operator+() const { return *this; };
  BigInteger operator-() const {
    BigInteger result = *this;
    result.isNegative = !result.isNegative;
    return result;
  };
  // binary arithmetics operators
  BigInteger &operator+=(const BigInteger &rhs) {
    *this = *this + rhs;
    return *this;
  };
  BigInteger &operator-=(const BigInteger &rhs) {
    *this = *this - rhs;
    return *this;
  };
  BigInteger &operator*=(const BigInteger &rhs) {
    *this = *this * rhs;
    return *this;
  };
  BigInteger &operator/=(const BigInteger &rhs) {
    *this = *this / rhs;
    return *this;
  };
  BigInteger &operator%=(const BigInteger &rhs) {
    *this = *this % rhs;
    return *this;
  };

  // Returns new BigInteger with digits shifted left by shift
  BigInteger leftShift(BigInteger number, size_t shift){
    BigInteger result;
    result.digits.resize(number.digits.size() + shift, 0);
    for (size_t i = 0; i < number.digits.size(); ++i) {
      result.digits[i + shift] = number.digits[i];
    }
    return result;
  }

  std::string toString() const {
    if (digits.size() == 1 && digits[0] == 0) {
      return "0";
    }

    BigInteger temp = *this;
    std::string result;

    while (temp.digits.size() > 1 || temp.digits[0] != 0) {
      WORD remainder = temp.divideBySingleDigit(10);
      result.push_back('0' + remainder);
    }

    if (isNegative) {
      result.push_back('-');
    }

    std::reverse(result.begin(), result.end());
    return result;
  }

  static BigInteger multiplyByTwo(const BigInteger &num) {
    BigInteger result;
    result.digits.resize(num.digits.size(), 0);
    WORD carry = 0;
    for (size_t i = 0; i < num.digits.size(); i++) {
      uint64_t current = uint64_t(num.digits[i]) * 2 + carry;
      result.digits[i] = current & WORD_MAX;
      carry = current >> WORD_SIZE;
    }
    if (carry != 0) {
      result.digits.push_back(carry);
    }
    return result;
  };
  BigInteger &normalize() {
    while (!digits.empty() && digits.back() == 0) {
      digits.pop_back();
    }

    if (digits.empty()) {
      digits.push_back(0);
    }

    return *this;
  };
  // Square root function
  double sqrt() const {
    if (isNegative) {
      throw std::domain_error(
          "Square root of a negative number is not defined");
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

  operator double() const {
    double result = 0;
    for (int i = digits.size() - 1; i >= 0; --i) {
      result += result * (1LL << WORD_SIZE) + digits[i];
    }
    return isNegative ? -result : result;
  }
#if SUPPORT_ISQRT == 1
  // Integer square root function
  BigInteger isqrt() const {
    if (isNegative) {
      throw std::domain_error(
          "Square root of a negative number is not defined");
    }
    BigInteger left(0), right = *this, result;
    while (left <= right) {
      BigInteger mid = (left + right) / BigInteger(2);
      BigInteger square = mid * mid;

      if (square == *this) {
        return mid;
      } else if (square < *this) {
        left = mid + BigInteger(1);
        result = mid;
      } else {
        right = mid - BigInteger(1);
      }
    }
    return result;
  }
#endif

  static BigInteger subAbs(const BigInteger &lhs, const BigInteger &rhs) {
    if (compareMagnitude(lhs, rhs) < 0) {
      // Ensure lhs >= rhs in terms of absolute value
      throw std::invalid_argument(
          "lhs must be greater than or equal to rhs in absolute value");
    }

    BigInteger result;
    result.digits.resize(lhs.digits.size(), 0);
    bool borrow = false;

    for (size_t i = 0; i < lhs.digits.size(); ++i) {
      int64_t diff = int64_t(lhs.digits[i]) -
                     (i < rhs.digits.size() ? rhs.digits[i] : 0) - borrow;
      borrow = false;

      if (diff < 0) {
        diff += (1LL << WORD_SIZE); // Assuming WORD_SIZE is 32 for WORD
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

  BigInteger addAbs(BigInteger &lhs, BigInteger &rhs) {
    BigInteger result;
    size_t maxLength = std::max(lhs.digits.size(), rhs.digits.size());
    result.digits.resize(maxLength + 1, 0);

    for (size_t i = 0; i < maxLength; ++i) {
      WORD a = i < lhs.digits.size() ? lhs.digits[i] : 0;
      WORD b = i < rhs.digits.size() ? rhs.digits[i] : 0;
      WORD res = result.digits[i];
      DOUBLE_WORD sum = (DOUBLE_WORD)a + (DOUBLE_WORD)b + (DOUBLE_WORD)res;
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

    return result.normalize();
  }
  bool isZero() const { return digits.size() == 1 && digits[0] == 0; }

private:
  WORD divideBySingleDigit(WORD divisor) {
    DOUBLE_WORD remainder = 0;
    for (int i = digits.size() - 1; i >= 0; --i) {
      DOUBLE_WORD current = remainder * ((DOUBLE_WORD)WORD_MAX + 1) + digits[i];
      digits[i] = current / divisor;
      remainder = current % divisor;
    }

    while (!digits.empty() && digits.back() == 0) {
      digits.pop_back();
    }

    return remainder;
  }

  static int compareMagnitude(const BigInteger &a, const BigInteger &b) {
    if (a.digits.size() != b.digits.size()) {
      return a.digits.size() < b.digits.size() ? -1 : 1;
    }
    for (int i = a.digits.size() - 1; i >= 0; --i) {
      if (a.digits[i] != b.digits[i]) {
        return a.digits[i] < b.digits[i] ? -1 : 1;
      }
    }
    return 0; // Magnitudes are equal
  }
};

inline BigInteger operator+(BigInteger lhs, const BigInteger &rhs) {
  if (lhs.isZero()) {
    return rhs;
  } else if (rhs.isZero()) {
    return lhs;
  }
  if (lhs.isNegative && !rhs.isNegative) { // -a + b = b - a
    lhs.isNegative = false;
    return rhs - lhs;
  } else if (!lhs.isNegative && rhs.isNegative) { // a + (-b) = a - b
    BigInteger temp = rhs;
    temp.isNegative = false;
    return (lhs - temp).normalize();
  } else { // a + b or -a + (-b)
    BigInteger temp = rhs, temp2 = lhs;
    temp.isNegative = false;
    temp2.isNegative = false;
    BigInteger result = lhs.addAbs(temp2, temp);
    result.isNegative = lhs.isNegative && rhs.isNegative;
    return result;
  }
}

inline BigInteger operator-(BigInteger lhs, const BigInteger &rhs) {
  if (lhs.isZero()) {
    BigInteger result = rhs;
    result.isNegative = !result.isNegative;
    return result.normalize();
  } else if (rhs.isZero()) {
    return lhs;
  }

  BigInteger result;
  result.digits.resize(std::max(lhs.digits.size(), rhs.digits.size()), 0);

  if (lhs.isNegative && rhs.isNegative) { // -a - (-b) = -a + b = b - a
    return (-rhs) - lhs;
    /* BigInteger temp = rhs; */
    /* temp.isNegative = false; */
    /* lhs.isNegative = false; */
    /* return temp - lhs; */
  } else if (lhs.isNegative && !rhs.isNegative) { // -a - b = -(a + b)
    lhs.isNegative = false;
    result = lhs + rhs;
    result.isNegative = true;
    return result;
  } else if (!lhs.isNegative && rhs.isNegative) { // a - (-b) = a + b
    BigInteger temp = rhs;
    temp.isNegative = false;
    result = lhs + temp;
    return result;
  } else { // a - b
    if (lhs < rhs) {
      result = rhs - lhs;
      result.isNegative = true;
      return result;
    } else {
      result = BigInteger::subAbs(lhs, rhs);
      return result.normalize();
    }
  }
};

inline BigInteger operator*(BigInteger lhs, const BigInteger &rhs) {
  if (lhs.isZero() || rhs.isZero()) {
    return BigInteger((WORD)0);
  }

  BigInteger result, carries;
  result.digits.resize(lhs.digits.size() + rhs.digits.size(), 0);

  for (size_t i = 0; i < lhs.digits.size(); ++i) {
    for (size_t j = 0; j < rhs.digits.size(); ++j) {
      DOUBLE_WORD product =
          (DOUBLE_WORD)lhs.digits[i] * (DOUBLE_WORD)rhs.digits[j];
      result.addToPosDigit(product & WORD_MAX, i + j);
      result.addToPosDigit(product >> WORD_SIZE, i + j + 1);
    }
  }

  // Remove leading zeros
  while (result.digits.size() > 1 && result.digits.back() == 0) {
    result.digits.pop_back();
  }
  result.isNegative = lhs.isNegative ^ rhs.isNegative;

  return result;
};
inline BigInteger operator%(BigInteger lhs, const BigInteger &rhs) {
  if (rhs.digits.empty() || (rhs.digits.size() == 1 && rhs.digits[0] == 0)) {
    throw std::runtime_error("Modulo by zero");
  }

  BigInteger quotient = lhs / rhs;
  BigInteger remainder = lhs - quotient * rhs;

  remainder.isNegative = false;

  while (remainder.digits.size() > 1 && remainder.digits.back() == 0) {
    remainder.digits.pop_back();
  }

  return remainder;
};
inline BigInteger operator/(BigInteger lhs, const BigInteger &rhs) {
  if (rhs.isZero()) {
    throw std::runtime_error("Division by zero");
  }

  if (lhs.isZero()) {
    return BigInteger((WORD)0);
  }

  if (lhs == BigInteger(1)) {
    return BigInteger(1);
  }

  if (lhs < rhs) {
    return BigInteger((WORD)0);
  }

  // TODO: implement [] operator to access digits
  BigInteger result;
  result.digits.resize(lhs.digits.size(), 0);
  BigInteger temp = lhs;
  temp.isNegative = false;
  BigInteger currentQuotient;

  BigInteger tempDivisor = rhs;
  tempDivisor.isNegative = false;
  for (size_t i = lhs.digits.size(); i-- > 0;) {
    result.digits[i] = 0;
    while (temp >= tempDivisor) {
      temp -= tempDivisor;
      result.digits[i]++;
    }
    temp.digits.insert(temp.digits.begin(), lhs.digits[i]);
  }

  result.isNegative = lhs.isNegative ^ rhs.isNegative;
  return result.normalize();
};

inline bool operator==(const BigInteger &lhs, const BigInteger &rhs) {
  if (lhs.isZero() && rhs.isZero()) {
    return true;
  }

  if (lhs.digits.size() != rhs.digits.size()) {
    return false;
  }

  if (lhs.isNegative != rhs.isNegative) {
    return false;
  }

  for (size_t i = lhs.digits.size(); i-- > 0;) {
    if (lhs.digits[i] != rhs.digits[i]) {
      return false;
    }
  }

  return true;
};
inline bool operator!=(const BigInteger &lhs, const BigInteger &rhs) {
  return !(lhs == rhs);
};
inline bool operator<(const BigInteger &lhs, const BigInteger &rhs) {
  return rhs > lhs && rhs != lhs;
}
inline bool operator>(const BigInteger &lhs, const BigInteger &rhs) {
  if (lhs.isNegative && !rhs.isNegative) {
    return false;
  } else if (!lhs.isNegative && rhs.isNegative) {
    return true;
  } else if (lhs.isNegative && rhs.isNegative) {
    return -lhs < -rhs;
  }
  if (lhs.digits.size() != rhs.digits.size()) {
    return lhs.digits.size() > rhs.digits.size();
  }
  for (size_t i = lhs.digits.size(); i-- > 0;) {
    if (lhs.digits[i] != rhs.digits[i]) {
      return lhs.digits[i] > rhs.digits[i];
    }
  }
  return false;
};
inline bool operator<=(const BigInteger &lhs, const BigInteger &rhs) {
  return lhs < rhs || lhs == rhs;
};
inline bool operator>=(const BigInteger &lhs, const BigInteger &rhs) {
  return lhs > rhs || lhs == rhs;
};
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


class BigInttegerShifted : BigInteger {
public:
  BigInttegerShifted() : BigInteger() { shift = 0; };

private:
  size_t shift;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
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

  bool isZero() const { return numerator.isZero(); }

  BigInteger gcd(BigInteger numerator, BigInteger denominator) const {
    while (!denominator.isZero()) {
      BigInteger temp = denominator;
      denominator = numerator % denominator;
      numerator = temp;
    }

    return numerator;
  }

  BigRational normalize() const {
    BigRational result = *this;
    result.numerator.isNegative = false;
    result.denominator.isNegative = false;
    BigInteger gcd = result.gcd(result.numerator, result.denominator);
    result.numerator /= gcd;
    result.denominator /= gcd;
    return result;
  }

  BigRational addAbs(const BigRational &lhs, const BigRational &rhs) const {
    BigRational result;
    result.denominator = lhs.denominator * rhs.denominator;
    result.numerator =
        lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator;
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
  if (lhs.numerator.isZero()) {
    return rhs;
  } else if (rhs.numerator.isZero()) {
    return lhs;
  }
  if (lhs.isNegative && !rhs.isNegative) { // -a + b = b - a
    lhs.isNegative = false;
    BigRational temp = rhs;
    return temp - lhs;
  } else if (!lhs.isNegative && rhs.isNegative) { // a + (-b) = a - b
    BigRational temp = rhs;
    temp.isNegative = false;
    return (lhs - temp).normalize();
  } else { // a + b or -a + (-b)
    BigRational temp = rhs, temp2 = lhs;
    temp.isNegative = false;
    temp2.isNegative = false;
    BigRational result = lhs.addAbs(temp2, temp);
    result.isNegative = lhs.isNegative && rhs.isNegative;
    return result;
  }
};
inline BigRational operator-(BigRational lhs, const BigRational &rhs) {
  if (lhs.numerator.isZero()) {
    BigRational result = rhs;
    result.isNegative = !result.isNegative;
    return result.normalize();
  } else if (rhs.numerator.isZero()) {
    return lhs;
  }

  BigRational result;
  result.denominator = lhs.denominator * rhs.denominator;
  result.numerator =
      lhs.numerator * rhs.denominator - rhs.numerator * lhs.denominator;
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
inline BigRational operator/(BigRational lhs, const BigRational &rhs) {
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
inline bool operator!=(const BigRational &lhs, const BigRational &rhs) {
  return !(lhs == rhs);
};
inline bool operator<(const BigRational &lhs, const BigRational &rhs) {
  if (lhs.isNegative && !rhs.isNegative)
    return true;
  if (!lhs.isNegative && rhs.isNegative)
    return false;

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
  if (lhs.isNegative && !rhs.isNegative)
    return false;
  if (!lhs.isNegative && rhs.isNegative)
    return true;

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
inline std::istream &operator>>(std::istream &lhs, BigRational &rhs); // bonus
#endif

#if SUPPORT_EVAL == 1
inline BigInteger eval(const std::string &);
#endif
