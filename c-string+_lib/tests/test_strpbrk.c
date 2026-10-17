#include "test.h"

START_TEST(s21_test_strpbrk) {
  const char* inputs[] = {"", "abc", "abcdef", "aaa", "hello world", "\t\n\r"};
  const char* sets[] = {"", "a", "f", "xyz", "bcd", " \t\n"};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    for (size_t j = 0; j < sizeof(sets) / sizeof(sets[0]); ++j)
      ck_assert_ptr_eq(s21_strpbrk(inputs[i], sets[j]),
                       strpbrk(inputs[i], sets[j]));
}
END_TEST

S21_SUITE(strpbrk, s21_test_strpbrk)
