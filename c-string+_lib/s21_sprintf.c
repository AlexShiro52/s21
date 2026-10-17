#include <stdarg.h>
#include <stdio.h>

#include "s21_string.h"

static void s21_widthprint(char** ps, int w, char c) {
  for (int j = 0; j < w; j++) {
    **ps = c;
    (*ps)++;
  }
}

static void s21_signprint(long long n, char** ps, int* flags) {
  if (n < 0) {
    **ps = '-';
    (*ps)++;
  } else {
    if (flags[1] == 1) {
      **ps = '+';
      (*ps)++;
    } else if (flags[6] == 1) {
      **ps = ' ';
      (*ps)++;
    }
  }
}

static void s21_widthsetup(int* flags, int i, int sign) {
  flags[7] = flags[4] ? 0 : flags[7];
  if (flags[1] || flags[6] || sign) {
    flags[3]--;
  }
  if (flags[4] && flags[5] > i) {
    flags[3] -= flags[5];
  } else {
    flags[3] -= i;
  }
  flags[5] -= i;
}

static void s21_arrmult(int* num_len, unsigned char* buf, int x) {
  unsigned char carry = 0;
  for (int i = 0; i < *num_len; i++) {
    unsigned char cur = (buf[i] * x) + carry;
    buf[i] = cur % 10;
    carry = cur / 10;
  }
  if (carry) {
    buf[*num_len] = carry;
    (*num_len)++;
  }
}

static void s21_arrround(int* num_len, unsigned char* buf, int* dec_exp,
                         int rpos) {
  if (rpos >= 0 && rpos < *num_len) {
    int bp = 0;  // 1 если больше половины
    if (buf[rpos] == 5) {
      for (int i = rpos - 1; i >= 0 && bp == 0; i--) {
        bp = (buf[i] != 0) ? 1 : 0;
      }
      if (!bp && buf[rpos + 1] % 2 == 0) {
        buf[rpos] = 0;
      } else {
        buf[rpos] += 5;
      }
    } else {
      buf[rpos] += 5;
    }

    unsigned char carry = 0;
    for (int i = rpos; i < *num_len; i++) {
      unsigned char cur = buf[i] + carry;
      buf[i] = cur % 10;
      carry = cur / 10;
    }
    if (carry) {
      buf[*num_len] = carry;
      (*num_len)++;
      (*dec_exp)++;
    }
  }
}

static void s21_printbuf(char** ps, int* flags, char* buf, int i) {
  if (flags[7] && !flags[0]) {
    s21_widthprint(ps, flags[3], '0');
  }
  if (flags[4]) {
    s21_widthprint(ps, flags[5], '0');
  }
  for (i--; i >= 0; i--) {
    **ps = buf[i];
    (*ps)++;
  }
  if (flags[0]) {
    s21_widthprint(ps, flags[3], ' ');
  }
}

static void s21_printeger(long long n, char** ps, int* flags) {
  char buf[32] = {' '};
  unsigned long long num;
  int i = 0;
  int sign = 0;
  if (n >= 0) {
    num = (unsigned long long)(n);
  } else {
    num = 0ULL - (unsigned long long)n;
    sign = 1;
  }
  if (!(n == 0 && flags[4] == 1 && flags[5] == 0)) {
    do {
      buf[i] = (char)((num % 10) + 48);
      num = num / 10;
      i++;
    } while (num != 0);
  }
  s21_widthsetup(flags, i, sign);
  if (!flags[7] && !flags[0]) {
    s21_widthprint(ps, flags[3], ' ');
  }
  s21_signprint(n, ps, flags);
  s21_printbuf(ps, flags, buf, i);
}

static void s21_prunsign(unsigned long long n, char** ps, int* flags) {
  char buf[32] = {' '};
  int i = 0;
  int sign = 0;

  if (!(n == 0 && flags[4] == 1 && flags[5] == 0)) {
    do {
      buf[i] = (char)((n % 10) + 48);
      n = n / 10;
      i++;
    } while (n != 0);
  }
  s21_widthsetup(flags, i, sign);

  if (!flags[7] && !flags[0]) {
    s21_widthprint(ps, flags[3], ' ');
  }
  s21_printbuf(ps, flags, buf, i);
}

