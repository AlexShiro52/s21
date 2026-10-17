#include "s21_string.h"

void* s21_insert(const char* src, const char* str, s21_size start_index) {
  if (src == S21_NULL || str == S21_NULL) {
    return S21_NULL;
  }
  if (start_index > s21_strlen(src)) {
    return S21_NULL;
  }
  char* res =
      (char*)malloc((s21_strlen(src) + s21_strlen(str) + 1) * sizeof(char));
  if (res == S21_NULL) {
    return S21_NULL;
  }
  s21_size i = 0;
  s21_size j = 0;
  int was_str = 0;
  for (; src[i] != '\0'; i++) {
    if (i != start_index && was_str == 0) {
      res[i] = src[i];
    } else if (i == start_index) {
      was_str = 1;
      for (; str[j] != '\0'; j++) {
        res[i + j] = str[j];
      }
      res[i + j] = src[i];
    } else if (i != start_index && was_str == 1) {
      res[i + j] = src[i];
    }
  }
  if (start_index == i) {
    for (; str[j] != '\0'; j++) {
      res[i + j] = str[j];
    }
  }
  res[i + j] = '\0';
  return res;
}
