#include <algorithm>
#include <cmath>
#include <cstdint>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../src/lib.cpp"

// NOTE: Implement `BigIntegerToString`. Test requires you to return string representation of BigInteger in base 10 format.

TEST_CASE("Test division by two function") {
  BigInteger a("111");
  CHECK(a / BigInteger(2) == BigInteger("55"));
}
TEST_CASE("Check multiply by two function") {
  BigInteger a("111");
  CHECK(a * BigInteger(2) == BigInteger("222"));
  BigInteger largeNum = BigInteger("100000000000000000000000000000000000000");  // 10^38
  CHECK(largeNum / BigInteger(2) == BigInteger("50000000000000000000000000000000000000"));
}

TEST_CASE("ISQRT") {
  BigInteger a("100");
  CHECK(a.isqrt() == BigInteger("10"));
  BigInteger largeNum = BigInteger("100000000000000000000000000000000000000");  // 10^38
  CHECK(largeNum.isqrt() == BigInteger("10000000000000000000"));                // 10^19
  CHECK_THROWS(BigInteger(-1).isqrt());
  BigInteger largeNum2 = BigInteger("100000000000000000000000000000000000001");  // 10^38
  CHECK(largeNum2.isqrt() == BigInteger("10000000000000000000"));                // 10^19
  CHECK(BigInteger(0).isqrt() == BigInteger(0));
  CHECK(BigInteger(1).isqrt() == BigInteger(1));
  BigInteger veryLargeNum = BigInteger("123456789012345678901234567890123456789");  // 39 digits
  BigInteger result("11111111061111110993");
  CHECK(BigIntegerToString(veryLargeNum.isqrt()) == BigIntegerToString(result));  // 19 digits
}

TEST_CASE("FACTORIAL") {
  BigInteger factorial(1);
  for (int i = 1; i <= 100; i++) {
    factorial *= i;
  }
  BigInteger hundretFactorial(
      "93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976"
      "156518286253697920827223758251185210916864000000000000000000000000");
  CHECK(factorial == hundretFactorial);
}