static int s21_double_classify(double f, int* sign) {
  union Doublebits {
    double value;
    unsigned long long bits;
  } dbits;
  dbits.value = f;
  *sign = (int)((dbits.bits >> 63) & 0x1ULL);

  unsigned long long exp_bits = (dbits.bits >> 52) & 0x7FFULL;
  unsigned long long mant_bits = dbits.bits & 0xFFFFFFFFFFFFFULL;

  int result = 0;
  if (exp_bits == 0x7FFULL) {
    result = (mant_bits == 0ULL) ? 1 : 2;  // 1 = inf, 2 = nan
  }
  return result;
}

static void s21_prinfnan(char** ps, int* flags, int neg, int is_nan,
                         int upper) {
  const char* body;
  if (is_nan)
    body = upper ? "NAN" : "nan";
  else
    body = upper ? "INF" : "inf";

  int blen = 3;
  int sgn_len = (neg || flags[1] || flags[6]) ? 1 : 0;
  int w = flags[3] - sgn_len - blen;
  if (w < 0) w = 0;

  if (!flags[0]) s21_widthprint(ps, w, ' ');
  if (neg) {
    **ps = '-';
    (*ps)++;
  } else if (flags[1]) {
    **ps = '+';
    (*ps)++;
  } else if (flags[6]) {
    **ps = ' ';
    (*ps)++;
  }
  for (int i = 0; i < blen; i++) {
    **ps = body[i];
    (*ps)++;
  }
  if (flags[0]) s21_widthprint(ps, w, ' ');
}

static void s21_doublesetup(double f, unsigned char* buf, int* num_len,
                            int* dec_exp, int* sign) {
  static union Doublebits {
    double value;
    unsigned long long bits;
  } dbits;

  dbits.value = f;
  *sign = (int)((dbits.bits >> 63) & 0x1);

  if ((dbits.bits & 0x7FFFFFFFFFFFFFFFULL) == 0ULL) {
    buf[0] = 0;
    *num_len = 1;
    *dec_exp = 1;
  } else {
    unsigned long long mantis = dbits.bits & 0xFFFFFFFFFFFFFULL;
    long long exp = (long long)((dbits.bits >> 52) & 0x7FFULL);
    if (exp != 0) mantis |= 0x10000000000000ULL;
    exp = (exp ? exp : 1) - 1023 - 52;

    do {
      buf[*num_len] = (unsigned char)((mantis % 10));
      mantis = mantis / 10;
      (*num_len)++;
    } while (mantis != 0);

    if (exp >= 0) {
      for (int j = 0; j < exp; j++) {
        s21_arrmult(num_len, buf, 2);
      }
      *dec_exp = *num_len;
    } else if (exp < 0) {
      for (int j = 0; j > exp; j--) {
        s21_arrmult(num_len, buf, 5);
      }
      *dec_exp = *num_len + exp;
    }
  }
}

static int s21_rawf(char** ps, unsigned char* buf, int num_len, int dec_exp,
                    int prec, int hash) {
  char* start = *ps;

  if (dec_exp <= 0) {
    **ps = '0';
    (*ps)++;
  } else {
    for (int i = 0; i < dec_exp; i++) {
      **ps = buf[num_len - i - 1] + '0';
      (*ps)++;
    }
  }
  int starti = num_len - dec_exp - 1;
  if (prec > 0 || hash) {
    **ps = '.';
    (*ps)++;
  }
  for (int i = 0; i < prec; i++) {
    int ci = starti - i;
    **ps = (ci >= 0 && ci < num_len) ? (char)(buf[ci] + '0') : '0';
    (*ps)++;
  }
  return (int)(*ps - start);
}

