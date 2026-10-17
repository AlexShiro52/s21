#include "test.h"

#define S21_SCAN_INTEGER(type, format, input)                               \
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

START_TEST(s21_test_sscanf_signed) {
  const char* inputs[] = {"",        " ",          "abc",        "+",   "-",
                          "0",       "-0",         "42",         "-42", "+42x",
                          " \t123 ", "2147483647", "-2147483648"};
  const char* formats[] = {"%d%n", "%1d%n", "%3d%n", "%12d%n"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
      S21_SCAN_INTEGER(int, formats[f], inputs[i]);
  S21_SCAN_INTEGER(short, "%hd%n", "-32768");
  S21_SCAN_INTEGER(short, "%hd%n", "32767");
  char input[64];
  sprintf(input, "%ld", LONG_MIN);
  S21_SCAN_INTEGER(long, "%ld%n", input);
  sprintf(input, "%ld", LONG_MAX);
  S21_SCAN_INTEGER(long, "%ld%n", input);
}
END_TEST

START_TEST(s21_test_sscanf_unsigned) {
  const char* inputs[] = {"",   " ",   "xyz",  "+",       "0",         "-0",
                          "42", "-42", "+42x", " \t123 ", "4294967295"};
  const char* formats[] = {"%u%n", "%1u%n", "%3u%n", "%12u%n"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
      S21_SCAN_INTEGER(unsigned, formats[f], inputs[i]);
  S21_SCAN_INTEGER(unsigned short, "%hu%n", "65535");
  char input[64];
  sprintf(input, "%lu", ULONG_MAX);
  S21_SCAN_INTEGER(unsigned long, "%lu%n", input);
}
END_TEST

START_TEST(s21_test_sscanf_auto_base) {
  const char* inputs[] = {"",      " ",   "xyz",  "+",     "-",    "0",
                          "7",     "077", "-077", "+077",  "0x1f", "+0x1F",
                          "-0X1f", "0x",  "0xz",  "123abc"};
  const char* formats[] = {"%i%n", "%1i%n", "%2i%n", "%3i%n", "%5i%n"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
      S21_SCAN_INTEGER(int, formats[f], inputs[i]);
  const char* short_inputs[] = {"0x7fff", "+0x7fff", "-0x8000",
                                "077777", "+077777", "-0100000"};
  for (size_t i = 0; i < 6; ++i)
    S21_SCAN_INTEGER(short, "%hi%n", short_inputs[i]);
  const char* int_inputs[] = {"0x7fffffff", "+0x7fffffff", "-0x80000000",
                              "+017777777777", "-020000000000"};
  for (size_t i = 0; i < 5; ++i) S21_SCAN_INTEGER(int, "%i%n", int_inputs[i]);
  char positive[64], negative[64];
  sprintf(positive, "+0x%lx", LONG_MAX);
  sprintf(negative, "-0x%lx", (unsigned long)LONG_MAX + 1UL);
  S21_SCAN_INTEGER(long, "%li%n", positive);
  S21_SCAN_INTEGER(long, "%li%n", negative);
  S21_SCAN_INTEGER(long, "%li%n", "0x123");
  S21_SCAN_INTEGER(long, "%li%n", "+0");
  S21_SCAN_INTEGER(long, "%li%n", "-0x10");
}
END_TEST

#define S21_SCAN_FLOAT(type, epsilon, format, input)                          \
  do {                                                                        \
    type actual = 99, expected = 99;                                          \
    int actual_n = -1, expected_n = -1;                                       \
    int actual_result = s21_sscanf(input, format, &actual, &actual_n);        \
    int expected_result = sscanf(input, format, &expected, &expected_n);      \
    ck_assert_msg(actual_result == expected_result && actual_n == expected_n, \
                  "sscanf(%s, %s): return %d/%d, n %d/%d", input, format,     \
                  actual_result, expected_result, actual_n, expected_n);      \
    if (isnan(expected)) {                                                    \
      ck_assert(isnan(actual));                                               \
    } else {                                                                  \
      ck_assert_msg(                                                          \
          actual == expected || (isfinite(actual) && isfinite(expected) &&    \
                                 fabsl((long double)actual - expected) <=     \
                                     4 * (epsilon) * fabsl(expected)),        \
          "sscanf(%s, %s): value %.21Lg/%.21Lg", input, format,               \
          (long double)actual, (long double)expected);                        \
      if (expected == 0)                                                      \
        ck_assert_int_eq(!!signbit(actual), !!signbit(expected));             \
    }                                                                         \
  } while (0)

START_TEST(s21_test_sscanf_literals) {
  const char* inputs[] = {"",    "x",    "x42",   "x 42",
                          "%42", " %42", "x\t42", "x42end"};
  const char* formats[] = {"x%d%n", "x %d%n", "%%%d%n", "x%dend%n", " %d%n"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
      S21_SCAN_INTEGER(int, formats[f], inputs[i]);
  int actual = -1, expected = -1;
  ck_assert_int_eq(s21_sscanf(" \t", " \t%n", &actual),
                   sscanf(" \t", " \t%n", &expected));
  ck_assert_int_eq(actual, expected);
}
END_TEST

START_TEST(s21_test_sscanf_mixed) {
  const char* input = " -42 077 0x1f 1.25 word Z %";
  int da = 0, db = 0, ia = 0, ib = 0, na = 0, nb = 0;
  unsigned xa = 0, xb = 0;
  double fa = 0, fb = 0;
  char sa[16] = {0}, sb[16] = {0}, ca = 0, cb = 0;
  const char* format = "%d %i %x %lf %s %c %%%n";
  int a = s21_sscanf(input, format, &da, &ia, &xa, &fa, sa, &ca, &na);
  int b = sscanf(input, format, &db, &ib, &xb, &fb, sb, &cb, &nb);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(da, db);
  ck_assert_int_eq(ia, ib);
  ck_assert_uint_eq(xa, xb);
  ck_assert_double_eq(fa, fb);
  ck_assert_str_eq(sa, sb);
  ck_assert_int_eq(ca, cb);
  ck_assert_int_eq(na, nb);
}
END_TEST

START_TEST(s21_test_sscanf_octal) {
  const char* inputs[] = {"",  " \t", "xyz", "+",    "-", "0",
                          "7", "077", "-17", "+17x", "8", " \t7654 rest"};
  const char* formats[] = {"%o%n", "%1o%n", "%3o%n", "%12o%n"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
      S21_SCAN_INTEGER(unsigned, formats[f], inputs[i]);
  S21_SCAN_INTEGER(unsigned short, "%ho%n", "177777");
  char input[64];
  sprintf(input, "%lo", ULONG_MAX);
  S21_SCAN_INTEGER(unsigned long, "%lo%n", input);
}
END_TEST

START_TEST(s21_test_sscanf_hex) {
  const char* inputs[] = {"",   " \t",  "xyz",  "+",     "-",     "0",
                          "aF", "BEEF", "0x1f", "-0X1f", "+0x1F", " \tF0 rest"};
  const char* formats[] = {"%x%n", "%X%n", "%1x%n", "%5x%n", "%12x%n"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
      S21_SCAN_INTEGER(unsigned, formats[f], inputs[i]);
  S21_SCAN_INTEGER(unsigned short, "%hx%n", "ffff");
  char input[64];
  sprintf(input, "%lx", ULONG_MAX);
  S21_SCAN_INTEGER(unsigned long, "%lx%n", input);
}
END_TEST

START_TEST(s21_test_sscanf_chars) {
  const char* inputs[] = {"", "A", " ", "ABC", "A B", "\tXYZ"};
  const char* formats[] = {"%c%n", "%1c%n", " %c%n"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f) {
      char actual[8], expected[8];
      memset(actual, '#', sizeof(actual));
      memset(expected, '#', sizeof(expected));
      int na = -1, nb = -1;
      int a = s21_sscanf(inputs[i], formats[f], actual, &na);
      int b = sscanf(inputs[i], formats[f], expected, &nb);
      ck_assert_int_eq(a, b);
      ck_assert_mem_eq(actual, expected, sizeof(actual));
      ck_assert_int_eq(na, nb);
    }
  char actual[8] = "#######", expected[8] = "#######";
  int na = -1, nb = -1;
  ck_assert_int_eq(s21_sscanf("A B!", "%3c%n", actual, &na),
                   sscanf("A B!", "%3c%n", expected, &nb));
  ck_assert_mem_eq(actual, expected, sizeof(actual));
  ck_assert_int_eq(na, nb);
}
END_TEST

START_TEST(s21_test_sscanf_partial_chars) {
  const char* inputs[] = {"", "a", "ab", "abc", "abcd"};
  const char* formats[] = {"%3c%n", "%*3c%n"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f) {
      char actual[8] = "#######", expected[8] = "#######";
      int na = -1, nb = -1;
      int a = f == 0 ? s21_sscanf(inputs[i], formats[f], actual, &na)
                     : s21_sscanf(inputs[i], formats[f], &na);
      int b = f == 0 ? sscanf(inputs[i], formats[f], expected, &nb)
                     : sscanf(inputs[i], formats[f], &nb);
      ck_assert_msg(a == b && na == nb, "sscanf(%s, %s): return %d/%d, n %d/%d",
                    inputs[i], formats[f], a, b, na, nb);
      ck_assert_mem_eq(actual, expected, sizeof(actual));
    }
}
END_TEST

START_TEST(s21_test_sscanf_strings) {
  const char* inputs[] = {"",      " \t\n", "a", "hello world", " \tword\nnext",
                          "abcdef"};
  const char* formats[] = {"%s%n", "%1s%n", "%3s%n", "%15s%n"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f) {
      char actual[32], expected[32];
      memset(actual, '#', sizeof(actual));
      memset(expected, '#', sizeof(expected));
      int na = -1, nb = -1;
      int a = s21_sscanf(inputs[i], formats[f], actual, &na);
      int b = sscanf(inputs[i], formats[f], expected, &nb);
      ck_assert_int_eq(a, b);
      ck_assert_mem_eq(actual, expected, sizeof(actual));
      ck_assert_int_eq(na, nb);
    }
}
END_TEST

START_TEST(s21_test_sscanf_decimal_float) {
  const char* inputs[] = {"",        " \t",         "xyz",  "+",     "-",
                          ".",       "0",           "-0",   "+1.25", "-.5",
                          "-42.125", " \t12.5tail", "1.2.3"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i) {
    S21_SCAN_FLOAT(float, FLT_EPSILON, "%f%n", inputs[i]);
    S21_SCAN_FLOAT(double, DBL_EPSILON, "%lf%n", inputs[i]);
    S21_SCAN_FLOAT(long double, LDBL_EPSILON, "%Lf%n", inputs[i]);
  }
  S21_SCAN_FLOAT(float, FLT_EPSILON, "%3f%n", "1.25");
  S21_SCAN_FLOAT(double, DBL_EPSILON, "%4lf%n", "-12.5");
}
END_TEST

START_TEST(s21_test_sscanf_exponents) {
  const char* inputs[] = {"0e0",      "-0e+0", "1.25e2", "-1.25E+2",
                          "+1.25e-2", ".5E3",  "12e2e3", "1e3.5"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i) {
    S21_SCAN_FLOAT(float, FLT_EPSILON, "%e%n", inputs[i]);
    S21_SCAN_FLOAT(double, DBL_EPSILON, "%lE%n", inputs[i]);
    S21_SCAN_FLOAT(long double, LDBL_EPSILON, "%Lg%n", inputs[i]);
    S21_SCAN_FLOAT(float, FLT_EPSILON, "%G%n", inputs[i]);
  }
  S21_SCAN_FLOAT(double, DBL_EPSILON, "%4lf%n", "1e+2tail");
}
END_TEST

START_TEST(s21_test_sscanf_incomplete_exponent) {
  S21_SCAN_FLOAT(double, DBL_EPSILON, "%lf%n", "1e+");
}
END_TEST

START_TEST(s21_test_sscanf_special_float) {
  const char* inputs[] = {"inf", "-INFINITY", "nan", "NAN(payload)"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    S21_SCAN_FLOAT(double, DBL_EPSILON, "%lf%n", inputs[i]);
}
END_TEST

START_TEST(s21_test_sscanf_hex_float) {
  S21_SCAN_FLOAT(double, DBL_EPSILON, "%lf%n", "0x1.8p+2");
}
END_TEST

START_TEST(s21_test_sscanf_pointer) {
  const char* inputs[] = {"",      " \t",   "xyz",        "+",    "-",
                          "0",     "1f",    "aF",         "BEEF", "0x123",
                          "-0X1f", "+0x1F", " \tABC rest"};
  const char* formats[] = {"%p%n", "%1p%n", "%5p%n", "%12p%n"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f) {
      void* actual = NULL;
      void* expected = NULL;
      int na = -1, nb = -1;
      int a = s21_sscanf(inputs[i], formats[f], &actual, &na);
      int b = sscanf(inputs[i], formats[f], &expected, &nb);
      ck_assert_msg(a == b && actual == expected && na == nb,
                    "sscanf(%s, %s): return %d/%d, pointer %p/%p, n %d/%d",
                    inputs[i], formats[f], a, b, actual, expected, na, nb);
    }
}
END_TEST

START_TEST(s21_test_sscanf_suppression) {
  const char* inputs[] = {
      "A 42",   "ABC 42",  "word 42", "abcdef 42", "17 42", "0x1f 42", "12 42",
      "-42 42", "1.25 42", "1e2 42",  "0x123 42",  "",      "xyz",     "17"};
  const char* formats[] = {"%*c%d%n", "%*3c%d%n", "%*s%d%n", "%*3s%*s%d%n",
                           "%*o%d%n", "%*x%d%n",  "%*u%d%n", "%*d%d%n",
                           "%*f%d%n", "%*e%d%n",  "%*p%d%n", "%*o%d%n",
                           "%*x%d%n", "%*o%d%n"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    S21_SCAN_INTEGER(int, formats[i], inputs[i]);
}
END_TEST

START_TEST(s21_test_sscanf_count) {
  int actual_value = 0, expected_value = 0, actual_n = -1, expected_n = -1;
  short actual_hn = -1, expected_hn = -1;
  long actual_ln = -1, expected_ln = -1;
  const char* format = "%n \t%hn%d%ln";
  int a = s21_sscanf(" \t42tail", format, &actual_n, &actual_hn, &actual_value,
                     &actual_ln);
  int b = sscanf(" \t42tail", format, &expected_n, &expected_hn,
                 &expected_value, &expected_ln);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(actual_value, expected_value);
  ck_assert_int_eq(actual_n, expected_n);
  ck_assert_int_eq(actual_hn, expected_hn);
  ck_assert_int_eq(actual_ln, expected_ln);
}
END_TEST

S21_SUITE(sscanf, s21_test_sscanf_signed, s21_test_sscanf_unsigned,
          s21_test_sscanf_auto_base, s21_test_sscanf_literals,
          s21_test_sscanf_mixed, s21_test_sscanf_octal, s21_test_sscanf_hex,
          s21_test_sscanf_chars, s21_test_sscanf_partial_chars,
          s21_test_sscanf_strings, s21_test_sscanf_decimal_float,
          s21_test_sscanf_exponents, s21_test_sscanf_incomplete_exponent,
          s21_test_sscanf_special_float, s21_test_sscanf_hex_float,
          s21_test_sscanf_pointer, s21_test_sscanf_suppression,
          s21_test_sscanf_count)
