#include "test.h"

START_TEST(s21_test_strncpy) {
  const char* inputs[] = {"", "a", "hello", "abc\0def"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t n = 0; n <= 12; ++n) {
      char actual[20], expected[20];
      memset(actual, '!', sizeof(actual));
      memset(expected, '!', sizeof(expected));
      ck_assert_ptr_eq(s21_strncpy(actual, inputs[i], n), actual);
      strncpy(expected, inputs[i], n);
      ck_assert_mem_eq(actual, expected, sizeof(actual));
    }
}
END_TEST
START_TEST(s21_test_strncpy_bounded_source) {
  char* source = malloc(3);
  ck_assert_ptr_nonnull(source);
  memcpy(source, "abc", 3);
  char actual[5] = "!!!!", expected[5] = "!!!!";
  s21_strncpy(actual, source, 3);
  strncpy(expected, source, 3);
  ck_assert_mem_eq(actual, expected, sizeof(actual));
  free(source);
}
END_TEST

S21_SUITE(strncpy, s21_test_strncpy, s21_test_strncpy_bounded_source)