static int s21_rawe(char** ps, unsigned char* buf, int num_len, int dec_exp,
                    int prec, char er, int hash) {
  char* start = *ps;

  **ps = buf[num_len - 1] + '0';
  (*ps)++;
  int frac_start_idx = num_len - 2;
  if (prec > 0 || hash) {
    **ps = '.';
    (*ps)++;
  }
  for (int i = 0; i < prec; i++) {
    int ci = frac_start_idx - i;
    **ps = (ci >= 0 && ci < num_len) ? (char)(buf[ci] + '0') : '0';
    (*ps)++;
  }
  **ps = er;
  (*ps)++;
  int fuflags[8] = {0, 1, 0, 3, 0, 0, 0, 1};
  s21_printeger((long long)dec_exp - 1, ps, fuflags);

  return (int)(*ps - start);
}

static void s21_prdouble(double f, char** ps, int* flags) {
  int sign = 0;
  int cls = s21_double_classify(f, &sign);
  if (cls != 0) {
    s21_prinfnan(ps, flags, sign, cls == 2, 0);

  } else {
    unsigned char buf[1024] = {0};
    int num_len = 0;
    int dec_exp = 0;
    s21_doublesetup(f, buf, &num_len, &dec_exp, &sign);

    if (flags[4] == 0) flags[5] = 6;
    int rpos = num_len - dec_exp - flags[5] - 1;
    s21_arrround(&num_len, buf, &dec_exp, rpos);

    int dot_len = (flags[5] > 0 || flags[2]) ? 1 : 0;

    if (flags[1] || flags[6] || sign) flags[3]--;
    if (dec_exp <= 0)
      flags[3] -= flags[5] + 1 + dot_len;
    else
      flags[3] -= flags[5] + dec_exp + dot_len;

    if (!flags[7] && !flags[0]) s21_widthprint(ps, flags[3], ' ');
    s21_signprint(sign ? -1 : 0, ps, flags);
    if (flags[7] && !flags[0]) s21_widthprint(ps, flags[3], '0');

    s21_rawf(ps, buf, num_len, dec_exp, flags[5], flags[2]);

    if (flags[0]) s21_widthprint(ps, flags[3], ' ');
  }
}

static void s21_predouble(double f, char** ps, int* flags, char er) {
  int sign = 0;
  int cls = s21_double_classify(f, &sign);
  if (cls != 0) {
    s21_prinfnan(ps, flags, sign, cls == 2, er == 'E');
  } else {
    unsigned char buf[1024] = {0};
    int num_len = 0;
    int dec_exp = 0;
    s21_doublesetup(f, buf, &num_len, &dec_exp, &sign);

    if (flags[4] == 0) flags[5] = 6;
    int rpos = num_len - flags[5] - 2;
    s21_arrround(&num_len, buf, &dec_exp, rpos);

    int dot_len = (flags[5] > 0 || flags[2]) ? 1 : 0;
    int exp_digits = 2;
    for (int e = abs(dec_exp - 1); e >= 100; e /= 10) exp_digits++;
    flags[3] -= 1 + dot_len + flags[5] + 2 + exp_digits;
    if (flags[1] || flags[6] || sign) flags[3]--;

    if (!flags[7] && !flags[0]) s21_widthprint(ps, flags[3], ' ');
    s21_signprint(sign ? -1 : 0, ps, flags);
    if (flags[7] && !flags[0]) s21_widthprint(ps, flags[3], '0');

    s21_rawe(ps, buf, num_len, dec_exp, flags[5], er, flags[2]);

    if (flags[0]) s21_widthprint(ps, flags[3], ' ');
  }
}

