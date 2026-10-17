#ifndef S21_TESTS_TEST_H
#define S21_TESTS_TEST_H

#include <check.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_string.h"

Suite* s21_memchr_suite(void);
Suite* s21_memcmp_suite(void);
Suite* s21_memcpy_suite(void);
Suite* s21_memset_suite(void);
Suite* s21_strlen_suite(void);
Suite* s21_strncat_suite(void);
Suite* s21_strchr_suite(void);
Suite* s21_strncmp_suite(void);
Suite* s21_strncpy_suite(void);
Suite* s21_strcspn_suite(void);
Suite* s21_strerror_suite(void);
Suite* s21_strpbrk_suite(void);
Suite* s21_strrchr_suite(void);
Suite* s21_strstr_suite(void);
Suite* s21_strtok_suite(void);
Suite* s21_extra_suite(void);
Suite* s21_sprintf_suite(void);
Suite* s21_sscanf_suite(void);
Suite* s21_rounding_suite(void);
Suite* s21_scanf_i_suite(void);

#define S21_ASSERT_SPRINTF(...)                                      \
  do {                                                               \
    char actual[16384] = {0}, expected[16384] = {0};                 \
    int actual_length = s21_sprintf(actual, __VA_ARGS__);            \
    int expected_length = sprintf(expected, __VA_ARGS__);            \
    ck_assert_msg(actual_length == expected_length,                  \
                  "sprintf length: actual %d, expected %d (%s)",     \
                  actual_length, expected_length, #__VA_ARGS__);     \
    ck_assert_mem_eq(actual, expected, (size_t)expected_length + 1); \
  } while (0)

#define S21_SUITE(name, ...)                                      \
  Suite* s21_##name##_suite(void) {                               \
    Suite* suite = suite_create(#name);                           \
    TCase* cases = tcase_create("comparisons");                   \
    const TTest* tests[] = {__VA_ARGS__};                         \
    for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i) \
      tcase_add_test(cases, tests[i]);                            \
    suite_add_tcase(suite, cases);                                \
    return suite;                                                 \
  }

#endif
