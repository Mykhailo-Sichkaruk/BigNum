#include <assert.h>

#include <cstring>  // Add this line at the beginning of your fuzz.cpp file
#include <string>

#include "../src/lib.cpp"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size < sizeof(int64_t) * 2) return 0;  // Ensure we have enough data

  int64_t part1, part2;
  memcpy(&part1, Data, sizeof(int64_t));
  memcpy(&part2, Data + sizeof(int64_t), sizeof(int64_t));

  BigInteger a(part1);
  BigInteger b(part2);

  // Perform the operation you want to test
  assert(a + a == a * 2);
  assert(a + b == b + a);
  assert(a * b == b * a);
  assert((a * b) == ((b - 1) * a) + a);
  assert(a * b == b * a);
  assert(BigIntegerToString(a * b) == BigIntegerToString(b * a));

  return 0;
}
