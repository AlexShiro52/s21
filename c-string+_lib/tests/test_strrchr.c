#include "test.h"

START_TEST(s21_test_strrchr) {
  const char* inputs[] = {"",       "a",       "abacaba", "hello world",
                          "\t\n\r", "\x80\xff"};
  const int chars[] = {0, 'a', 'z', ' ', '\t', 128, 255, 256 + 'a', -1};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t c = 0; c < sizeof(chars) / sizeof(chars[0]); ++c)
      ck_assert_ptr_eq(s21_strrchr(inputs[i], chars[c]),
                       strrchr(inputs[i], chars[c]));
}
END_TEST

S21_SUITE(strrchr, s21_test_strrchr)
