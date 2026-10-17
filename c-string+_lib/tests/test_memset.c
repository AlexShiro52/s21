#include "test.h"

START_TEST(s21_test_memset) {
  const int chars[] = {0, 'x', -1, 256, 0x123, INT_MAX, INT_MIN};
  for (size_t c = 0; c < sizeof(chars) / sizeof(chars[0]); ++c)
    for (size_t n = 0; n <= 12; ++n) {
      unsigned char actual[16] = {0}, expected[16] = {0};
      ck_assert_ptr_eq(s21_memset(actual + 1, chars[c], n), actual + 1);
      memset(expected + 1, chars[c], n);
      ck_assert_mem_eq(actual, expected, sizeof(actual));
    }
}
END_TEST

S21_SUITE(memset, s21_test_memset)
