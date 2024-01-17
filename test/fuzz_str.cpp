#include <assert.h>

#include <cstring>
#include <string>

#include "../src/lib.cpp"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size < 2) return 0;

  size_t mid = Size / 2;
  std::string str1(Data, Data + mid);
  std::string str2(Data + mid, Data + Size);
  try {
    BigInteger a(str1), b(str2);
    assert(a + a == a * 2);
    assert(a + b == b + a);
    assert(a * b == b * a);
    assert((a * b) == ((b - 1) * a) + a);
    assert(a * b == b * a);
    assert(BigIntegerToString(a * b) == BigIntegerToString(b * a));
  } catch (std::exception error) {
    // Ignore invalid input
  }

  return 0;
}
