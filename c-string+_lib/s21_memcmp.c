#include "s21_string.h"

int s21_memcmp(const void* str1, const void* str2, s21_size n) {
  unsigned const char* string1 = str1;
  unsigned const char* string2 = str2;
  int res = 0;
  for (s21_size i = 0; i < n; i++) {
    if (string1[i] < string2[i]) {
      res = string1[i] - string2[i];
      return res;
    } else if (string1[i] > string2[i]) {
      res = string1[i] - string2[i];
      return res;
    }
  }
  return res;
}
