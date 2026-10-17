#include "s21_string.h"

char* s21_strncat(char* dest, const char* src, s21_size n) {
  s21_size end_of_dest = s21_strlen(dest);
  s21_size end_of_src = 0;
  while (end_of_src < n && src[end_of_src] != '\0') end_of_src++;
  s21_size i = 0;
  if (end_of_src < n) {
    while (src[i] != '\0') {
      dest[i + end_of_dest] = src[i];
      i++;
    }
    dest[i + end_of_dest] = '\0';
  } else if (end_of_src >= n) {
    s21_size j = 0;
    for (; j < n; j++) {
      dest[j + end_of_dest] = src[j];
    }
    dest[j + end_of_dest] = '\0';
  }
  return dest;
}
