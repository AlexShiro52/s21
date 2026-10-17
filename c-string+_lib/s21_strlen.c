#include "s21_string.h"

s21_size s21_strlen(const char* str) {
  s21_size len = 0;
  for (s21_size i = 0; str[i] != '\0'; i++) {
    len += 1;
  }
  return len;
}
