#include "s21_string.h"

char* s21_strncpy(char* dest, const char* src, s21_size n) {
  s21_size src_len = 0;
  while (src_len < n && src[src_len] != '\0') src_len++;
  if (src_len < n) {
    s21_size i = 0;
    for (; src[i] != '\0'; i++) {
      dest[i] = src[i];
    }
    for (; i < n; i++) {
      dest[i] = '\0';
    }
  } else if (src_len >= n) {
    for (s21_size i = 0; i < n && src[i] != '\0'; i++) {
      dest[i] = src[i];
    }
  }
  return dest;
}
