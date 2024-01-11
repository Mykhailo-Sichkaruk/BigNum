#include "catch_amalgamated.cpp"
#include "lib.cpp"

std::string BigIntegerToString(BigInteger value) { return value.toString(); }

TEST_CASE("BigInteger Square Root") {
  CHECK("0" == BigIntegerToString(BigInteger(0)));
  CHECK("1" == BigIntegerToString(BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-1)));
  CHECK("123456789" == BigIntegerToString(BigInteger(123456789)));
  CHECK("-123456789" == BigIntegerToString(BigInteger(-123456789)));
  CHECK("2147483647" ==
        BigIntegerToString(BigInteger(std::numeric_limits<int32_t>::max())));
  CHECK("-2147483648" ==
        BigIntegerToString(BigInteger(std::numeric_limits<int32_t>::min())));
  CHECK("-2147483648" ==
        BigIntegerToString(BigInteger(std::numeric_limits<int32_t>::min())));
  CHECK("9223372036854775807" ==
        BigIntegerToString(BigInteger(std::numeric_limits<int64_t>::max())));
  CHECK("-9223372036854775808" ==
        BigIntegerToString(BigInteger(std::numeric_limits<int64_t>::min())));
  CHECK(BigInteger("0") == BigInteger(0));
  CHECK(BigInteger("-0") == BigInteger(0));
  CHECK(BigInteger("1") == BigInteger(1));
  CHECK(BigInteger("-1") == BigInteger(-1));
  CHECK(BigInteger("1000000000") == BigInteger(1000000000));
  CHECK(BigInteger("-1000000000") == BigInteger(-1000000000));
  CHECK(BigInteger("0") != BigInteger(1));
  CHECK(BigInteger("-0") != BigInteger(-1));
  CHECK(BigInteger("1") != BigInteger(2));
  CHECK(BigInteger("-1") != BigInteger(1));
  CHECK(BigInteger("1000000000") != BigInteger(1000000001));
  CHECK(BigInteger("-1000000000") != BigInteger(-1000000001));
  CHECK("-1" == BigIntegerToString(-BigInteger(1)));
  CHECK("1" == BigIntegerToString(-(-BigInteger(1))));
  CHECK("0" == BigIntegerToString(-BigInteger(0)));
  CHECK("-1000" == BigIntegerToString(-BigInteger(1000)));
  CHECK("1000" == BigIntegerToString(-BigInteger(-1000)));
  CHECK("-123456789123" == BigIntegerToString(-BigInteger(123456789123)));
  CHECK("123456789123" == BigIntegerToString(-BigInteger(-123456789123)));
  CHECK("3" == BigIntegerToString(BigInteger(2) + BigInteger(1)));
  CHECK("1" == BigIntegerToString(BigInteger(2) + BigInteger(-1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-2) + BigInteger(1)));
  CHECK("-3" == BigIntegerToString(BigInteger(-2) + BigInteger(-1)));
  CHECK("123" == BigIntegerToString(BigInteger(123) + BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(-123) + BigInteger(0)));
  CHECK("1" == BigIntegerToString(BigInteger(2) - BigInteger(1)));
  CHECK("3" == BigIntegerToString(BigInteger(2) - BigInteger(-1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-2) - BigInteger(-1)));
  CHECK("123" == BigIntegerToString(BigInteger(123) - BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(-123) - BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(0) - BigInteger(123)));
  CHECK("123" == BigIntegerToString(BigInteger(0) - BigInteger(-123)));
  CHECK(BigInteger(0).sqrt() == Catch::Approx(0));
  CHECK(BigInteger(1).sqrt() == Catch::Approx(1));
  CHECK(BigInteger(2).sqrt() == Catch::Approx(1.4142135624));
  CHECK(BigInteger(3).sqrt() == Catch::Approx(1.7320508076));
  CHECK(BigInteger(4).sqrt() == Catch::Approx(2));
  CHECK(BigInteger(1000).sqrt() == Catch::Approx(31.6227766017));
  CHECK(BigInteger(10000099).sqrt() == Catch::Approx(3162.2933134041));
  CHECK(BigInteger(100000990099).sqrt() == Catch::Approx(316229.3314969375));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger(0)));
  CHECK("0" == BigIntegerToString(BigInteger(123) * BigInteger(0)));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger(-123)));
  CHECK("0" ==
        BigIntegerToString(BigInteger(0) * BigInteger("123456789123456789")));
  CHECK("1" == BigIntegerToString(BigInteger(1) * BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-1) * BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(1) * BigInteger(-1)));
  CHECK("1" == BigIntegerToString(BigInteger(-1) * BigInteger(-1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) / BigInteger(1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) % BigInteger(1)));
  CHECK("7" == BigIntegerToString(BigInteger(57) / BigInteger(8)));
  CHECK("1" == BigIntegerToString(BigInteger(57) % BigInteger(8)));
  CHECK("0" == BigIntegerToString(BigInteger(0)));
  CHECK("1" == BigIntegerToString(BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-1)));
  CHECK("123456789" == BigIntegerToString(BigInteger(123456789)));
  CHECK("-123456789" == BigIntegerToString(BigInteger(-123456789)));
  CHECK("2147483647" == BigIntegerToString(BigInteger(std::numeric_limits<int32_t>::max())));
  CHECK("2147483647" == BigIntegerToString(BigInteger(std::numeric_limits<int32_t>::max())));
  CHECK("2147483647" == BigIntegerToString(BigInteger(std::numeric_limits<int32_t>::max())));
  CHECK("-2147483648" == BigIntegerToString(BigInteger(std::numeric_limits<int32_t>::min())));
  CHECK("-2147483648" == BigIntegerToString(BigInteger(std::numeric_limits<int32_t>::min())));
  CHECK("-2147483648" == BigIntegerToString(BigInteger(std::numeric_limits<int32_t>::min())));
  CHECK("9223372036854775807" == BigIntegerToString(BigInteger(std::numeric_limits<int64_t>::max())));
  CHECK("9223372036854775807" == BigIntegerToString(BigInteger(std::numeric_limits<int64_t>::max())));
  CHECK("9223372036854775807" == BigIntegerToString(BigInteger(std::numeric_limits<int64_t>::max())));
  CHECK("-9223372036854775808" == BigIntegerToString(BigInteger(std::numeric_limits<int64_t>::min())));
  CHECK("-9223372036854775808" == BigIntegerToString(BigInteger(std::numeric_limits<int64_t>::min())));
  CHECK("-9223372036854775808" == BigIntegerToString(BigInteger(std::numeric_limits<int64_t>::min())));
  CHECK(BigInteger("0") == BigInteger(0));
  CHECK(BigInteger("0") == BigInteger(0));
  CHECK(BigInteger("0") == BigInteger(0));
  CHECK(BigInteger("-0") == BigInteger(0));
  CHECK(BigInteger("-0") == BigInteger(0));
  CHECK(BigInteger("-0") == BigInteger(0));
  CHECK(BigInteger("1") == BigInteger(1));
  CHECK(BigInteger("1") == BigInteger(1));
  CHECK(BigInteger("1") == BigInteger(1));
  CHECK(BigInteger("-1") == BigInteger(-1));
  CHECK(BigInteger("-1") == BigInteger(-1));
  CHECK(BigInteger("-1") == BigInteger(-1));
  CHECK(BigInteger("1000000000") == BigInteger(1000000000));
  CHECK(BigInteger("1000000000") == BigInteger(1000000000));
  CHECK(BigInteger("1000000000") == BigInteger(1000000000));
  CHECK(BigInteger("-1000000000") == BigInteger(-1000000000));
  CHECK(BigInteger("-1000000000") == BigInteger(-1000000000));
  CHECK(BigInteger("-1000000000") == BigInteger(-1000000000));
  CHECK(BigInteger("0") != BigInteger(1));
  CHECK(BigInteger("0") != BigInteger(1));
  CHECK(BigInteger("0") != BigInteger(1));
  CHECK(BigInteger("-0") != BigInteger(-1));
  CHECK(BigInteger("-0") != BigInteger(-1));
  CHECK(BigInteger("-0") != BigInteger(-1));
  CHECK(BigInteger("1") != BigInteger(2));
  CHECK(BigInteger("1") != BigInteger(2));
  CHECK(BigInteger("1") != BigInteger(2));
  CHECK(BigInteger("-1") != BigInteger(1));
  CHECK(BigInteger("-1") != BigInteger(1));
  CHECK(BigInteger("-1") != BigInteger(1));
  CHECK(BigInteger("1000000000") != BigInteger(1000000001));
  CHECK(BigInteger("1000000000") != BigInteger(1000000001));
  CHECK(BigInteger("1000000000") != BigInteger(1000000001));
  CHECK(BigInteger("-1000000000") != BigInteger(-1000000001));
  CHECK(BigInteger("-1000000000") != BigInteger(-1000000001));
  CHECK(BigInteger("-1000000000") != BigInteger(-1000000001));
  CHECK("-1" == BigIntegerToString(-BigInteger(1)));
  CHECK("-1" == BigIntegerToString(-BigInteger(1)));
  CHECK("-1" == BigIntegerToString(-BigInteger(1)));
  CHECK("1" == BigIntegerToString(-(-BigInteger(1))));
  CHECK("1" == BigIntegerToString(-(-BigInteger(1))));
  CHECK("1" == BigIntegerToString(-(-BigInteger(1))));
  CHECK("0" == BigIntegerToString(-BigInteger(0)));
  CHECK("0" == BigIntegerToString(-BigInteger(0)));
  CHECK("0" == BigIntegerToString(-BigInteger(0)));
  CHECK("-1000" == BigIntegerToString(-BigInteger(1000)));
  CHECK("-1000" == BigIntegerToString(-BigInteger(1000)));
  CHECK("-1000" == BigIntegerToString(-BigInteger(1000)));
  CHECK("1000" == BigIntegerToString(-BigInteger(-1000)));
  CHECK("1000" == BigIntegerToString(-BigInteger(-1000)));
  CHECK("1000" == BigIntegerToString(-BigInteger(-1000)));
  CHECK("-123456789123" == BigIntegerToString(-BigInteger(123456789123)));
  CHECK("-123456789123" == BigIntegerToString(-BigInteger(123456789123)));
  CHECK("-123456789123" == BigIntegerToString(-BigInteger(123456789123)));
  CHECK("123456789123" == BigIntegerToString(-BigInteger(-123456789123)));
  CHECK("123456789123" == BigIntegerToString(-BigInteger(-123456789123)));
  CHECK("123456789123" == BigIntegerToString(-BigInteger(-123456789123)));
  CHECK("3" == BigIntegerToString(BigInteger(2) + BigInteger(1)));
  CHECK("3" == BigIntegerToString(BigInteger(2) + BigInteger(1)));
  CHECK("3" == BigIntegerToString(BigInteger(2) + BigInteger(1)));
  CHECK("3" == BigIntegerToString(BigInteger(2) + BigInteger(1)));
  CHECK("3" == BigIntegerToString(BigInteger(2) + BigInteger(1)));
  CHECK("3" == BigIntegerToString(BigInteger(2) + BigInteger(1)));
  CHECK("1" == BigIntegerToString(BigInteger(2) + BigInteger(-1)));
  CHECK("1" == BigIntegerToString(BigInteger(2) + BigInteger(-1)));
  CHECK("1" == BigIntegerToString(BigInteger(2) + BigInteger(-1)));
  CHECK("1" == BigIntegerToString(BigInteger(2) + BigInteger(-1)));
  CHECK("1" == BigIntegerToString(BigInteger(2) + BigInteger(-1)));
  CHECK("1" == BigIntegerToString(BigInteger(2) + BigInteger(-1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-2) + BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-2) + BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-2) + BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-2) + BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-2) + BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-2) + BigInteger(1)));
  CHECK("-3" == BigIntegerToString(BigInteger(-2) + BigInteger(-1)));
  CHECK("-3" == BigIntegerToString(BigInteger(-2) + BigInteger(-1)));
  CHECK("-3" == BigIntegerToString(BigInteger(-2) + BigInteger(-1)));
  CHECK("-3" == BigIntegerToString(BigInteger(-2) + BigInteger(-1)));
  CHECK("-3" == BigIntegerToString(BigInteger(-2) + BigInteger(-1)));
  CHECK("-3" == BigIntegerToString(BigInteger(-2) + BigInteger(-1)));
  CHECK("123" == BigIntegerToString(BigInteger(123) + BigInteger(0)));
  CHECK("123" == BigIntegerToString(BigInteger(123) + BigInteger(0)));
  CHECK("123" == BigIntegerToString(BigInteger(123) + BigInteger(0)));
  CHECK("123" == BigIntegerToString(BigInteger(123) + BigInteger(0)));
  CHECK("123" == BigIntegerToString(BigInteger(123) + BigInteger(0)));
  CHECK("123" == BigIntegerToString(BigInteger(123) + BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(-123) + BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(-123) + BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(-123) + BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(-123) + BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(-123) + BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(-123) + BigInteger(0)));
  CHECK("1" == BigIntegerToString(BigInteger(2) - BigInteger(1)));
  CHECK("1" == BigIntegerToString(BigInteger(2) - BigInteger(1)));
  CHECK("1" == BigIntegerToString(BigInteger(2) - BigInteger(1)));
  CHECK("1" == BigIntegerToString(BigInteger(2) - BigInteger(1)));
  CHECK("1" == BigIntegerToString(BigInteger(2) - BigInteger(1)));
  CHECK("1" == BigIntegerToString(BigInteger(2) - BigInteger(1)));
  CHECK("3" == BigIntegerToString(BigInteger(2) - BigInteger(-1)));
  CHECK("3" == BigIntegerToString(BigInteger(2) - BigInteger(-1)));
  CHECK("3" == BigIntegerToString(BigInteger(2) - BigInteger(-1)));
  CHECK("3" == BigIntegerToString(BigInteger(2) - BigInteger(-1)));
  CHECK("3" == BigIntegerToString(BigInteger(2) - BigInteger(-1)));
  CHECK("3" == BigIntegerToString(BigInteger(2) - BigInteger(-1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-2) - BigInteger(-1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-2) - BigInteger(-1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-2) - BigInteger(-1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-2) - BigInteger(-1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-2) - BigInteger(-1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-2) - BigInteger(-1)));
  CHECK("-3" == BigIntegerToString(BigInteger(-2) - BigInteger(1)));
  CHECK("-3" == BigIntegerToString(BigInteger(-2) - BigInteger(1)));
  CHECK("-3" == BigIntegerToString(BigInteger(-2) - BigInteger(1)));
  CHECK("-3" == BigIntegerToString(BigInteger(-2) - BigInteger(1)));
  CHECK("-3" == BigIntegerToString(BigInteger(-2) - BigInteger(1)));
  CHECK("-3" == BigIntegerToString(BigInteger(-2) - BigInteger(1)));
  CHECK("123" == BigIntegerToString(BigInteger(123) - BigInteger(0)));
  CHECK("123" == BigIntegerToString(BigInteger(123) - BigInteger(0)));
  CHECK("123" == BigIntegerToString(BigInteger(123) - BigInteger(0)));
  CHECK("123" == BigIntegerToString(BigInteger(123) - BigInteger(0)));
  CHECK("123" == BigIntegerToString(BigInteger(123) - BigInteger(0)));
  CHECK("123" == BigIntegerToString(BigInteger(123) - BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(-123) - BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(-123) - BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(-123) - BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(-123) - BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(-123) - BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(-123) - BigInteger(0)));
  CHECK("-123" == BigIntegerToString(BigInteger(0) - BigInteger(123)));
  CHECK("-123" == BigIntegerToString(BigInteger(0) - BigInteger(123)));
  CHECK("-123" == BigIntegerToString(BigInteger(0) - BigInteger(123)));
  CHECK("-123" == BigIntegerToString(BigInteger(0) - BigInteger(123)));
  CHECK("-123" == BigIntegerToString(BigInteger(0) - BigInteger(123)));
  CHECK("-123" == BigIntegerToString(BigInteger(0) - BigInteger(123)));
  CHECK("123" == BigIntegerToString(BigInteger(0) - BigInteger(-123)));
  CHECK("123" == BigIntegerToString(BigInteger(0) - BigInteger(-123)));
  CHECK("123" == BigIntegerToString(BigInteger(0) - BigInteger(-123)));
  CHECK("123" == BigIntegerToString(BigInteger(0) - BigInteger(-123)));
  CHECK("123" == BigIntegerToString(BigInteger(0) - BigInteger(-123)));
  CHECK("123" == BigIntegerToString(BigInteger(0) - BigInteger(-123)));
  CHECK(BigInteger(0).sqrt() == Catch::Approx(0));
  CHECK(BigInteger(0).sqrt() == Catch::Approx(0));
  CHECK(BigInteger(0).sqrt() == Catch::Approx(0));
  CHECK(BigInteger(1).sqrt() == Catch::Approx(1));
  CHECK(BigInteger(1).sqrt() == Catch::Approx(1));
  CHECK(BigInteger(1).sqrt() == Catch::Approx(1));
  CHECK(BigInteger(2).sqrt() == Catch::Approx(1.4142135624));
  CHECK(BigInteger(2).sqrt() == Catch::Approx(1.4142135624));
  CHECK(BigInteger(2).sqrt() == Catch::Approx(1.4142135624));
  CHECK(BigInteger(3).sqrt() == Catch::Approx(1.7320508076));
  CHECK(BigInteger(3).sqrt() == Catch::Approx(1.7320508076));
  CHECK(BigInteger(3).sqrt() == Catch::Approx(1.7320508076));
  CHECK(BigInteger(4).sqrt() == Catch::Approx(2));
  CHECK(BigInteger(4).sqrt() == Catch::Approx(2));
  CHECK(BigInteger(4).sqrt() == Catch::Approx(2));
  CHECK(BigInteger(1000).sqrt() == Catch::Approx(31.6227766017));
  CHECK(BigInteger(1000).sqrt() == Catch::Approx(31.6227766017));
  CHECK(BigInteger(1000).sqrt() == Catch::Approx(31.6227766017));
  CHECK(BigInteger(10000099).sqrt() == Catch::Approx(3162.2933134041));
  CHECK(BigInteger(10000099).sqrt() == Catch::Approx(3162.2933134041));
  CHECK(BigInteger(10000099).sqrt() == Catch::Approx(3162.2933134041));
  CHECK(BigInteger(100000990099).sqrt() == Catch::Approx(316229.3314969375));
  CHECK(BigInteger(100000990099).sqrt() == Catch::Approx(316229.3314969375));
  CHECK(BigInteger(100000990099).sqrt() == Catch::Approx(316229.3314969375));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger(0)));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger(0)));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger(0)));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger(0)));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger(0)));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger(0)));
  CHECK("0" == BigIntegerToString(BigInteger(123) * BigInteger(0)));
  CHECK("0" == BigIntegerToString(BigInteger(123) * BigInteger(0)));
  CHECK("0" == BigIntegerToString(BigInteger(123) * BigInteger(0)));
  CHECK("0" == BigIntegerToString(BigInteger(123) * BigInteger(0)));
  CHECK("0" == BigIntegerToString(BigInteger(123) * BigInteger(0)));
  CHECK("0" == BigIntegerToString(BigInteger(123) * BigInteger(0)));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger(-123)));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger(-123)));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger(-123)));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger(-123)));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger(-123)));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger(-123)));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger("123456789123456789")));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger("123456789123456789")));
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger("123456789123456789")));
  CHECK("1" == BigIntegerToString(BigInteger(1) * BigInteger(1)));
  CHECK("1" == BigIntegerToString(BigInteger(1) * BigInteger(1)));
  CHECK("1" == BigIntegerToString(BigInteger(1) * BigInteger(1)));
  CHECK("1" == BigIntegerToString(BigInteger(1) * BigInteger(1)));
  CHECK("1" == BigIntegerToString(BigInteger(1) * BigInteger(1)));
  CHECK("1" == BigIntegerToString(BigInteger(1) * BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-1) * BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-1) * BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-1) * BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-1) * BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-1) * BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-1) * BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(1) * BigInteger(-1)));
  CHECK("-1" == BigIntegerToString(BigInteger(1) * BigInteger(-1)));
  CHECK("-1" == BigIntegerToString(BigInteger(1) * BigInteger(-1)));
  CHECK("-1" == BigIntegerToString(BigInteger(1) * BigInteger(-1)));
  CHECK("-1" == BigIntegerToString(BigInteger(1) * BigInteger(-1)));
  CHECK("-1" == BigIntegerToString(BigInteger(1) * BigInteger(-1)));
  CHECK("1" == BigIntegerToString(BigInteger(-1) * BigInteger(-1)));
  CHECK("1" == BigIntegerToString(BigInteger(-1) * BigInteger(-1)));
  CHECK("1" == BigIntegerToString(BigInteger(-1) * BigInteger(-1)));
  CHECK("1" == BigIntegerToString(BigInteger(-1) * BigInteger(-1)));
  CHECK("1" == BigIntegerToString(BigInteger(-1) * BigInteger(-1)));
  CHECK("1" == BigIntegerToString(BigInteger(-1) * BigInteger(-1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) / BigInteger(1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) / BigInteger(1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) / BigInteger(1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) / BigInteger(1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) / BigInteger(1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) / BigInteger(1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) % BigInteger(1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) % BigInteger(1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) % BigInteger(1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) % BigInteger(1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) % BigInteger(1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) % BigInteger(1)));
  CHECK("7" == BigIntegerToString(BigInteger(57) / BigInteger(8)));
  CHECK("7" == BigIntegerToString(BigInteger(57) / BigInteger(8)));
  CHECK("7" == BigIntegerToString(BigInteger(57) / BigInteger(8)));
  CHECK("7" == BigIntegerToString(BigInteger(57) / BigInteger(8)));
  CHECK("7" == BigIntegerToString(BigInteger(57) / BigInteger(8)));
  CHECK("7" == BigIntegerToString(BigInteger(57) / BigInteger(8)));
  CHECK("1" == BigIntegerToString(BigInteger(57) % BigInteger(8)));
  CHECK("1" == BigIntegerToString(BigInteger(57) % BigInteger(8)));
  CHECK("1" == BigIntegerToString(BigInteger(57) % BigInteger(8)));
  CHECK("1" == BigIntegerToString(BigInteger(57) % BigInteger(8)));
  CHECK("1" == BigIntegerToString(BigInteger(57) % BigInteger(8)));
  CHECK("1" == BigIntegerToString(BigInteger(57) % BigInteger(8)));
};

