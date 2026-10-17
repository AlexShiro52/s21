#include "test.h"

START_TEST(s21_test_strtok) {
  const char* inputs[] = {"", "abc", ",,a;;b,c,", ",;;", " a b\tc ", "abc,def"};
  const char* delimiters[] = {"", ",;", " \t"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t j = 0; j < sizeof(delimiters) / sizeof(delimiters[0]); ++j) {
      char actual[64], expected[64];
      strcpy(actual, inputs[i]);
      strcpy(expected, inputs[i]);
      char* a = s21_strtok(actual, delimiters[j]);
      char* b = strtok(expected, delimiters[j]);
      while (a || b) {
        ck_assert_ptr_nonnull(a);
        ck_assert_ptr_nonnull(b);
        ck_assert_str_eq(a, b);
        a = s21_strtok(NULL, delimiters[j]);
        b = strtok(NULL, delimiters[j]);
      }
      ck_assert_ptr_null(s21_strtok(NULL, delimiters[j]));
    }
}
END_TEST
START_TEST(s21_test_strtok_changed_delimiter) {
  char actual[] = "a,b;c:d", expected[] = "a,b;c:d";
  ck_assert_str_eq(s21_strtok(actual, ","), strtok(expected, ","));
  ck_assert_str_eq(s21_strtok(NULL, ";"), strtok(NULL, ";"));
  ck_assert_str_eq(s21_strtok(NULL, ":"), strtok(NULL, ":"));
  ck_assert_str_eq(s21_strtok(NULL, ""), strtok(NULL, ""));
  ck_assert_ptr_null(s21_strtok(NULL, ""));
}
END_TEST

S21_SUITE(strtok, s21_test_strtok, s21_test_strtok_changed_delimiter)