TEST_CASE("BigInteger Addition") {
  // Large positive + Large positive
  CHECK(BigInteger("12345678912345678912345678912345") +
            BigInteger("98765432198765432198765432198765") == BigInteger("111111111111111111111111111111110"));

  // Large negative + Large negative
  CHECK(BigInteger("-12345678912345678912345678912345") +
            BigInteger("-98765432198765432198765432198765") ==
        BigInteger("-111111111111111111111111111111110"));

  // Large positive + Large negative
  CHECK(BigInteger("12345678912345678912345678912345") +
            BigInteger("-98765432198765432198765432198765") ==
        BigInteger("-86419753286419753286419753286420"));
}
TEST_CASE("BigInteger Subtraction") {
  // Large positive - Large positive
  CHECK(BigInteger("98765432198765432198765432198765") -
            BigInteger("12345678912345678912345678912345") ==
        BigInteger("86419753286419753286419753286420"));

  // Large negative - Large negative
  CHECK(BigInteger("-98765432198765432198765432198765") -
            BigInteger("-12345678912345678912345678912345") ==
        BigInteger("-86419753286419753286419753286420"));

  // Large positive - Large negative
  CHECK(BigInteger("98765432198765432198765432198765") -
            BigInteger("-12345678912345678912345678912345") ==
        BigInteger("111111111111111111111111111111110"));
}
TEST_CASE("BigInteger Multiplication") {
  // Large positive * Large positive
  CHECK(BigInteger("12345678912345678912345678912345") *
            BigInteger("98765432198765432198765432198765") ==
        BigInteger("1219326313565005315910684318256290931743611929397932927052253925"));

  // Large negative * Large negative
  CHECK(BigInteger("-12345678912345678912345678912345") *
            BigInteger("-98765432198765432198765432198765") ==
        BigInteger("1219326313565005315910684318256290931743611929397932927052253925"));

  // Large positive * Large negative
  CHECK(BigInteger("12345678912345678912345678912345") *
            BigInteger("-98765432198765432198765432198765") ==
        BigInteger("-1219326313565005315910684318256290931743611929397932927052253925"));
}
TEST_CASE("BigInteger Division") {
  // Large positive / Large positive
  CHECK(BigInteger("98765432198765432198765432198765") /
            BigInteger("12345678912345678912345678912345") ==
        BigInteger("8"));

  // Large negative / Large negative
  CHECK(BigInteger("-98765432198765432198765432198765") /
            BigInteger("-12345678912345678912345678912345") ==
        BigInteger("8"));

  // Large positive / Large negative
  CHECK(BigInteger("98765432198765432198765432198765") /
            BigInteger("-12345678912345678912345678912345") ==
        BigInteger("-8"));
}
TEST_CASE("Really big one") {
  // Create string with 10000 9's
  std::string big(10000, '9');
  std::string big_minus_1(10000, '9');
  big_minus_1[big_minus_1.size() - 1] = '8';
  BigInteger a(big);
  CHECK(a == BigInteger(big));
  CHECK(a - 1 == BigInteger(big_minus_1));
  CHECK(a * a == (a * (a - 1)) + a);
}
TEST_CASE("Big one") {
  REQUIRE((BigInteger(
               "-18224909727634776050312394179610579601844989529623334093909233530432892596607") %
           BigInteger("14954691977398614017")) == BigInteger("-3100049211437790421"));

  BigInteger small(
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999");
  BigInteger big(
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "9999999999999999999999999999999999999999999999999999999999999999");
  BigInteger sum(
      "100000000009999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "99999999999999999999999999999999999999999999999999999999999999998");
  BigInteger mul(
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999999999999999999999999"
      "999999999999999999999999999999999999999999999999999998999999999900000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "0000000000000000000000000000000000000000000001");
  BigInteger sub(
      "-99999999990000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "00000000000000000000000000000000000000000000000000000000000000000");
  BigInteger div("0");
  BigInteger div2("10000000000");
  BigInteger mod2("9999999999");
  CHECK(small + big == sum);
  CHECK(big + small == sum);
  CHECK(small - big == sub);
  CHECK(big - small == -sub);
  CHECK(small * big == mul);
  CHECK(big * small == mul);

  CHECK(small / big == div);
  CHECK(big / small == div2);
  CHECK(big % small == mod2);
  CHECK(BigInteger("00000100") == BigInteger("100"));
  CHECK(BigInteger("-00000100") == BigInteger(-100));
  CHECK(BigIntegerToString(BigInteger("-00000100")) == BigIntegerToString(BigInteger(-100)));
  CHECK(BigInteger("-00000100") != BigInteger(-101));
}

TEST_CASE("BigInteger Square Root") {
  CHECK("0" == BigIntegerToString(BigInteger(0)));
  CHECK("1" == BigIntegerToString(BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-1)));
  CHECK("123456789" == BigIntegerToString(BigInteger(123456789)));
  CHECK("-123456789" == BigIntegerToString(BigInteger(-123456789)));
  CHECK("2147483647" == BigIntegerToString(BigInteger(std::numeric_limits<int32_t>::max())));
  CHECK("-2147483648" == BigIntegerToString(BigInteger(std::numeric_limits<int32_t>::min())));
  CHECK("-2147483648" == BigIntegerToString(BigInteger(std::numeric_limits<int32_t>::min())));
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
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger("123456789123456789")));
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
  CHECK("-2147483648" == BigIntegerToString(BigInteger(std::numeric_limits<int32_t>::min())));
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
  CHECK("-3" == BigIntegerToString(BigInteger(-2) - BigInteger(1)));
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
  CHECK("0" == BigIntegerToString(BigInteger(0) * BigInteger("123456789123456789")));
  CHECK("1" == BigIntegerToString(BigInteger(1) * BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(-1) * BigInteger(1)));
  CHECK("-1" == BigIntegerToString(BigInteger(1) * BigInteger(-1)));
  CHECK("1" == BigIntegerToString(BigInteger(-1) * BigInteger(-1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) / BigInteger(1)));
  CHECK("0" == BigIntegerToString(BigInteger(0) % BigInteger(1)));
  CHECK("7" == BigIntegerToString(BigInteger(57) / BigInteger(8)));
  CHECK("1" == BigIntegerToString(BigInteger(57) % BigInteger(8)));
};

TEST_CASE("FromInt ❌FromInt64Min") {
  CHECK(BigIntegerToString(BigInteger(std::numeric_limits<int64_t>::min())) ==
        BigIntegerToString(BigInteger("-9223372036854775808")));
  CHECK(BigInteger("0") == BigInteger(0));
}

TEST_CASE("❌FromString ; FromString - big one") {
  CHECK(BigInteger(
            "123456789123456789123456789123456789123456789123456789123456789123456789123456789") ==
        BigInteger(
            "123456789123456789123456789123456789123456789123456789123456789123456789123456789"));
  CHECK(BigInteger(
            "-123456789123456789123456789123456789123456789123456789123456789123456789123456789") ==
        BigInteger(
            "-123456789123456789123456789123456789123456789123456789123456789123456789123456789"));
}

TEST_CASE("BigIntegerBinaryAddition") {
  CHECK(BigInteger(
            "123456789123456789123456789123456789123456789123456789123456789123456789123456789") +
            BigInteger("123456789123456789123456789123456789123456789123456789123456789123456789123"
                       "456789") ==
        BigInteger(
            "246913578246913578246913578246913578246913578246913578246913578246913578246913578"));
  CHECK(BigInteger(
            "123456789123456789123456789123456789123456789123456789123456789123456789123456789") +
            BigInteger("123456789123456789123456789123456789123456789123456789123456789123456789123"
                       "456789") ==
        BigInteger(
            "123456789123456789123456789123456789123456789123456789123456789123456789123456789") *
            BigInteger("2"));
}

TEST_CASE("DIVISION") {
  BigInteger a("12345678912345678916789123456789123456789");
  BigInteger b("1234567891233456786789123456789123456782");
  CHECK_THROWS(BigInteger("+1a"));

  CHECK(((a * b) / b) == a);
  CHECK((a * b) / a == b);
  CHECK(BigInteger(0) + BigInteger(0) == BigInteger(0));
  CHECK(BigInteger(0) + BigInteger(1) == BigInteger("1"));
}

TEST_CASE("csBIArithmeticsTests:  TenDivTwoEqualsFive") {
  REQUIRE(BigInteger(10) / BigInteger(2) == BigInteger(5));
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

TEST_CASE("csBIArithmeticsTests:  OneGreaterThanZero") { REQUIRE(BigInteger(1) > BigInteger(0)); }

TEST_CASE("csBIArithmeticsTests:  NotZeroGreaterThanOne") {
  REQUIRE(!(BigInteger(0) > BigInteger(1)));
}

TEST_CASE("csBIArithmeticsTests:  ZeroLeqOne") { REQUIRE(BigInteger(0) <= BigInteger(1)); }

TEST_CASE("csBIArithmeticsTests:  NotZeroGeqOne") { REQUIRE(!(BigInteger(0) >= BigInteger(1))); }

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

TEST_CASE("csBIArithmeticsTests:  SpecialModNeg2") {
  REQUIRE(
      BigInteger("-18224909727634776050312394179610579601844989529623334093909233530432892596607") /
          BigInteger("14954691977398614017") ==
      BigInteger("-1218675032235937780712688130619504358062280950643422399658"));

  REQUIRE(BigInteger(
              "-18224909727634776050312394179610579601844989529623334093906133481221454806186") ==
          BigInteger("14954691977398614017") *
              BigInteger("-1218675032235937780712688130619504358062280950643422399658"));

  REQUIRE(
      (BigInteger(
           "-18224909727634776050312394179610579601844989529623334093909233530432892596607") /
       BigInteger("14954691977398614017")) *
          BigInteger("14954691977398614017") ==
      BigInteger("-18224909727634776050312394179610579601844989529623334093909233530432892596607") -
          BigInteger("-3100049211437790421"));
}

TEST_CASE("csBIArithmeticsTests:  Minus5Divides2EqualsMinus2") {
  REQUIRE(-5 / 2 == -2);  // c++ standard, same thing
  REQUIRE(BigInteger(-5) / BigInteger(2) == BigInteger(-2));
}

TEST_CASE("csBIArithmeticsTests:  FiveDividesMinus2EqualsMinus2") {
  REQUIRE(5 / -2 == -2);  // c++ standard, same thing
  REQUIRE(BigInteger(5) / BigInteger(-2) == BigInteger(-2));
}

TEST_CASE("csBIArithmeticsTests:  FiveDivides2Equals2") {
  REQUIRE(5 / 2 == 2);  // c++ standard, same thing
  REQUIRE(BigInteger(5) / BigInteger(2) == BigInteger(2));
}

TEST_CASE("csBIArithmeticsTests:  MinusFiveDividesMinus2Equals2") {
  REQUIRE(-5 / -2 == 2);  // c++ standard, same thing
  REQUIRE(BigInteger(-5) / BigInteger(-2) == BigInteger(2));
}

TEST_CASE("csBIArithmeticsTests:  Minus10Mod3EqualsMinus1") {
  REQUIRE(-10 % 3 == -1);  // c++ standard, same thing
  REQUIRE(BigInteger(-10) % BigInteger(3) == BigInteger(-1));
}

TEST_CASE("csBIArithmeticsTests:  TenMod3Equals1") {
  REQUIRE(10 % 3 == 1);  // c++ standard, same thing
  REQUIRE(BigInteger(10) % BigInteger(3) == BigInteger(1));
}

TEST_CASE("csBIArithmeticsTests:  TenModMinus3Equals1") {
  REQUIRE(10 % -3 == 1);  // c++ standard, same thing
  REQUIRE(BigInteger(10) % BigInteger(-3) == BigInteger(1));
}

TEST_CASE("csBIArithmeticsTests:  MinusTenModMinus3EqualsMinus1") {
  REQUIRE(-10 % -3 == -1);  // c++ standard, same thing
  REQUIRE(BigInteger(-10) % BigInteger(-3) == BigInteger(-1));
}

TEST_CASE("csBIArithmeticsTests:  TenModMinus5Equals0") {
  REQUIRE(10 % -5 == 0);  // c++ standard, same thing
  REQUIRE(BigInteger(10) % BigInteger(-5) == BigInteger(0));
}
TEST_CASE("csBIArithmeticsTests:  TenModTwoEqualsZero") {
  REQUIRE(BigInteger(10) % BigInteger(2) == BigInteger(0));
}

TEST_CASE("BigInteger Division2") {
  CHECK(BigInteger("123456789123456789123456789") / BigInteger("123456789") ==
        BigInteger("1000000001000000001"));
  CHECK(BigInteger("1000000000000") / BigInteger("1000") == BigInteger("1000000000"));
  CHECK(BigInteger("1000") / BigInteger("1000000000000") == BigInteger("0"));
  CHECK(BigInteger("-123456789123456789123456789") / BigInteger("123456789") ==
        BigInteger("-1000000001000000001"));
  CHECK_THROWS(BigInteger("123456789") / BigInteger("0"));
}

TEST_CASE("BigRational Basic Addition") {
  BigRational a(1, 2);         // Represents 1/2
  BigRational b(1, 3);         // Represents 1/3
  BigRational expected(5, 6);  // Expected result is 5/6
  CHECK(a + b == expected);
}

TEST_CASE("BigRational Basic Subtraction") {
  BigRational a(3, 4);         // Represents 3/4
  BigRational b(1, 4);         // Represents 1/4
  BigRational expected(1, 2);  // Expected result is
  CHECK(a - b == expected);
}
TEST_CASE("BigRational Multiplication with Negative Numbers") {
  BigRational a(-3, 7);  // Represents -3/7
  BigRational b(2, 5);   // Represents 2/5
  BigRational expected(-6, 35);  // Expected result is -6/35
  CHECK(a * b == expected);
}

TEST_CASE("BigRational Division and Simplification") {
  BigRational a(4, 6);  // Represents 4/6
  BigRational b(2, 3);  // Represents 2/3
  BigRational expected(1, 1);  // Expected result is 1/1 or 1
  CHECK(a / b == expected);
}

TEST_CASE("BigRational Equality Check") {
  BigRational a(8, 12);  // Represents 8/12
  BigRational b(2, 3);   // Represents 2/3
  CHECK(a == b);  // They are equal when simplified
}

TEST_CASE("BigRational Inequality Check") {
  BigRational a(1, 3);  // Represents 1/3
  BigRational b(1, 2);  // Represents 1/2
  CHECK(a < b);  // 1/3 is less than 1/2
}
TEST_CASE("BigRational Zero Denominator Check") {
  CHECK_THROWS(BigRational(1, 0));  // Should throw an exception
}
TEST_CASE("BigRational Negative Denominator Check") {
  BigRational a(-1, -2);  // Represents -1/-2
  BigRational expected(1, 2);  // Expected result is 1/2
  CHECK(a == expected);
}
TEST_CASE("BigRational Large Number Addition") {
  BigRational a("123456789123456789", "987654321987654321");
  BigRational b("987654321987654321", "123456789123456789");
  BigRational expected("12230896747409402", "1505341124847349");  // Calculated result
  CHECK(a + b == expected);
}
TEST_CASE("BigRational Mixed Operations") {
  BigRational a("1", "2"), b("1", "3"), c("1", "4");
  CHECK(a + b - c == BigRational("7", "12"));
}
TEST_CASE("BigRational Division by Zero") {
  BigRational a("1", "2");
  CHECK_THROWS(a / BigRational("0", "1"));
}

TEST_CASE("BigRational Zero Comparison Equal") {
  CHECK(BigRational("0", "1") == BigRational("0", "2"));
}

TEST_CASE("BigRational Zero Comparison Not Equal") {
  CHECK(BigRational("0", "1") != BigRational("1", "2"));
}
TEST_CASE("BigRational One Comparison Equal") {
  CHECK(BigRational("1", "1") == BigRational("2", "2"));
}

TEST_CASE("BigRational One Comparison Not Equal") {
  CHECK(BigRational("1", "1") != BigRational("1", "2"));
}
TEST_CASE("BigRational Negative One Comparison Equal") {
  CHECK(BigRational("-1", "1") == BigRational("2", "-2"));
}

TEST_CASE("BigRational Negative One Comparison Not Equal") {
  CHECK(BigRational("-1", "1") != BigRational("1", "1"));
}
TEST_CASE("BigRational Fraction Comparison Equal") {
  CHECK(BigRational("1", "2") == BigRational("2", "4"));
}

TEST_CASE("BigRational Fraction Comparison Not Equal") {
  CHECK(BigRational("1", "2") != BigRational("3", "4"));
}
TEST_CASE("BigRational Large Number Comparison Equal") {
  CHECK(BigRational("123456789", "123456789") == BigRational("246913578", "246913578"));
}

TEST_CASE("BigRational Mixed Sign Comparison Equal") {
  CHECK(BigRational("-1", "2") == BigRational("1", "-2"));
}

TEST_CASE("BigRational Mixed Sign Comparison Not Equal") {
  CHECK(BigRational("-1", "2") != BigRational("1", "2"));
}
TEST_CASE("BigRational Zero Numerator Comparison Equal") {
  CHECK(BigRational("0", "123456789") == BigRational("0", "987654321"));
}

TEST_CASE("BigRational Zero Numerator Comparison Not Equal") {
  CHECK(BigRational("0", "123456789") != BigRational("1", "123456789"));
}
TEST_CASE("BigRational One Denominator Comparison Equal") {
  CHECK(BigRational("123456789", "1") == BigRational("123456789", "1"));
}

TEST_CASE("BigRational One Denominator Comparison Not Equal") {
  CHECK(BigRational("123456789", "1") != BigRational("987654321", "1"));
}
TEST_CASE("BigRational Negative Denominator Comparison Equal") {
  CHECK(BigRational("1", "-1") == BigRational("-1", "1"));
}

TEST_CASE("BigRational Negative Denominator Comparison Not Equal") {
  CHECK(BigRational("1", "-1") != BigRational("1", "1"));
}
TEST_CASE("BigRational Complex Fraction Comparison Equal") {
  CHECK(BigRational("1", "3") == BigRational("2", "6"));
}

TEST_CASE("BigRational Complex Fraction Comparison Not Equal") {
  CHECK(BigRational("1", "3") != BigRational("1", "6"));
}

