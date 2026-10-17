#include "test.h"

START_TEST(s21_test_memcpy) {
  const unsigned char input[] = {0, 255, 'a', 0, 128, 'b', 'c'};
  for (size_t n = 0; n <= sizeof(input); ++n) {
    unsigned char actual[20], expected[20];
    memset(actual, 0xA5, sizeof(actual));
    memset(expected, 0xA5, sizeof(expected));
    ck_assert_ptr_eq(s21_memcpy(actual + 2, input, n), actual + 2);
    memcpy(expected + 2, input, n);
    ck_assert_mem_eq(actual, expected, sizeof(actual));
  }
}
END_TEST

S21_SUITE(memcpy, s21_test_memcpy)
