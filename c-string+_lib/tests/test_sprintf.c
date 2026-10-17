#include "test.h"

START_TEST(s21_test_sprintf_signed) {
  const char* formats[] = {"%d",     "%+8d", "%-8d", "% 8.5d", "%08d",
                           "%08.5d", "%.0d", "%+d",  "% d"};
  const int values[] = {0, 1, -1, 42, -42, INT_MIN, INT_MAX};
  for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
    for (size_t n = 0; n < sizeof(values) / sizeof(values[0]); ++n)
      S21_ASSERT_SPRINTF(formats[f], values[n]);
  S21_ASSERT_SPRINTF("%hd %hd", SHRT_MIN, SHRT_MAX);
  S21_ASSERT_SPRINTF("%ld %ld", LONG_MIN, LONG_MAX);
  S21_ASSERT_SPRINTF("%hhd %lld", (int)SCHAR_MIN, LLONG_MIN);
}
END_TEST

START_TEST(s21_test_sprintf_unsigned) {
  const char* formats[] = {"%u", "%12u", "%-12u", "%012u", "%12.6u", "%.0u"};
  const unsigned values[] = {0, 1, 42, UINT_MAX};
  for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
    for (size_t n = 0; n < sizeof(values) / sizeof(values[0]); ++n)
      S21_ASSERT_SPRINTF(formats[f], values[n]);
  S21_ASSERT_SPRINTF("%hu %lu %hhu %llu", (unsigned int)USHRT_MAX, ULONG_MAX,
                     (unsigned int)UCHAR_MAX, ULLONG_MAX);
}
END_TEST

START_TEST(s21_test_sprintf_bases) {
  const char* formats[] = {"%o",    "%x",    "%X",    "%#o",
                           "%#x",   "%#X",   "%#08x", "%#08o",
                           "%#.5o", "%#.0o", "%#.0x", "%-#12.6X"};
  const unsigned values[] = {0, 1, 7, 8, 15, 16, 255, UINT_MAX};
  for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
    for (size_t n = 0; n < sizeof(values) / sizeof(values[0]); ++n)
      S21_ASSERT_SPRINTF(formats[f], values[n]);
  S21_ASSERT_SPRINTF("%ho %lx %llX", (unsigned int)USHRT_MAX, ULONG_MAX,
                     ULLONG_MAX);
}
END_TEST

START_TEST(s21_test_sprintf_text) {
  const char* formats[] = {"%s",   "%12s",   "%-12s",  "%.0s",
                           "%.3s", "%12.3s", "%-12.3s"};
  const char* strings[] = {"", "abc", "hello world"};
  for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
    for (size_t s = 0; s < sizeof(strings) / sizeof(strings[0]); ++s)
      S21_ASSERT_SPRINTF(formats[f], strings[s]);
  S21_ASSERT_SPRINTF("%c|%4c|%-4c|%c", 'A', 'B', 'C', 0);
  S21_ASSERT_SPRINTF("literal %% text");
  char empty[1];
  ck_assert_int_eq(s21_sprintf(empty, ""), 0);
  ck_assert_int_eq(empty[0], 0);
}
END_TEST

START_TEST(s21_test_sprintf_bounded_text) {
  char* source = malloc(3);
  ck_assert_ptr_nonnull(source);
  memcpy(source, "abc", 3);
  char actual[64], expected[64];
  int length = s21_sprintf(actual, "%.3s|%8.2s|%.0s", source, source, source);
  int reference = sprintf(expected, "%.3s|%8.2s|%.0s", "abc", "abc", "abc");
  ck_assert_int_eq(length, reference);
  ck_assert_mem_eq(actual, expected, (size_t)reference + 1);
  free(source);
}
END_TEST

START_TEST(s21_test_sprintf_float) {
  const char* formats[] = {"%f",      "%.0f",    "%.1f",    "%.6f",    "%.15f",
                           "%+14.3f", "% 14.3f", "%014.3f", "%-14.3f", "%#.0f"};
  const double values[] = {0.0,  -0.0,  1.0,     -1.0,        0.5,
                           1.5,  2.5,   1.25,    9.9999,      0.00001,
                           1e20, 1e-20, DBL_MIN, DBL_TRUE_MIN};
  for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
    for (size_t n = 0; n < sizeof(values) / sizeof(values[0]); ++n)
      S21_ASSERT_SPRINTF(formats[f], values[n]);
}
END_TEST

START_TEST(s21_test_sprintf_dynamic_fields) {
  const int widths[] = {0, 12, -12};
  const int precisions[] = {-1, 0, 3, 8};
  for (size_t w = 0; w < 3; ++w)
    for (size_t p = 0; p < 4; ++p) {
      S21_ASSERT_SPRINTF("%*.*f", widths[w], precisions[p], 1.25);
      S21_ASSERT_SPRINTF("%*.*g", widths[w], precisions[p], 1.25);
      S21_ASSERT_SPRINTF("%*.*d", widths[w], precisions[p], -42);
      S21_ASSERT_SPRINTF("%*.*s", widths[w], precisions[p], "hello");
    }
}
END_TEST

START_TEST(s21_test_sprintf_mixed) {
  S21_ASSERT_SPRINTF("[%+06d] %s %.3f %#x %% %lu", -42, "hello", 1.25, 255u,
                     ULONG_MAX);
}
END_TEST

