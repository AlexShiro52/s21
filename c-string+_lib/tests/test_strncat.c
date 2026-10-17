#include "test.h"

START_TEST(s21_test_strncat) {
  const char* inputs[] = {"", "a", "hello", "abc\0def"};
  for (size_t a = 0; a < sizeof(inputs) / sizeof(inputs[0]); ++a)
    for (size_t b = 0; b < sizeof(inputs) / sizeof(inputs[0]); ++b)
      for (size_t n = 0; n < 9; ++n) {
        char actual[32] = {0}, expected[32] = {0};
        strcpy(actual, inputs[a]);
        strcpy(expected, inputs[a]);
        ck_assert_ptr_eq(s21_strncat(actual, inputs[b], n), actual);
        strncat(expected, inputs[b], n);
        ck_assert_mem_eq(actual, expected, sizeof(actual));
      }
}
END_TEST
START_TEST(s21_test_strncat_bounded_source) {
  char* source = malloc(3);
  ck_assert_ptr_nonnull(source);
  memcpy(source, "abc", 3);
  char actual[8] = "x", expected[8] = "x";
  s21_strncat(actual, source, 3);
  strncat(expected, source, 3);
  ck_assert_mem_eq(actual, expected, sizeof(actual));
  free(source);
}
END_TEST

S21_SUITE(strncat, s21_test_strncat, s21_test_strncat_bounded_source)