static int s21_gzeros(char* tmp, int len, char er) {
  int dot = -1, epos = -1;
  for (int i = 0; i < len && epos < 0; i++) {
    if (tmp[i] == '.')
      dot = i;
    else if (tmp[i] == er)
      epos = i;
  }

  if (dot >= 0) {
    int mant_end = (epos >= 0) ? epos : len;
    int end = mant_end;
    while (end > dot + 1 && tmp[end - 1] == '0') end--;
    if (end == dot + 1) end = dot;

    if (epos < 0) {
      len = end;
    } else {
      int shift = mant_end - end;
      for (int i = end; i + shift < len; i++) tmp[i] = tmp[i + shift];
      len -= shift;
    }
  }
  return len;
}

static void s21_prgdouble(double f, char** ps, int* flags, char er) {
  int sign = 0;
  int cls = s21_double_classify(f, &sign);
  if (cls != 0) {
    s21_prinfnan(ps, flags, sign, cls == 2, er == 'E');
  } else {
    int P;
    if (!flags[4])
      P = 6;
    else if (!flags[5])
      P = 1;
    else
      P = flags[5];
    unsigned char buf[1024] = {0};
    int num_len = 0;
    int dec_exp = 0;
    s21_doublesetup(f, buf, &num_len, &dec_exp, &sign);

    int rpos = num_len - P - 1;
    s21_arrround(&num_len, buf, &dec_exp, rpos);

    int X = dec_exp - 1;
    int use_f = (X >= -4 && X < P);
    char* tmp = malloc((s21_size)P + 16);
    if (tmp == S21_NULL) return;
    char* tp = tmp;
    int len;
    if (use_f) {
      int prec = P - 1 - X;
      len = s21_rawf(&tp, buf, num_len, dec_exp, prec, flags[2]);
    } else {
      int prec = P - 1;
      len = s21_rawe(&tp, buf, num_len, dec_exp, prec, er, flags[2]);
    }
    if (!flags[2]) {
      len = s21_gzeros(tmp, len, er);
    }
    int sgn_len = (sign || flags[1] || flags[6]) ? 1 : 0;
    int w = flags[3] - sgn_len - len;
    if (w < 0) w = 0;

    if (!flags[0] && !flags[7]) s21_widthprint(ps, w, ' ');
    s21_signprint(sign ? -1 : 0, ps, flags);
    if (flags[7] && !flags[0]) s21_widthprint(ps, w, '0');
    for (int i = 0; i < len; i++) {
      **ps = tmp[i];
      (*ps)++;
    }
    if (flags[0]) s21_widthprint(ps, w, ' ');
    free(tmp);
  }
}

static void s21_prstr(char* s, char** ps, int* flags) {
  int l = 0;

  if (s != NULL) {
    char* t = s;
    while ((!flags[4] || l < flags[5]) && *t != '\0') {
      l++;
      t++;
    }
  }

  if (l > flags[5] && flags[4] == 1) {
    l = flags[5];
  }

  flags[3] -= l;
  if (flags[0] == 0) {
    s21_widthprint(ps, flags[3], ' ');
  }

  for (int i = 0; i < l; i++) {
    **ps = s[i];
    (*ps)++;
  }

  if (flags[0] == 1) {
    s21_widthprint(ps, flags[3], ' ');
  }
}

static void s21_prch(char c, char** ps, int* flags) {
  if (flags[0] == 0) {
    s21_widthprint(ps, flags[3] - 1, ' ');
  }

  **ps = c;
  (*ps)++;

  if (flags[0] == 1) {
    s21_widthprint(ps, flags[3] - 1, ' ');
  }
}

static void s21_proct(unsigned long long n, char** ps, int* flags) {
  int digit = 0;
  char buf[32] = {0};
  int i = 0;
  unsigned long long nn = n;
  if (!(n == 0 && flags[4] == 1 && flags[5] == 0)) {
    do {
      digit = n & 0x7;
      buf[i] = (char)(digit + 48);
      i++;

      n = n >> 3;
    } while (n != 0);
  }
  int oct_zero = flags[2] && (nn != 0 || i == 0);
  if (oct_zero) {
    flags[3]--;
    flags[5]--;
  }
  s21_widthsetup(flags, i, flags[1]);
  if (!flags[7] && !flags[0]) {
    s21_widthprint(ps, flags[3], ' ');
  }

  if (oct_zero) {
    buf[i] = '0';
    i++;
  }
  s21_printbuf(ps, flags, buf, i);
}