TEST_CASE("FromInt ❌FromInt64Min") {
  CHECK(BigIntegerToString(BigInteger(std::numeric_limits<int64_t>::min())) == BigIntegerToString(BigInteger("-9223372036854775808")));
  CHECK(BigInteger("0") == BigInteger(0));
}

TEST_CASE("❌FromString ; FromString - big one") {
  CHECK(BigInteger("123456789123456789123456789123456789123456789123456789123456789123456789123456789") ==
        BigInteger("123456789123456789123456789123456789123456789123456789123456789123456789123456789"));
  CHECK(BigInteger("-1234567891234567891234567891234567891234567891234567891234"
                   "56789123456789123456789") ==
        BigInteger("-1234567891234567891234567891234567891234567891234567891234"
                   "56789123456789123456789"));
}

TEST_CASE("BigIntegerBinaryAddition") {
  CHECK(BigInteger("12345678912345678912345678912345678912345678912345678912345"
                   "6789123456789123456789") +
            BigInteger("1234567891234567891234567891234567891234567891234567891"
                       "23456789123456789123456789") ==
        BigInteger("24691357824691357824691357824691357824691357824691357824691"
                   "3578246913578246913578"));
  CHECK(BigInteger("12345678912345678912345678912345678912345678912345678912345"
                   "6789123456789123456789") +
            BigInteger("1234567891234567891234567891234567891234567891234567891"
                       "23456789123456789123456789") ==
        BigInteger("12345678912345678912345678912345678912345678912345678912345"
                   "6789123456789123456789") * BigInteger("2")); }
 TEST_CASE("Big one") {
  BigInteger a("999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
  BigInteger a_m("-999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
  BigInteger b("9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
  BigInteger b_m("-9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
  BigInteger sum("10000000000999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999998");
  BigInteger mul("9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999989999999999000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");
  BigInteger sub("-9999999999000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
  BigInteger div2("10000000000");
  BigInteger div("0");
  CHECK(a + b == sum);
  CHECK(a - b == sub);
  CHECK(a * b == mul);
  CHECK(a / b == div);
  /* CHECK(b / a == div2); */
  CHECK(BigInteger("00000100") == BigInteger("100"));
  CHECK(BigInteger("-00000100") == BigInteger(-100));
  CHECK(BigIntegerToString(BigInteger("-00000100")) == BigIntegerToString(BigInteger(-100)));
  CHECK(BigInteger("-00000100") != BigInteger(-101));
}

TEST_CASE("DIVISION") {
  BigInteger a("12345678912345678916789123456789123456789");
  BigInteger b("1234567891233456786789123456789123456782");
  CHECK_THROWS(BigInteger("+1a"));
  
  /* CHECK( ((a * b) / b) == a); */
  /*  CHECK((a * b) / a == b); */
CHECK (BigInteger(0) + BigInteger(0) == BigInteger(0));
CHECK (BigInteger(0) + BigInteger(1) == BigInteger("1"));
}

TEST_CASE("csBIArithmeticsTests:  TenDivTwoEqualsFive") {
  REQUIRE(BigInteger(10) / BigInteger(2) == BigInteger(5));
}

TEST_CASE("csBIArithmeticsTests:  TenModTwoEqualsZero") {
  REQUIRE(BigInteger(10) % BigInteger(2) == BigInteger(0));
}

TEST_CASE("csBIArithmeticsTests:  TenDivThreeEqualsThree") {
  REQUIRE(BigInteger(10) / BigInteger(3) == BigInteger(3));
}

TEST_CASE("csBIArithmeticsTests:  TenModThreeEqualsOne") {
  REQUIRE(BigInteger(10) % BigInteger(3) == BigInteger(1));
}

TEST_CASE("csBIArithmeticsTests:  _255DivOneEquals255") {
  REQUIRE(BigInteger(255) / BigInteger(1) == BigInteger(255));
}

TEST_CASE("csBIArithmeticsTests:  _255DivMinusOneEqualsM255") {
  REQUIRE(BigInteger(255) / BigInteger(-1) == BigInteger(-255));
}

TEST_CASE("csBIArithmeticsTests:  Minus10IsMinus10") {
  REQUIRE(-BigInteger(10) == BigInteger(-10));
}

// SHL SHR


// ========= compare

TEST_CASE("csBIArithmeticsTests:  ZeroLessThanOne") {
  // cout << "will compare 0 and 1" << endl;
  BigInteger big0(0);
  BigInteger big1(1);
}

TEST_CASE("csBIArithmeticsTests:  OneGreaterThanZero") {
  REQUIRE(BigInteger(1) > BigInteger(0));
}

TEST_CASE("csBIArithmeticsTests:  NotZeroGreaterThanOne") {
  REQUIRE(!(BigInteger(0) > BigInteger(1)));
}

TEST_CASE("csBIArithmeticsTests:  ZeroLeqOne") {
  REQUIRE(BigInteger(0) <= BigInteger(1));
}

TEST_CASE("csBIArithmeticsTests:  NotZeroGeqOne") {
  REQUIRE(!(BigInteger(0) >= BigInteger(1)));
}


TEST_CASE("csBIArithmeticsTests:  SpecialDiv2") {
  BigInteger b1 = BigInteger("-11001000000");
  BigInteger b2 = BigInteger(86400);
  REQUIRE(b1 / b2 == BigInteger(-127326));
}


TEST_CASE("csBIArithmeticsTests:  SpecialMod") {
  BigInteger b1 = BigInteger("20195283520469175757");
  BigInteger b2 = BigInteger("1048576");
  REQUIRE(b1 % b2 == BigInteger("888269"));
}

TEST_CASE("csBIArithmeticsTests:  SpecialModNeg") {
  REQUIRE(BigInteger(860593) % BigInteger(-201) == BigInteger(112));
}

/* TEST_CASE("csBIArithmeticsTests:  SpecialModNeg2") { */
/*   REQUIRE(BigInteger("-18224909727634776050312394179610579601844989529623334093" */
/*                      "909233530432892596607") % */
/*               BigInteger("14954691977398614017") == */
/*           BigInteger("-3100049211437790421")); */
/* } */


/* TEST_CASE("csBIArithmeticsTests:  Minus5Divides2EqualsMinus2") { */
/*   REQUIRE(-5 / 2 == -2);  // c++ standard, same thing */
/*   REQUIRE(BigInteger(-5) / BigInteger(2) == BigInteger(-2)); */
/* } */

/* TEST_CASE("csBIArithmeticsTests:  FiveDividesMinus2EqualsMinus2") { */
/*   REQUIRE(5 / -2 == -2);  // c++ standard, same thing */
/*   REQUIRE(BigInteger(5) / BigInteger(-2) == BigInteger(-2)); */
/* } */

/* TEST_CASE("csBIArithmeticsTests:  FiveDivides2Equals2") { */
/*   REQUIRE(5 / 2 == 2);  // c++ standard, same thing */
/*   REQUIRE(BigInteger(5) / BigInteger(2) == BigInteger(2)); */
/* } */
/*  */
/* TEST_CASE("csBIArithmeticsTests:  MinusFiveDividesMinus2Equals2") { */
/*   REQUIRE(-5 / -2 == 2);  // c++ standard, same thing */
/*   REQUIRE(BigInteger(-5) / BigInteger(-2) == BigInteger(2)); */
/* } */

/* TEST_CASE("csBIArithmeticsTests:  Minus10Mod3EqualsMinus1") { */
/*   REQUIRE(-10 % 3 == -1);  // c++ standard, same thing */
/*   REQUIRE(BigInteger(-10) % BigInteger(3) == BigInteger(-1)); */
/* } */
/*  */
/* TEST_CASE("csBIArithmeticsTests:  TenMod3Equals1") { */
/*   REQUIRE(10 % 3 == 1);  // c++ standard, same thing */
/*   REQUIRE(BigInteger(10) % BigInteger(3) == BigInteger(1)); */
/* } */
/*  */
/* TEST_CASE("csBIArithmeticsTests:  TenModMinus3Equals1") { */
/*   REQUIRE(10 % -3 == 1);  // c++ standard, same thing */
/*   REQUIRE(BigInteger(10) % BigInteger(-3) == BigInteger(1)); */
/* } */
/*  */
/* TEST_CASE("csBIArithmeticsTests:  MinusTenModMinus3EqualsMinus1") { */
/*   REQUIRE(-10 % -3 == -1);  // c++ standard, same thing */
/*   REQUIRE(BigInteger(-10) % BigInteger(-3) == BigInteger(-1)); */
/* } */
/*  */
/* TEST_CASE("csBIArithmeticsTests:  TenModMinus5Equals0") { */
/*   REQUIRE(10 % -5 == 0);  // c++ standard, same thing */
/*   REQUIRE(BigInteger(10) % BigInteger(-5) == BigInteger(0)); */
/* } */
/* TEST_CASE("BIG_rational") { */
/*   BigRational a("12345678912345678916789123456789123456789", "1234567891233456786789123456789123456782"); */
/*   BigRational b("1234567891233456786789123456789123456782", "12345678912345678916789123456789123456789"); */
/*  */
/*   CHECK(a + b == BigRational("24691357824680246984691357824680246984671", "152415787532691752533578246913578246913578")); */
/* } */
