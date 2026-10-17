#include "test.h"

START_TEST(s21_test_memcmp) {
  const unsigned char inputs[][8] = {
      {0}, {1, 0, 2}, {1, 0, 3}, {255, 128}, {'a', 'b'}};
  for (size_t a = 0; a < 5; ++a)
    for (size_t b = 0; b < 5; ++b)
      for (size_t n = 0; n <= 8; ++n) {
        int actual = s21_memcmp(inputs[a], inputs[b], n);
        int expected = memcmp(inputs[a], inputs[b], n);
        ck_assert_int_eq((actual > 0) - (actual < 0),
                         (expected > 0) - (expected < 0));
      }
}
END_TEST

S21_SUITE(memcmp, s21_test_memcmp)
