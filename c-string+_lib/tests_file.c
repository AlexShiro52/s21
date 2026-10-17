#include <stdio.h>
#include <string.h>

#include "s21_string.h"

// #test strlen:

// int main(void) {
//     char *a = "A";
//     char *b = "asfklmlkdnalksn";
//     size_t c = strlen(b);
//     if (c != -1) {
//         printf("%zu\n", c);
//     }
//     s21_size d = s21_strlen(b);
//     if (d != -1) {
//         printf("%llu\n", d);
//     }
//     else {
//         fprintf(stderr, "error\n");
//     }
// }

// #test memset:

// int main(void) {
//     int a = 'G';
//     s21_size n = 3;
//     char *s = "adkfsdlfkn";
//     char buffer1[1024];
//     int i1 = 0;
//     char buffer2[1024] = "adkfsdlfkn";
//     for (int i = 0; s[i] != '\0'; i++) {
//         buffer1[i1] = s[i];
//         i1 += 1;
//     }
//     buffer1[i1] = '\0';
//     memset(buffer1, a, n);
//     printf("%s\n", buffer1);
//     s21_memset(buffer2, a, n);
//     printf("%s\n", buffer2);
// }

// #test memcpy:

// int main(void) {
//     s21_size n = 4;
//     char src[1024] = "ABCDjaskjdlkasjdl";
//     char dest1[1024] = "1234567";
//     char dest2[1024] = "0123456789";
//     s21_memcpy(dest1, src, n);
//     printf("%s\n", dest1);
//     memcpy(dest2, src, n);
//     printf("%s\n", dest2);
// }

// #test memchr:

// int main(void) {
//     s21_size n = 4;
//     int c = 'B';
//     char buffer1[1024] = "ABCDnjasndlasnmdn";
//     char buffer2[1024] = "ABCDnjasndlasnmdn";
//     void* res1 = s21_memchr(buffer1, c, n);
//     if (res1 == (void*)&buffer1[1]) {
//         printf("true1\n");
//     }
//     void* res2 = memchr(buffer2, c, n);
//     if (res2 == (void*)&buffer2[1]) {
//         printf("true2\n");
//     }
// }

// #test strchr:

// int main(void) {
//     int c = 'M';
//     char str[1024] = "dasudhausMasudhasuhd";
//     char *res1 = strchr(str, c);
//     char *res2 = s21_strchr(str, c);
//     if (res1 == res2) {
//         printf("true\n");
//     }
// }

// #strRchr:
//  int main(void) {
//      int c = 'a';
//      char str[1024] = "amdsWjnfsdWlkf";
//      char *res1 = s21_strrchr(str, c);
//      char *res2 = strrchr(str, c);
//      if (res1 == res2) {
//          printf("true\n");
//      }
//  }

// #memcmp:

// int main(void) {
//     s21_size n = 5;
//     char str11[1024] = "ABCDEfdslkdnf";
//     char str12[1024] = "ABCDEfdslkdnf";
//     char str21[1024] = "ABCDAfdslkdnf";
//     char str22[1024] = "ABCDEfdslkdnf";
//     char str31[1024] = "ABCDEfdslkdnf";
//     char str32[1024] = "ABCDAfdslkdnf";
//     int res1_base = memcmp(str11, str12, 5);
//     int res2_base = memcmp(str21, str22, 5);
//     int res3_base = memcmp(str31, str32, 5);
//     printf("%d\n%d\n%d\n", res1_base, res2_base, res3_base);
//     int res1_sch21 = s21_memcmp(str11, str12, 5);
//     int res2_sch21 = s21_memcmp(str21, str22, 5);
//     int res3_sch21 = s21_memcmp(str31, str32, 5);
//     printf("%d\n%d\n%d\n", res1_sch21, res2_sch21, res3_sch21);
// }

// #test strncmp:

// int main(void) {
//     char str1[1024] = "fdkjcjfklsdjf";
//     char str2[1024] = "fdkjsfksjdfk;jsdf";
//     s21_size n = 5;
//     int res1 = strncmp(str1, str2, n);
//     int res2 = s21_strncmp(str1, str2, n);
//     printf("%d\n%d\n", res1, res2);
// }

// #test s21_strncat.c:

// int main(void) {
//     char str1[1024] = "Hi";
//     char str2[1024] = "abc";
//     s21_size n = 2;
//     char *res = s21_strncat(str1, str2, n);
//     char *res_t = strncat(str1, str2, n);
//     if (res == res_t) {
//         printf("true");
//     }
//     return 0;
// }

// #test s21_strncpy:

// int main(void) {
//     char str1[1024] = "ASDGJKasdjaks";
//     char str2[1024] = "masdkaskdmsalkmaskdm";
//     s21_size n = 5;
//     char *res1 = s21_strncpy(str2, str1, n);
//     char *res2 = strncpy(str2, str1, n);
//     if (res1 == res2) {
//         printf("true\n");
//     }
//     printf("%s\n%s\n", res1, res2);
//     return 0;
// }

// s21_strcspn.c:

// int main(void) {
//     char a[1024] = "asx";
//     char b[1024] = "oxvbd";
//     s21_size res1 = s21_strcspn(a, b);
//     s21_size res2 = strcspn(a, b);
//     printf("%llu\n%llu", res1, res2);
// }

// s21_strpbrk.c:

// int main(void) {
//     char str1[1024] = "fdjKflsdkfj";
//     char str2[1024] = "K";
//     char *res1 = s21_strpbrk(str1, str2);
//     char *res2 = strpbrk(str1, str2);
//     if (res1 == res2) {
//         printf("true\n");
//     }
//     printf("%s\n%s\n", res1, res2);
// }

// test s21_strstr:

// int main(void) {
//     char str1[1024] = "dfhjscatlkdjflkdsjfkl";
//     char str2[1024] = "cat";
//     char *res1 = s21_strstr(str1, str2);
//     char *res2 = strstr(str1, str2);
//     if (res1 == res2) {
//         printf("true\n");
//     }
//     printf("%s\n%s\n", res1, res2);
//     printf("%p\n%p\n", res1, res2);
// }

// test s21_strerror:

// int main(void) {
//     for (int i = -2; i < 110; i++) {
//         char *res = strerror(i);
//         char *res_my = s21_strerror(i);
//         printf("%d %s\n", i, res);
//         printf("%d %s\n", i, res_my);
//     }
// }

// test s21_strtok.c:

// int main(void) {
//     char str1[1024] = "ab.cd;e";
//     char str2[1024] = "ab.cd;e";
//     char delim[1024] = ".;";
//     char *res_my = s21_strtok(str1, delim);
//     char *res_func = strtok(str2, delim);
//     char *res_my2 = s21_strtok(S21_NULL, delim);
//     char *res_func2 = strtok(NULL, delim);
//     printf("%s\n%s\n%s\n%s\n", res_my, res_func, res_my2, res_func2);
//     if (res_my == res_func) {
//         printf("true");
//     }
// }

// test s21_sscanf:

int main(void) {
  // char a[1024] = "25 A hello";
  char str1[1024] = "12.345";
  char str_my[1024] = ".5 7";
  float numb1 = 999;
  float numb2 = 1;
  float my_numb1 = 999;
  float my_numb2 = 1;
  int res1 = sscanf(str_my, "%f %f", &numb1, &numb2);
  int res2 = s21_sscanf(str_my, "%f %f", &my_numb1, &my_numb2);
  if (res1 == res2) {
    printf("true\n");
  }
  printf("%f %f %s\n%f %f %s\n%d %d\n", numb1, numb2, str1, my_numb1, my_numb2,
         str_my, res1, res2);
}
