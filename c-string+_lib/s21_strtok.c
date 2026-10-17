#include "s21_string.h"

char* s21_strtok(char* str, const char* delim) {
  int j = 0;
  int i = 0;
  static char* temp_str;
  if (str != S21_NULL) {
    for (; str[i] != '\0'; i++) {
      int was_coincidence = 0;
      for (int j = 0; delim[j] != '\0'; j++) {
        if (str[i] == delim[j]) {
          was_coincidence = 1;
        }
      }
      if (was_coincidence == 0) {
        break;
      }
    }
    if (str[i] == '\0') {
      temp_str = S21_NULL;
      return S21_NULL;
    } else {
      temp_str = &str[i];
    }
  }
  if (str == S21_NULL) {
    if (temp_str != S21_NULL) {
      str = temp_str;
    } else {
      return S21_NULL;
    }
    for (; str[i] != '\0'; i++) {
      int was_coincidence = 0;
      for (int j = 0; delim[j] != '\0'; j++) {
        if (str[i] == delim[j]) {
          was_coincidence = 1;
        }
      }
      if (was_coincidence == 0) {
        break;
      }
    }
    if (str[i] == '\0') {
      temp_str = S21_NULL;
      return S21_NULL;
    } else {
      temp_str = &str[i];
    }
  }
  str = temp_str;
  for (i = 0; str[i] != '\0'; i++) {
    for (j = 0; delim[j] != '\0'; j++) {
      if (str[i] == delim[j]) {
        str[i] = '\0';
        i++;
        temp_str = &str[i];
        return str;
      }
    }
  }
  if (str[i] == '\0') {
    temp_str = S21_NULL;
    return str;
  }

  return S21_NULL;
}
