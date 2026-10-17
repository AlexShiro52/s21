#include "s21_string.h"

// !!!!HE!LLO!!!!!

void* s21_trim(const char* src, const char* trim_chars) {
  if (src == S21_NULL || trim_chars == S21_NULL) {
    return S21_NULL;
  }
  int len = s21_strlen(src);
  char* res = (char*)malloc((s21_strlen(src) + 1) * sizeof(char));
  if (res == S21_NULL) {
    return S21_NULL;
  }
  int res_ind = 0;
  int st_ind = 0;
  int fin_ind = 0;
  int was_match = 0;
  if (len == 0) {
    res[res_ind] = '\0';
    return res;
  }
  for (int i = 0; i < len; i++) {
    was_match = 0;
    for (int j = 0; trim_chars[j] != '\0'; j++) {
      if (src[i] == trim_chars[j]) {
        was_match = 1;
      }
    }
    if (was_match == 0) {
      st_ind = i;
      break;
    }
  }
  if (was_match == 1) {
    res[res_ind] = '\0';
    return res;
  }
  for (int i = len - 1; i >= 0; i--) {
    was_match = 0;
    for (int j = 0; trim_chars[j] != '\0'; j++) {
      if (src[i] == trim_chars[j]) {
        was_match = 1;
      }
    }
    if (was_match == 0) {
      fin_ind = i;
      break;
    }
  }
  for (; st_ind <= fin_ind; st_ind++) {
    res[res_ind] = src[st_ind];
    res_ind++;
  }
  res[res_ind] = '\0';
  return res;
}
