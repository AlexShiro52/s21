#include "s21_string.h"

int s21_strncmp(const char* str1, const char* str2, s21_size n) {
  int res = 0;
  for (s21_size i = 0; i < n; i++) {
    if (str1[i] == '\0' && str2[i] == '\0') {
      return 0;
    }
    if ((unsigned char)str1[i] < (unsigned char)str2[i]) {
      res = (unsigned char)str1[i] - (unsigned char)str2[i];
      return res;
    } else if ((unsigned char)str1[i] > (unsigned char)str2[i]) {
      res = (unsigned char)str1[i] - (unsigned char)str2[i];
      return res;
    }
  }
  return 0;
}

// abc abd

// abd abc

// 10
// 11