static void s21_prhex(unsigned long long n, char** ps, int mode, int* flags) {
  int digit = 0;
  char buf[64] = {0};
  int i = 0;
  unsigned long long nn = n;

  if (!(n == 0 && flags[4] == 1 && flags[5] == 0)) {
    do {
      digit = n & 0xf;
      if (digit < 10) {
        buf[i] = (char)(digit + 48);
      } else {
        buf[i] = (char)(digit + mode);
      }
      i++;
      n = n >> 4;
    } while (n != 0);
  }

  if (nn != 0 && flags[2]) {
    flags[3] -= 2;
  }

  s21_widthsetup(flags, i, flags[1]);

  if (!flags[7] && !flags[0]) {
    s21_widthprint(ps, flags[3], ' ');
  }

  s21_signprint(n, ps, flags);
  if (nn != 0 && flags[2]) {
    **ps = '0';
    (*ps)++;
    **ps = (char)(mode + 33);
    (*ps)++;
  }
  s21_printbuf(ps, flags, buf, i);
}

static void s21_prpointer(unsigned long long n, char** ps, int* flags) {
  if (n == 0) {
    char body[] = "(nil)";
    flags[4] = 0;
    flags[5] = 0;
    s21_prstr(body, ps, flags);
  } else {
    flags[2] = 1;
    s21_prhex(n, ps, 87, flags);
  }
}

static const char* s21_flagsparse(const char* pf, int* flags) {
  int fl = 1;
  while (fl) {
    if (*pf == '-') {
      flags[0] = 1;
      (pf)++;
    } else if (*pf == '0') {
      flags[7] = 1;
      (pf)++;
    } else if (*pf == '+') {
      flags[1] = 1;
      (pf)++;
    } else if (*pf == ' ') {
      flags[6] = 1;
      (pf)++;
    } else if (*pf == '#') {
      flags[2] = 1;
      (pf)++;
    } else {
      fl = 0;
    }
  }
  return pf;
}

static const char* s21_widthparse(const char* pf, int* flags, va_list* args) {
  if (*pf == '*') {
    flags[3] = va_arg(*args, int);
    if (flags[3] < 0) {
      flags[0] = 1;
      flags[3] = -flags[3];
    }
    pf++;
  } else {
    while (*pf >= '0' && *pf <= '9') {
      flags[3] *= 10;
      flags[3] += (int)(*pf - '0');
      pf++;
    }
  }
  return pf;
}

static const char* s21_precparse(const char* pf, int* flags, va_list* args) {
  if (*pf == '.') {
    flags[4] = 1;
    pf++;
    if (*pf == '*') {
      flags[5] = va_arg(*args, int);
      if (flags[5] < 0) {
        flags[4] = 0;
        flags[5] = 0;
      }
      pf++;
    } else {
      while (*pf >= '0' && *pf <= '9') {
        flags[5] *= 10;
        flags[5] += (int)(*pf - '0');
        pf++;
      }
    }
  }
  return pf;
}

static const char* s21_lenparse(const char* pf, int* len_mod) {
  if (*pf == 'h') {
    pf++;
    if (*pf == 'h') {
      *len_mod = 2;
      pf++;
    } else {
      *len_mod = 1;
    }
  } else if (*pf == 'l') {
    pf++;
    if (*pf == 'l') {
      *len_mod = 4;
      pf++;
    } else {
      *len_mod = 3;
    }
  } else if (*pf == 'L') {
    *len_mod = 5;
    pf++;
  }
  return pf;
}

static long long s21_intlength(va_list* args, int len_mod) {
  long long v;
  if (len_mod == 1)
    v = (short)va_arg(*args, int);
  else if (len_mod == 2)
    v = (signed char)va_arg(*args, int);
  else if (len_mod == 3)
    v = va_arg(*args, long);
  else if (len_mod == 4)
    v = va_arg(*args, long long);
  else
    v = va_arg(*args, int);

  return v;
}

