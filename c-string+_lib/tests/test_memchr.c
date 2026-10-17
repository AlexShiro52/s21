#include "test.h"

START_TEST(s21_test_memchr) {
  const unsigned char data[] = {0, 'a', 255, 'a', 128, 'z', 0};
  const int chars[] = {0, 'a', 'z', 'x', 255, 128, -1, 256 + 'a'};
  for (size_t n = 0; n <= sizeof(data); ++n)
    for (size_t i = 0; i < sizeof(chars) / sizeof(chars[0]); ++i)
      ck_assert_ptr_eq(s21_memchr(data, chars[i], n),
                       memchr(data, chars[i], n));
}
END_TEST

S21_SUITE(memchr, s21_test_memchr)
