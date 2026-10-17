#include "test.h"

START_TEST(s21_test_strerror) {
  for (int code = -20; code < 160; ++code)
    ck_assert_str_eq(s21_strerror(code), strerror(code));
  ck_assert_str_eq(s21_strerror(INT_MIN), strerror(INT_MIN));
  ck_assert_str_eq(s21_strerror(INT_MAX), strerror(INT_MAX));
}
END_TEST

S21_SUITE(strerror, s21_test_strerror)