static unsigned long long s21_unslength(va_list* args, int len_mod,
                                        int* flags) {
  flags[1] = 0;
  flags[6] = 0;
  unsigned long long v;
  if (len_mod == 1)
    v = (unsigned short)va_arg(*args, int);
  else if (len_mod == 2)
    v = (unsigned char)va_arg(*args, int);
  else if (len_mod == 3)
    v = va_arg(*args, unsigned long);
  else if (len_mod == 4)
    v = va_arg(*args, unsigned long long);
  else
    v = va_arg(*args, unsigned int);
  return v;
}

static double s21_doublength(va_list* args, int len_mod) {
  double v = (len_mod == 5) ? (double)va_arg(*args, long double)
                            : va_arg(*args, double);
  return v;
}

static int s21_specparse(char spec, char** ps, int* flags, int len_mod,
                         va_list* args) {
  int handled = 1;
  if (spec == 'c') {
    s21_prch((char)va_arg(*args, int), ps, flags);
  } else if (spec == 's') {
    s21_prstr(va_arg(*args, char*), ps, flags);
  } else if (spec == 'd') {
    s21_printeger(s21_intlength(args, len_mod), ps, flags);
  } else if (spec == 'u') {
    s21_prunsign(s21_unslength(args, len_mod, flags), ps, flags);
  } else if (spec == 'o') {
    s21_proct(s21_unslength(args, len_mod, flags), ps, flags);
  } else if (spec == 'x') {
    s21_prhex(s21_unslength(args, len_mod, flags), ps, 87, flags);
  } else if (spec == 'X') {
    s21_prhex(s21_unslength(args, len_mod, flags), ps, 55, flags);
  } else if (spec == 'f') {
    s21_prdouble(s21_doublength(args, len_mod), ps, flags);
  } else if (spec == 'e') {
    s21_predouble(s21_doublength(args, len_mod), ps, flags, 'e');
  } else if (spec == 'E') {
    s21_predouble(s21_doublength(args, len_mod), ps, flags, 'E');
  } else if (spec == 'g') {
    s21_prgdouble(s21_doublength(args, len_mod), ps, flags, 'e');
  } else if (spec == 'G') {
    s21_prgdouble(s21_doublength(args, len_mod), ps, flags, 'E');
  } else if (spec == 'p') {
    s21_prpointer((unsigned long long)va_arg(*args, void*), ps, flags);
  } else if (spec == '%') {
    **ps = '%';
    (*ps)++;
  } else {
    handled = 0;
  }
  return handled;
}

// flags[0] - минус(-)
// flags[1] - плюс(+)
// flags[2] - решётка(#)
// flags[3] - значение ширины
// flags[4] - точность
// flags[5] - значение точности
// flags[6] - пробел( )
// flags[7] - нуль(0)

int s21_sprintf(char* str, const char* format, ...) {
  char* ps = str;
  const char* pf = format;
  const char* specstart;
  va_list args;
  va_start(args, format);

  int state = 0;
  while (*pf != '\0') {
    if (state == 0 && *pf == '%') {
      state = 1;
      specstart = pf;
    } else if (state == 1) {
      int flags[8] = {0};
      ;
      int len_mod = 0;

      pf = s21_flagsparse(pf, flags);
      pf = s21_widthparse(pf, flags, &args);
      pf = s21_precparse(pf, flags, &args);
      pf = s21_lenparse(pf, &len_mod);

      if (s21_specparse(*pf, &ps, flags, len_mod, &args) == 0) {
        pf = specstart;
        *ps = '%';
        ps++;
      }
      state = 0;
    } else {
      *ps = *pf;
      ps++;
    }

    pf++;
  }

  *ps = '\0';
  va_end(args);
  return (int)(ps - str);
}
