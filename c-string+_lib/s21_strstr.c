#include "s21_string.h"

char* s21_strstr(const char* haystack, const char* needle) {
  if (needle[0] == '\0') {
    return (char*)haystack;
  }
  for (s21_size i = 0; haystack[i] != '\0'; i++) {
    s21_size j = 0;
    if (haystack[i] == needle[j]) {
      for (; needle[j] != '\0'; j++) {
        if (haystack[i + j] == '\0' && needle[j] != '\0') {
          break;
        } else if (haystack[i + j] != needle[j]) {
          break;
        }
      }
      if (needle[j] == '\0') {
        return (char*)&haystack[i];
      }
    }
  }
  return S21_NULL;
}
