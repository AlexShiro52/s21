#include "s21_string.h"

char* s21_strrchr(const char* str, int c) {
  char symb = c;
  s21_size index = 0;
  int was_char = 0;
  s21_size i = 0;
  while (str[i] != '\0') {
    if (str[i] == symb) {
      index = i;
      was_char = 1;
    }
    i++;
  }
  if (str[i] == '\0' && symb == '\0') {
    return (char*)&str[i];
  }
  if (was_char == 1) {
    return (char*)&str[index];
  }
  return S21_NULL;
}
