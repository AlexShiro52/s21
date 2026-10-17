#include "test.h"

START_TEST(s21_test_strstr) {
  const char* inputs[] = {"", "a", "aaaab", "abcabc", "hello world", "needle"};
  const char* needles[] = {"",       "a",     "ab",     "abc",
                           "aaaaab", "world", "needle", "needles"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t j = 0; j < sizeof(needles) / sizeof(needles[0]); ++j)
      ck_assert_ptr_eq(s21_strstr(inputs[i], needles[j]),
                       strstr(inputs[i], needles[j]));
}
END_TEST

S21_SUITE(strstr, s21_test_strstr)
