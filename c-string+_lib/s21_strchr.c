#include "s21_string.h"

char* s21_strchr(const char* str, int c) {
  s21_size i = 0;
  char symb = c;
  while (str[i] != '\0') {
    if (str[i] == symb) {
      return (char*)&str[i];
    }
    i++;
  }
  if (symb == '\0' && str[i] == '\0') {
    return (char*)&str[i];
  }
  return S21_NULL;
}
