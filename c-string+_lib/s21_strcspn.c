#include "s21_string.h"

s21_size s21_strcspn(const char* str1, const char* str2) {
  s21_size len = 0;
  int was_ch = 0;
  for (s21_size i = 0; str1[i] != '\0'; i++) {
    for (s21_size j = 0; str2[j] != '\0'; j++) {
      if (str1[i] == str2[j]) {
        was_ch = 1;
      }
    }
    if (was_ch == 0) {
      len += 1;
    } else if (was_ch == 1) {
      return len;
    }
  }
  return len;
}
