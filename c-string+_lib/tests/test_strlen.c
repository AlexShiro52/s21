#include "test.h"

START_TEST(s21_test_strlen) {
  const char* inputs[] = {"",          "a",           "Hello, world!",
                          "a\0hidden", " \t\n\r\v\f", "0123456789"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    ck_assert_uint_eq(s21_strlen(inputs[i]), strlen(inputs[i]));
  char long_string[4096];
  memset(long_string, 'x', sizeof(long_string) - 1);
  long_string[sizeof(long_string) - 1] = '\0';
  ck_assert_uint_eq(s21_strlen(long_string), strlen(long_string));
}
END_TEST

S21_SUITE(strlen, s21_test_strlen)
