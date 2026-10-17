#include "test.h"

START_TEST(s21_test_case_conversion) {
  const char* inputs[] = {"", "abcXYZ", "012 !?\t\n", "\x80\xff"};
  const char* upper[] = {"", "ABCXYZ", "012 !?\t\n", "\x80\xff"};
  const char* lower[] = {"", "abcxyz", "012 !?\t\n", "\x80\xff"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i) {
    char *a = s21_to_upper(inputs[i]), *b = s21_to_lower(inputs[i]);
    ck_assert_ptr_nonnull(a);
    ck_assert_ptr_nonnull(b);
    ck_assert_str_eq(a, upper[i]);
    ck_assert_str_eq(b, lower[i]);
    ck_assert_ptr_ne(a, inputs[i]);
    ck_assert_ptr_ne(b, inputs[i]);
    free(a);
    free(b);
  }
  ck_assert_ptr_null(s21_to_upper(NULL));
  ck_assert_ptr_null(s21_to_lower(NULL));
}
END_TEST
START_TEST(s21_test_insert) {
  const char* sources[] = {"", "abc", "hello world"};
  const char* insertions[] = {"", "X", "123"};
  for (size_t a = 0; a < 3; ++a)
    for (size_t b = 0; b < 3; ++b)
      for (size_t pos = 0; pos <= strlen(sources[a]); ++pos) {
        char expected[64] = {0};
        memcpy(expected, sources[a], pos);
        strcat(expected, insertions[b]);
        strcat(expected, sources[a] + pos);
        char* actual = s21_insert(sources[a], insertions[b], pos);
        ck_assert_ptr_nonnull(actual);
        ck_assert_str_eq(actual, expected);
        free(actual);
      }
  ck_assert_ptr_null(s21_insert("abc", "x", 4));
  ck_assert_ptr_null(s21_insert("abc", "x", ULLONG_MAX));
  ck_assert_ptr_null(s21_insert(NULL, "x", 0));
  ck_assert_ptr_null(s21_insert("abc", NULL, 0));
}
END_TEST
START_TEST(s21_test_trim) {
  const char* inputs[] = {"",     "abc", "  abc \t", "xyabcxy",
                          "xxxx", "abc", " a b "};
  const char* sets[] = {" ", "", " \t", "xy", "x", "xyz", " "};
  const char* expected[] = {"", "abc", "abc", "abc", "", "abc", "a b"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i) {
    char* actual = s21_trim(inputs[i], sets[i]);
    ck_assert_ptr_nonnull(actual);
    ck_assert_str_eq(actual, expected[i]);
    free(actual);
  }
  ck_assert_ptr_null(s21_trim(NULL, " "));
  ck_assert_ptr_null(s21_trim("abc", NULL));
}
END_TEST

S21_SUITE(extra, s21_test_case_conversion, s21_test_insert, s21_test_trim)
