#include "s21_string.h"

void* s21_memcpy(void* dest, const void* src, s21_size n) {
  unsigned const char* stringfrom = src;
  unsigned char* stringto = dest;
  for (s21_size i = 0; i < n; i++) {
    stringto[i] = stringfrom[i];
  }
  return dest;
}
