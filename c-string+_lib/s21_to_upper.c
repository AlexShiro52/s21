#include "s21_string.h"

void* s21_to_upper(const char* str) {
  if (str == S21_NULL) {
    return S21_NULL;
  }
  char* res = (char*)malloc((s21_strlen(str) + 1) * sizeof(char));
  if (res == S21_NULL) {
    return S21_NULL;
  }
  int i = 0;
  for (; str[i] != '\0'; i++) {
    if (str[i] >= 'a' && str[i] <= 'z') {
      res[i] = str[i] - 32;
    } else {
      res[i] = str[i];
    }
  }
  res[i] = '\0';
  return res;
}
