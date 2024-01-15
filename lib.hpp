#include <stdint.h>
#include <string>

// if you do not plan to implement bonus, you can delete those lines
// or just keep them as is and do not define the macro to 1
#define SUPPORT_IFSTREAM 0
#define SUPPORT_ISQRT 0
#define SUPPORT_EVAL 0 // special bonus

class BigInteger {
public:
  // constructors
  BigInteger();
  BigInteger(int64_t n);
  explicit BigInteger(const std::string &str);
  // copy
  BigInteger(const BigInteger &other);
  BigInteger &operator=(const BigInteger &rhs);
  // unary operators
  const BigInteger &operator+() const;
  BigInteger operator-() const;
  // binary arithmetics operators
  BigInteger &operator+=(const BigInteger &rhs);
  BigInteger &operator-=(const BigInteger &rhs);
  BigInteger &operator*=(const BigInteger &rhs);
  BigInteger &operator/=(const BigInteger &rhs);
  BigInteger &operator%=(const BigInteger &rhs);

  double sqrt() const;
#if SUPPORT_ISQRT == 1
  BigInteger isqrt() const;
#endif
private:
  // here you can add private data and members, but do not add stuff to
  // public interface, also you can declare friends here if you want
};

inline BigInteger operator+(BigInteger lhs, const BigInteger &rhs);
inline BigInteger operator-(BigInteger lhs, const BigInteger &rhs);
inline BigInteger operator*(BigInteger lhs, const BigInteger &rhs);
inline BigInteger operator/(BigInteger lhs, const BigInteger &rhs);
inline BigInteger operator%(BigInteger lhs, const BigInteger &rhs);

// alternatively you can implement
// std::strong_ordering operator<=>(const BigInteger& lhs, const BigInteger&
// rhs); idea is, that all comparison should work, it is not important how you
// do it
inline bool operator==(const BigInteger &lhs, const BigInteger &rhs);
inline bool operator!=(const BigInteger &lhs, const BigInteger &rhs);
inline bool operator<(const BigInteger &lhs, const BigInteger &rhs);
inline bool operator>(const BigInteger &lhs, const BigInteger &rhs);
inline bool operator<=(const BigInteger &lhs, const BigInteger &rhs);
inline bool operator>=(const BigInteger &lhs, const BigInteger &rhs);

inline std::ostream &operator<<(std::ostream &lhs, const BigInteger &rhs);

#if SUPPORT_IFSTREAM == 1
// this should behave exactly the same as reading int with respect to
// whitespace, consumed characters etc...
inline std::istream &operator>>(std::istream &lhs, BigInteger &rhs); // bonus
#endif

class BigRational {
public:
  // constructors
  BigRational();
  BigRational(int64_t a, int64_t b);
  BigRational(const std::string &a, const std::string &b);
  // copy
  BigRational(const BigRational &other);
  BigRational &operator=(const BigRational &rhs);
  // unary operators
  const BigRational &operator+() const;
  BigRational operator-() const;
  // binary arithmetics operators
  BigRational &operator+=(const BigRational &rhs);
  BigRational &operator-=(const BigRational &rhs);
  BigRational &operator*=(const BigRational &rhs);
  BigRational &operator/=(const BigRational &rhs);

  double sqrt() const;
#if SUPPORT_ISQRT == 1
  BigInteger isqrt() const;
#endif
private:
  // here you can add private data and members, but do not add stuff to
  // public interface, also you can declare friends here if you want
};

inline BigRational operator+(BigRational lhs, const BigRational &rhs);
inline BigRational operator-(BigRational lhs, const BigRational &rhs);
inline BigRational operator*(BigRational lhs, const BigRational &rhs);
inline BigRational operator/(BigRational lhs, const BigRational &rhs);

// alternatively you can implement
// std::strong_ordering operator<=>(const BigRational& lhs, const BigRational&
// rhs); idea is, that all comparison should work, it is not important how you
// do it
inline bool operator==(const BigRational &lhs, const BigRational &rhs);
inline bool operator!=(const BigRational &lhs, const BigRational &rhs);
inline bool operator<(const BigRational &lhs, const BigRational &rhs);
inline bool operator>(const BigRational &lhs, const BigRational &rhs);
inline bool operator<=(const BigRational &lhs, const BigRational &rhs);
inline bool operator>=(const BigRational &lhs, const BigRational &rhs);

inline std::ostream &operator<<(std::ostream &lhs, const BigRational &rhs);

#if SUPPORT_IFSTREAM == 1
// this should behave exactly the same as reading int with respect to
// whitespace, consumed characters etc...
inline std::istream &operator>>(std::istream &lhs, BigRational &rhs); // bonus
#endif

#if SUPPORT_EVAL == 1
inline BigInteger eval(const std::string &);
#endif
