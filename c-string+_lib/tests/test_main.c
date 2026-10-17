#include "test.h"

int main(void) {
  Suite* suites[] = {
      s21_memchr_suite(),   s21_memcmp_suite(),   s21_memcpy_suite(),
      s21_memset_suite(),   s21_strlen_suite(),   s21_strncat_suite(),
      s21_strchr_suite(),   s21_strncmp_suite(),  s21_strncpy_suite(),
      s21_strcspn_suite(),  s21_strerror_suite(), s21_strpbrk_suite(),
      s21_strrchr_suite(),  s21_strstr_suite(),   s21_strtok_suite(),
      s21_extra_suite(),    s21_sprintf_suite(),  s21_sscanf_suite(),
      s21_rounding_suite(), s21_scanf_i_suite(),
  };
  SRunner* runner = srunner_create(suites[0]);
  for (size_t i = 1; i < sizeof(suites) / sizeof(suites[0]); ++i)
    srunner_add_suite(runner, suites[i]);
  srunner_set_fork_status(runner, CK_NOFORK);
  srunner_run_all(runner, CK_NORMAL);
  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
