#include "s21_string.h"

char* s21_strpbrk(const char* str1, const char* str2) {
  int was_char = 0;
  for (s21_size i = 0; str1[i] != '\0'; i++) {
    for (s21_size j = 0; str2[j] != '\0'; j++) {
      if (str1[i] == str2[j]) {
        was_char = 1;
      }
    }
    if (was_char == 1) {
      return (char*)&str1[i];
    }
  }
  return S21_NULL;
}
