#include "test.h"

#define S21_ASSERT_SCAN_I(type, format, input)                              \
  do {                                                                      \
    type actual = 99, expected = 99;                                        \
    int actual_n = -1, expected_n = -1;                                     \
    int actual_result = s21_sscanf(input, format, &actual, &actual_n);      \
    int expected_result = sscanf(input, format, &expected, &expected_n);    \
    ck_assert_msg(actual_result == expected_result && actual == expected && \
                      actual_n == expected_n,                               \
                  "sscanf(%s, %s): return %d/%d, value %Lg/%Lg, n %d/%d",   \
                  input, format, actual_result, expected_result,            \
                  (long double)actual, (long double)expected, actual_n,     \
                  expected_n);                                              \
  } while (0)

START_TEST(s21_test_scanf_i_decimal_and_failures) {
  const char* inputs[] = {"",    " \t\n", "xyz",         "+", "-",  "42",
                          "+42", "-42",   " \v\r\f 42x", "0", "+0", "-0"};
  const char* formats[] = {"%i%n", "%1i%n", "%3i%n", "%8i%n"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
      S21_ASSERT_SCAN_I(int, formats[f], inputs[i]);
}
END_TEST

START_TEST(s21_test_scanf_i_signed_hexadecimal) {
  S21_ASSERT_SCAN_I(short, "%hi%n", "+0x7fff");
  S21_ASSERT_SCAN_I(short, "%hi%n", "-0x7fff");
  S21_ASSERT_SCAN_I(short, "%hi%n", "-0x8000");
  S21_ASSERT_SCAN_I(short, "%hi%n", "+0x0");
  S21_ASSERT_SCAN_I(int, "%i%n", "+0x7fffffff");
  S21_ASSERT_SCAN_I(int, "%i%n", "-0x7fffffff");
  S21_ASSERT_SCAN_I(int, "%i%n", "-0x80000000");
  S21_ASSERT_SCAN_I(int, "%i%n", "-0X2A");
  char input[64];
  snprintf(input, sizeof(input), "+0x%lx", (unsigned long)LONG_MAX);
  S21_ASSERT_SCAN_I(long, "%li%n", input);
  snprintf(input, sizeof(input), "-0x%lx", (unsigned long)LONG_MAX);
  S21_ASSERT_SCAN_I(long, "%li%n", input);
  snprintf(input, sizeof(input), "-0x%lx", 0UL - (unsigned long)LONG_MIN);
  S21_ASSERT_SCAN_I(long, "%li%n", input);
}
END_TEST

START_TEST(s21_test_scanf_i_signed_octal) {
  S21_ASSERT_SCAN_I(short, "%hi%n", "+077777");
  S21_ASSERT_SCAN_I(short, "%hi%n", "-077777");
  S21_ASSERT_SCAN_I(short, "%hi%n", "-0100000");
  S21_ASSERT_SCAN_I(int, "%i%n", "+017777777777");
  S21_ASSERT_SCAN_I(int, "%i%n", "-017777777777");
  S21_ASSERT_SCAN_I(int, "%i%n", "-020000000000");
  char input[64];
  snprintf(input, sizeof(input), "+0%lo", (unsigned long)LONG_MAX);
  S21_ASSERT_SCAN_I(long, "%li%n", input);
  snprintf(input, sizeof(input), "-0%lo", (unsigned long)LONG_MAX);
  S21_ASSERT_SCAN_I(long, "%li%n", input);
  snprintf(input, sizeof(input), "-0%lo", 0UL - (unsigned long)LONG_MIN);
  S21_ASSERT_SCAN_I(long, "%li%n", input);
}
END_TEST

START_TEST(s21_test_scanf_i_without_sign) {
  S21_ASSERT_SCAN_I(short, "%hi%n", "0x7fff");
  S21_ASSERT_SCAN_I(short, "%hi%n", "077777");
  S21_ASSERT_SCAN_I(int, "%i%n", "0x7fffffff");
  S21_ASSERT_SCAN_I(int, "%i%n", "017777777777");
  char input[64];
  snprintf(input, sizeof(input), "0x%lx", (unsigned long)LONG_MAX);
  S21_ASSERT_SCAN_I(long, "%li%n", input);
  snprintf(input, sizeof(input), "0%lo", (unsigned long)LONG_MAX);
  S21_ASSERT_SCAN_I(long, "%li%n", input);
}
END_TEST

START_TEST(s21_test_scanf_i_suppressed) {
  const char* inputs[] = {"0x2a 7", "+0x2a 7", "-0x2a 7", "077 7", "+077 7",
                          "-077 7", "42 7",    "+42 7",   "-42 7"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    S21_ASSERT_SCAN_I(int, "%*i %d%n", inputs[i]);
  S21_ASSERT_SCAN_I(int, "%*4i %d%n", "-077 7");
  S21_ASSERT_SCAN_I(int, "%*5i %d%n", "+0x2a 7");
}
END_TEST

START_TEST(s21_test_scanf_i_prefix_width) {
  const char* inputs[] = {"0x1f", "+0x1F", "-0X1f", "0x", "0xz", "+0Xg"};
  const char* formats[] = {"%i%n", "%1i%n", "%2i%n", "%3i%n", "%4i%n"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
      S21_ASSERT_SCAN_I(int, formats[f], inputs[i]);
}
END_TEST

S21_SUITE(scanf_i, s21_test_scanf_i_decimal_and_failures,
          s21_test_scanf_i_signed_hexadecimal, s21_test_scanf_i_signed_octal,
          s21_test_scanf_i_without_sign, s21_test_scanf_i_suppressed,
          s21_test_scanf_i_prefix_width)
