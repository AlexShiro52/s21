#include "test.h"

START_TEST(s21_test_rounding_even_ties) {
  const double values[] = {0.5, 2.5, 4.5, -0.5, -2.5, -4.5};
  for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
    S21_ASSERT_SPRINTF("%.0f", values[i]);
  S21_ASSERT_SPRINTF("%.1f", 1.25);
  S21_ASSERT_SPRINTF("%.2f", 0.125);
}
END_TEST

START_TEST(s21_test_rounding_odd_ties) {
  const double values[] = {1.5, 3.5, 7.5, -1.5, -3.5, -7.5};
  for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
    S21_ASSERT_SPRINTF("%.0f", values[i]);
  S21_ASSERT_SPRINTF("%.1f", 1.75);
  S21_ASSERT_SPRINTF("%.2f", 0.375);
}
END_TEST

START_TEST(s21_test_rounding_neighbours) {
  S21_ASSERT_SPRINTF("%.0f", nextafter(2.5, 0.0));
  S21_ASSERT_SPRINTF("%.0f", nextafter(2.5, 3.0));
  S21_ASSERT_SPRINTF("%.0f", nextafter(1.5, 0.0));
  S21_ASSERT_SPRINTF("%.0f", nextafter(1.5, 2.0));
  S21_ASSERT_SPRINTF("%.1f", nextafter(1.25, 0.0));
  S21_ASSERT_SPRINTF("%.1f", nextafter(1.25, 2.0));
}
END_TEST

START_TEST(s21_test_rounding_carry) {
  S21_ASSERT_SPRINTF("%.0f", 9.5);
  S21_ASSERT_SPRINTF("%.0f", 99.5);
  S21_ASSERT_SPRINTF("%.0f", -99.5);
  S21_ASSERT_SPRINTF("%.2f", 9.999);
  S21_ASSERT_SPRINTF("%.2f", 99.999);
  S21_ASSERT_SPRINTF("%.0e", 9.5);
  S21_ASSERT_SPRINTF("%.1g", 99.5);
}
END_TEST

START_TEST(s21_test_rounding_outside_digits) {
  S21_ASSERT_SPRINTF("%.20f", 1.0);
  S21_ASSERT_SPRINTF("%.0f", 0.0);
  S21_ASSERT_SPRINTF("%.15f", DBL_MIN);
  S21_ASSERT_SPRINTF("%.15f", DBL_TRUE_MIN);
  S21_ASSERT_SPRINTF("%.15f", -DBL_TRUE_MIN);
  S21_ASSERT_SPRINTF("%+11.14f", 1.841002947786282e-298);
}
END_TEST

S21_SUITE(rounding, s21_test_rounding_even_ties, s21_test_rounding_odd_ties,
          s21_test_rounding_neighbours, s21_test_rounding_carry,
          s21_test_rounding_outside_digits)
