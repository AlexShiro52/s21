#include "s21_string.h"

void* s21_memchr(const void* str, int c, s21_size n) {
  unsigned const char* string = str;
  unsigned char symb = c;
  for (s21_size i = 0; i < n; i++) {
    if (string[i] == symb) {
      return ((void*)&string[i]);
    }
  }
  return S21_NULL;
}
