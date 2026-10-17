#include "test.h"

START_TEST(s21_test_strncmp) {
  const char* inputs[] = {"",    "a",    "abc",  "abcd",
                          "abd", "\x80", "\x7f", "\xff"};
  for (size_t a = 0; a < sizeof(inputs) / sizeof(inputs[0]); ++a)
    for (size_t b = 0; b < sizeof(inputs) / sizeof(inputs[0]); ++b)
      for (size_t n = 0; n < 8; ++n) {
        int actual = s21_strncmp(inputs[a], inputs[b], n);
        int expected = strncmp(inputs[a], inputs[b], n);
        ck_assert_int_eq((actual > 0) - (actual < 0),
                         (expected > 0) - (expected < 0));
      }
}
END_TEST

S21_SUITE(strncmp, s21_test_strncmp)