START_TEST(s21_test_sprintf_scientific) {
  const char* formats[] = {"%e",      "%E",      "%.0e",    "%#.0E",  "%.12e",
                           "%+18.4e", "% 18.4E", "%018.4e", "%-18.4E"};
  const double values[] = {0.0, -0.0, 1.25, -42.5, 9.9999, 1e-20, 1e20};
  for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
    for (size_t n = 0; n < sizeof(values) / sizeof(values[0]); ++n)
      S21_ASSERT_SPRINTF(formats[f], values[n]);
}
END_TEST

START_TEST(s21_test_sprintf_general) {
  const char* formats[] = {"%g",      "%G",      "%.0g",    "%.1G",
                           "%.8g",    "%#.8G",   "%+18.4g", "% 18.4G",
                           "%018.4g", "%-18.4G", "%#.0g"};
  const double values[] = {0.0,  -0.0, 1.25, -42.5, 9.9999, 1e-5,
                           1e-4, 1e-3, 1e5,  1e6,   1e20};
  for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
    for (size_t n = 0; n < sizeof(values) / sizeof(values[0]); ++n)
      S21_ASSERT_SPRINTF(formats[f], values[n]);
  S21_ASSERT_SPRINTF("%#.3000g", 1.25);
  S21_ASSERT_SPRINTF("%#.3000G", 1e-20);
  S21_ASSERT_SPRINTF("%.3000g", 1.25);
  S21_ASSERT_SPRINTF("%#.3000g", 0.0001);
  S21_ASSERT_SPRINTF("%#.3000G", DBL_TRUE_MIN);
}
END_TEST

START_TEST(s21_test_sprintf_long_double) {
  const char* formats[] = {"%Lf", "%Le", "%LE", "%Lg", "%LG", "%+14.3Lf"};
  const long double values[] = {0.0L, -0.0L, 1.25L, -42.5L};
  for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
    for (size_t n = 0; n < sizeof(values) / sizeof(values[0]); ++n)
      S21_ASSERT_SPRINTF(formats[f], values[n]);
}
END_TEST

START_TEST(s21_test_sprintf_long_double_precision) {
  S21_ASSERT_SPRINTF("%.*Lf", LDBL_DECIMAL_DIG - 1, 1.0L + LDBL_EPSILON);
}
END_TEST

START_TEST(s21_test_sprintf_infinity) {
  const char* formats[] = {"%f",   "%e",    "%E",    "%g",   "%G",
                           "%12f", "%+12e", "% 12G", "%-12f"};
  const double values[] = {INFINITY, -INFINITY};
  for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
    for (size_t n = 0; n < sizeof(values) / sizeof(values[0]); ++n)
      S21_ASSERT_SPRINTF(formats[f], values[n]);
}
END_TEST

START_TEST(s21_test_sprintf_nan) {
  const char* formats[] = {"%f",   "%e",    "%E",    "%g",   "%G",
                           "%12f", "%-12G", "%+12e", "% 12f"};
  for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
    S21_ASSERT_SPRINTF(formats[f], (double)NAN);
  S21_ASSERT_SPRINTF("%f", -(double)NAN);
}
END_TEST

START_TEST(s21_test_sprintf_infinity_zero_padding) {
  const char* formats[] = {"%012f",  "%+012e", "% 012E",
                           "%-012g", "%012G",  "%012.0f"};
  const double values[] = {INFINITY, -INFINITY};
  for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
    for (size_t n = 0; n < sizeof(values) / sizeof(values[0]); ++n)
      S21_ASSERT_SPRINTF(formats[f], values[n]);
}
END_TEST

START_TEST(s21_test_sprintf_pointer) {
  int object = 42;
  const char* formats[] = {"%p", "%24p", "%-24p"};
  for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
    S21_ASSERT_SPRINTF(formats[f], (void*)&object);
}
END_TEST

START_TEST(s21_test_sprintf_null_pointer) {
  const char* formats[] = {"%p", "[%p]", "%24p", "%-24p"};
  for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
    S21_ASSERT_SPRINTF(formats[f], (void*)NULL);
}
END_TEST

START_TEST(s21_test_sprintf_embedded_null) {
  S21_ASSERT_SPRINTF("A%cB%cC", 0, 0);
}
END_TEST

START_TEST(s21_test_sprintf_subnormal_scientific) {
  const char* formats[] = {"%.0e",    "%.6e", "%.17e", "%.6E",   "%24.6e",
                           "%-24.6E", "%.6g", "%.17G", "%24.6g", "%.324f"};
  const double values[] = {DBL_TRUE_MIN, 2 * DBL_TRUE_MIN,        -DBL_TRUE_MIN,
                           DBL_MIN / 2,  nextafter(DBL_MIN, 0.0), DBL_MIN};
  for (size_t f = 0; f < sizeof(formats) / sizeof(formats[0]); ++f)
    for (size_t n = 0; n < sizeof(values) / sizeof(values[0]); ++n)
      S21_ASSERT_SPRINTF(formats[f], values[n]);
}
END_TEST

S21_SUITE(sprintf, s21_test_sprintf_signed, s21_test_sprintf_unsigned,
          s21_test_sprintf_bases, s21_test_sprintf_text,
          s21_test_sprintf_bounded_text, s21_test_sprintf_float,
          s21_test_sprintf_dynamic_fields, s21_test_sprintf_mixed,
          s21_test_sprintf_scientific, s21_test_sprintf_general,
          s21_test_sprintf_long_double, s21_test_sprintf_long_double_precision,
          s21_test_sprintf_infinity, s21_test_sprintf_nan,
          s21_test_sprintf_infinity_zero_padding, s21_test_sprintf_pointer,
          s21_test_sprintf_null_pointer, s21_test_sprintf_embedded_null,
          s21_test_sprintf_subnormal_scientific)
