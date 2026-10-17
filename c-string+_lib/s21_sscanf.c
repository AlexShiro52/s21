#include "s21_string.h"

int s21_read_d(const char* str, void* address, int* j, int width, int was_star,
               char read_mod);

int s21_read_c(const char* str, char* address, int* j, int width, int was_star);

int s21_read_s(const char* str, char* address, int* j, int width, int was_star);

int s21_read_u(const char* str, void* address, int* j, int width, int was_star,
               char read_mod);

int s21_read_o(const char* str, void* address, int* j, int width, int was_star,
               int o_abs, char read_mod);

int s21_read_x(const char* str, void* address, int* j, int width, int was_star,
               int x_abs, char read_mod);

int s21_read_i(const char* str, void* address, int* j, int width, int was_star,
               char read_mod);

int s21_read_f(const char* str, void* address, int* j, int width, int was_star,
               char read_mod);

int s21_obr_i_zn(char read_mod, int is_otr, unsigned long read_hex_oct,
                 void* address);

int s21_obr_i_nozn(char read_mod, unsigned long read_hex_oct, void* address);

int s21_read_p(const char* str, void** address, int* j, int width,
               int was_star);

int s21_all_read_func(va_list* args, int* j, const char* str, char read_spec,
                      char read_mod, int width, int was_star, int* was_percent,
                      int* was_n_spec);

int s21_space_skip(const char* format, const char* str, int* i, int* j);

int s21_specification_analysis(const char* format, int* i, char* read_spec,
                               char* read_mod, int* width, int* was_star);

long double s21_exp_pow(int st, int exp_otr_numb);

int s21_sscanf(const char* str, const char* format, ...) {
  char read_spec;
  char read_mod;
  int was_star = 0;
  int width = 0;
  va_list args;
  int result = 0;
  int j = 0;
  int was_percent = 0;
  int was_n_spec = 0;
  va_start(args, format);
  for (int i = 0; format[i] != '\0'; i++) {
    if (s21_space_skip(format, str, &i, &j) == 1) {
      break;
    }
    if (format[i] == '%') {
      int specification_analysis_res = s21_specification_analysis(
          format, &i, &read_spec, &read_mod, &width, &was_star);
      if (specification_analysis_res == 1) {
        break;
      }
      int all_read_func_res =
          s21_all_read_func(&args, &j, str, read_spec, read_mod, width,
                            was_star, &was_percent, &was_n_spec);
      if (all_read_func_res == 1) {
        break;
      } else if (all_read_func_res == 0 && was_star == 0 && was_percent == 0 &&
                 was_n_spec == 0) {
        result += 1;
      } else if (all_read_func_res == -1) {
        if (result == 0) {
          result = EOF;
          break;
        } else if (result != 0) {
          break;
        }
      }
    } else if (str[j] != format[i] && format[i] != '\0') {
      if (str[j] == '\0') {
        if (result == 0) {
          result = EOF;
          break;
        } else if (result != 0) {
          break;
        }
      }
      break;
    } else if (str[j] == format[i] && format[i] != '\0') {
      j++;
    }
  }
  va_end(args);
  return result;
}

int s21_space_skip(const char* format, const char* str, int* i, int* j) {
  if (format[*i] == ' ' || format[*i] == '\n' || format[*i] == '\t' ||
      format[*i] == '\v' || format[*i] == '\r' || format[*i] == '\f') {
    while (format[*i] == ' ' || format[*i] == '\n' || format[*i] == '\t' ||
           format[*i] == '\v' || format[*i] == '\r' || format[*i] == '\f') {
      (*i)++;
    }
    while (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t' ||
           str[*j] == '\v' || str[*j] == '\r' || str[*j] == '\f') {
      (*j)++;
    }
    if (format[*i] == '\0') {
      return 1;
    }
  }
  return 0;
}

int s21_specification_analysis(const char* format, int* i, char* read_spec,
                               char* read_mod, int* width, int* was_star) {
  *width = 0;
  *was_star = 0;
  *read_mod = '0';
  (*i)++;
  if (format[*i] == '\0') {
    return 1;
  }
  if (format[*i] != '\0') {
    if (format[*i] == '*') {
      *was_star = 1;
      (*i)++;
    }
    while (format[*i] <= '9' && format[*i] >= '0') {
      *width = *width * 10 + format[*i] - '0';
      (*i)++;
    }
    if (format[*i] == 'h') {
      *read_mod = 'h';
      (*i)++;
    } else if (format[*i] == 'l') {
      *read_mod = 'l';
      (*i)++;
    } else if (format[*i] == 'L') {
      *read_mod = 'L';
      (*i)++;
    }
    if (format[*i] == 'd') {
      *read_spec = 'd';
    } else if (format[*i] == 'c') {
      *read_spec = 'c';
    } else if (format[*i] == 's') {
      *read_spec = 's';
    } else if (format[*i] == 'u') {
      *read_spec = 'u';
    } else if (format[*i] == 'o') {
      *read_spec = 'o';
    } else if (format[*i] == 'X' || format[*i] == 'x') {
      *read_spec = 'x';
    } else if (format[*i] == 'i') {
      *read_spec = 'i';
    } else if (format[*i] == '%') {
      *read_spec = '%';
    } else if (format[*i] == 'n') {
      *read_spec = 'n';
    } else if (format[*i] == 'p') {
      *read_spec = 'p';
    } else if (format[*i] == 'f') {
      *read_spec = 'f';
    } else if (format[*i] == 'e' || format[*i] == 'E') {
      *read_spec = 'f';
    } else if (format[*i] == 'g' || format[*i] == 'G') {
      *read_spec = 'f';
    } else {
      return 1;
    }
  }
  return 0;
}

int s21_all_read_func(va_list* args, int* j, const char* str, char read_spec,
                      char read_mod, int width, int was_star, int* was_percent,
                      int* was_n_spec) {
  int read_d_res;
  int read_c_res;
  int read_s_res;
  int read_u_res;
  int read_o_res;
  int o_abs = 0;
  int read_x_res;
  int x_abs = 0;
  int read_i_res;
  int read_p_res;
  int read_f_res;
  *was_percent = 0;
  *was_n_spec = 0;
  if (read_spec == 'd') {
    void* address = S21_NULL;
    if (was_star == 1) {
      address = S21_NULL;
    } else if (was_star == 0) {
      if (read_mod == 'l') {
        address = va_arg(*args, long*);
      } else if (read_mod == 'h') {
        address = va_arg(*args, short*);
      } else if (read_mod == '0') {
        address = va_arg(*args, int*);
      } else {
        return 1;
      }
    }
    read_d_res = s21_read_d(str, address, j, width, was_star, read_mod);
    if (read_d_res == 1) {
      return 1;
    } else if (read_d_res == 0) {
      return 0;
    } else if (read_d_res == -1) {
      return -1;
    }
  } else if (read_spec == 'c') {
    void* address = S21_NULL;
    if (was_star == 1) {
      address = S21_NULL;
    } else if (was_star == 0) {
      address = va_arg(*args, char*);
    }
    read_c_res = s21_read_c(str, address, j, width, was_star);
    if (read_c_res == 0) {
      return 0;
    } else if (read_c_res == 1) {
      return 1;
    } else if (read_c_res == -1) {
      return -1;
    }
  } else if (read_spec == 's') {
    void* address = S21_NULL;
    if (was_star == 1) {
      address = S21_NULL;
    } else if (was_star == 0) {
      address = va_arg(*args, char*);
    }
    read_s_res = s21_read_s(str, address, j, width, was_star);
    if (read_s_res == 0) {
      return 0;
    } else if (read_s_res == 1) {
      return 1;
    } else if (read_s_res == -1) {
      return -1;
    }
  } else if (read_spec == 'u') {
    void* address = S21_NULL;
    if (was_star == 1) {
      address = S21_NULL;
    } else if (was_star == 0) {
      if (read_mod == 'l') {
        address = va_arg(*args, unsigned long*);
      } else if (read_mod == 'h') {
        address = va_arg(*args, unsigned short*);
      } else if (read_mod == '0') {
        address = va_arg(*args, unsigned int*);
      } else {
        return 1;
      }
    }
    read_u_res = s21_read_u(str, address, j, width, was_star, read_mod);
    if (read_u_res == 0) {
      return 0;
    } else if (read_u_res == 1) {
      return 1;
    } else if (read_u_res == -1) {
      return -1;
    }
  } else if (read_spec == 'o') {
    void* address = S21_NULL;
    if (was_star == 1) {
      address = S21_NULL;
    } else if (was_star == 0) {
      if (read_mod == 'l') {
        address = va_arg(*args, unsigned long*);
      } else if (read_mod == 'h') {
        address = va_arg(*args, unsigned short*);
      } else if (read_mod == '0') {
        address = va_arg(*args, unsigned int*);
      } else {
        return 1;
      }
    }
    read_o_res = s21_read_o(str, address, j, width, was_star, o_abs, read_mod);
    if (read_o_res == 1) {
      return 1;
    } else if (read_o_res == 0) {
      return 0;
    } else if (read_o_res == -1) {
      return -1;
    }
  } else if (read_spec == 'x') {
    void* address = S21_NULL;
    if (was_star == 1) {
      address = S21_NULL;
    } else if (was_star == 0) {
      if (read_mod == 'l') {
        address = va_arg(*args, unsigned long*);
      } else if (read_mod == 'h') {
        address = va_arg(*args, unsigned short*);
      } else if (read_mod == '0') {
        address = va_arg(*args, unsigned int*);
      } else {
        return 1;
      }
    }
    read_x_res = s21_read_x(str, address, j, width, was_star, x_abs, read_mod);
    if (read_x_res == 0) {
      return 0;
    } else if (read_x_res == 1) {
      return 1;
    } else if (read_x_res == -1) {
      return -1;
    }
  } else if (read_spec == 'i') {
    void* address = S21_NULL;
    if (was_star == 1) {
      address = S21_NULL;
    } else if (was_star == 0) {
      if (read_mod == 'l') {
        address = va_arg(*args, long*);
      } else if (read_mod == 'h') {
        address = va_arg(*args, short*);
      } else if (read_mod == '0') {
        address = va_arg(*args, int*);
      } else {
        return 1;
      }
    }
    read_i_res = s21_read_i(str, address, j, width, was_star, read_mod);
    if (read_i_res == 1) {
      return 1;
    } else if (read_i_res == 0) {
      return 0;
    } else if (read_i_res == -1) {
      return -1;
    }
  } else if (read_spec == '%') {
    while (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t' ||
           str[*j] == '\v' || str[*j] == '\r' || str[*j] == '\f') {
      (*j)++;
    }
    if (str[*j] == '\0') {
      return -1;
    }
    for (; str[*j] != '\0'; (*j)++) {
      if (str[*j] == '%') {
        (*j)++;
        *was_percent = 1;
        return 0;
      } else {
        return 1;
      }
    }
  } else if (read_spec == 'n') {
    if (read_mod == 'l') {
      long* address = va_arg(*args, long*);
      *address = *j;
    } else if (read_mod == 'h') {
      short* address = va_arg(*args, short*);
      *address = *j;
    } else if (read_mod == '0') {
      int* address = va_arg(*args, int*);
      *address = *j;
    }
    *was_n_spec = 1;
    return 0;
  } else if (read_spec == 'p') {
    void** address = S21_NULL;
    if (was_star == 1) {
      address = S21_NULL;
    } else if (was_star == 0) {
      address = va_arg(*args, void**);
    }
    read_p_res = s21_read_p(str, address, j, width, was_star);
    if (read_p_res == 1) {
      return 1;
    } else if (read_p_res == 0) {
      return 0;
    } else if (read_p_res == -1) {
      return -1;
    }
  } else if (read_spec == 'f') {
    void* address = S21_NULL;
    if (was_star == 1) {
      address = S21_NULL;
    } else if (was_star == 0) {
      if (read_mod == 'l') {
        address = va_arg(*args, double*);
      } else if (read_mod == 'L') {
        address = va_arg(*args, long double*);
      } else if (read_mod == '0') {
        address = va_arg(*args, float*);
      } else {
        return 1;
      }
    }
    read_f_res = s21_read_f(str, address, j, width, was_star, read_mod);
    if (read_f_res == 1) {
      return 1;
    } else if (read_f_res == 0) {
      return 0;
    } else if (read_f_res == -1) {
      return -1;
    }
  }
  return 1;
}

int s21_read_d(const char* str, void* address, int* j, int width, int was_star,
               char read_mod) {
  long long result_char_d = 0;
  int otr_numb = 0;
  int was_number = 0;
  int was_znack = 0;
  int symb_count = 0;
  while (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t' ||
         str[*j] == '\v' || str[*j] == '\r' || str[*j] == '\f') {
    (*j)++;
  }
  if (str[*j] == '\0') {
    return -1;
  }
  for (; str[*j] != '\0'; (*j)++) {
    if (width != 0) {
      if (symb_count == width) {
        break;
      }
    }
    if (str[*j] == ' ') {
      break;
    } else if (str[*j] == '+' && was_number == 0 && was_znack == 0) {
      was_znack = 1;
      symb_count += 1;
    } else if (str[*j] == '-' && otr_numb == 0 && was_number == 0 &&
               was_znack == 0) {
      otr_numb = 1;
      was_znack = 1;
      symb_count += 1;
    } else if (str[*j] < '0' || str[*j] > '9') {
      break;
    } else if (str[*j] >= '0' && str[*j] <= '9') {
      if (otr_numb == 1) {
        result_char_d = result_char_d * 10 + ('0' - str[*j]);
        was_number = 1;
        symb_count += 1;
      } else {
        result_char_d = result_char_d * 10 + (str[*j] - '0');
        was_number = 1;
        symb_count += 1;
      }
    }
  }
  if (was_number == 1) {
    if (was_star == 0) {
      if (read_mod == 'l') {
        *(long*)address = (long)result_char_d;
      } else if (read_mod == 'h') {
        *(short*)address = (short)result_char_d;
      } else if (read_mod == '0') {
        *(int*)address = (int)result_char_d;
      }
      return 0;
    } else if (was_star == 1) {
      return 0;
    }
  } else {
    return 1;
  }
  return 1;
}

int s21_read_c(const char* str, char* address, int* j, int width,
               int was_star) {
  int symb_count = 0;
  int ind_pos = 0;
  if (str[*j] == '\0') {
    return -1;
  } else if (width != 0) {
    for (; str[*j] != '\0'; (*j)++) {
      if (symb_count == width) {
        break;
      }
      if (was_star == 0) {
        address[ind_pos] = str[*j];
        ind_pos++;
        symb_count += 1;
      } else if (was_star == 1) {
        symb_count += 1;
      }
    }
    return 0;
  } else {
    if (was_star == 0) {
      *address = str[*j];
      (*j)++;
      return 0;
    } else if (was_star == 1) {
      (*j)++;
      return 0;
    }
  }
  return 1;
}

int s21_read_s(const char* str, char* address, int* j, int width,
               int was_star) {
  int was_string = 0;
  int pos_ind = 0;
  int symb_count = 0;
  while (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t' ||
         str[*j] == '\v' || str[*j] == '\r' || str[*j] == '\f') {
    (*j)++;
  }
  if (str[*j] == '\0') {
    return -1;
  }
  for (; str[*j] != '\0'; (*j)++) {
    if (width != 0) {
      if (symb_count == width) {
        break;
      }
    }
    if (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t' ||
        str[*j] == '\v' || str[*j] == '\r' || str[*j] == '\f') {
      break;
    } else {
      if (was_star == 1) {
        was_string = 1;
        symb_count += 1;
      } else if (was_star == 0) {
        address[pos_ind] = str[*j];
        was_string = 1;
        pos_ind++;
        symb_count += 1;
      }
    }
  }
  if (was_string == 1 && was_star == 0) {
    address[pos_ind] = '\0';
    return 0;
  } else if (was_string == 1 && was_star == 1) {
    return 0;
  } else if (was_string == 0) {
    return 1;
  }
  return 1;
}

int s21_read_u(const char* str, void* address, int* j, int width, int was_star,
               char read_mod) {
  unsigned long long result_char_u = 0;
  int otr_numb = 0;
  int was_number = 0;
  int was_znack = 0;
  int symb_count = 0;
  while (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t' ||
         str[*j] == '\v' || str[*j] == '\r' || str[*j] == '\f') {
    (*j)++;
  }
  if (str[*j] == '\0') {
    return -1;
  }
  for (; str[*j] != '\0'; (*j)++) {
    if (width != 0) {
      if (symb_count == width) {
        break;
      }
    }
    if (str[*j] == '\0') {
      break;
    } else if (str[*j] >= '0' && str[*j] <= '9') {
      result_char_u = result_char_u * 10 + (str[*j] - '0');
      was_number = 1;
      symb_count += 1;
    } else if ((str[*j] == '-' || str[*j] == '+') && was_number == 0 &&
               was_znack == 0) {
      if (str[*j] == '-') {
        otr_numb = 1;
      }
      was_znack = 1;
      symb_count += 1;
    } else if (str[*j] < '0' || str[*j] > '9') {
      break;
    }
  }
  if (was_number == 1) {
    if (otr_numb == 1) {
      result_char_u = result_char_u * -1;
    }
    if (was_star == 1) {
      return 0;
    } else if (was_star == 0) {
      if (read_mod == 'l') {
        *(unsigned long*)address = (unsigned long)result_char_u;
      } else if (read_mod == 'h') {
        *(unsigned short*)address = (unsigned short)result_char_u;
      } else if (read_mod == '0') {
        *(unsigned int*)address = (unsigned int)result_char_u;
      }
      return 0;
    }
  } else {
    return 1;
  }
  return 1;
}

int s21_read_o(const char* str, void* address, int* j, int width, int was_star,
               int o_abs, char read_mod) {
  unsigned long long result_char_o = 0;
  int otr_numb = 0;
  int was_number = 0;
  int was_znack = 0;
  int symb_count = 0;
  while (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t' ||
         str[*j] == '\v' || str[*j] == '\r' || str[*j] == '\f') {
    (*j)++;
  }
  if (str[*j] == '\0') {
    return -1;
  }
  for (; str[*j] != '\0'; (*j)++) {
    if (width != 0) {
      if (symb_count == width) {
        break;
      }
    }
    if (str[*j] == '\0') {
      break;
    } else if (str[*j] >= '0' && str[*j] <= '7') {
      result_char_o = result_char_o * 8 + (str[*j] - '0');
      was_number = 1;
      symb_count += 1;
    } else if ((str[*j] == '-' || str[*j] == '+') && was_number == 0 &&
               was_znack == 0) {
      if (str[*j] == '-') {
        otr_numb = 1;
      }
      was_znack = 1;
      symb_count += 1;
    } else if (str[*j] < '0' || str[*j] > '7') {
      break;
    }
  }
  if (was_number == 1) {
    if (otr_numb == 1 && o_abs == 0) {
      result_char_o = result_char_o * -1;
    }
    if (was_star == 1) {
      return 0;
    } else if (was_star == 0) {
      if (read_mod == 'l') {
        *(unsigned long*)address = (unsigned long)result_char_o;
      } else if (read_mod == 'h') {
        *(unsigned short*)address = (unsigned short)result_char_o;
      } else if (read_mod == '0') {
        *(unsigned int*)address = (unsigned int)result_char_o;
      }
      return 0;
    }
  } else {
    return 1;
  }
  return 1;
}

int s21_read_x(const char* str, void* address, int* j, int width, int was_star,
               int x_abs, char read_mod) {
  unsigned long long result_char_x = 0;
  int otr_numb = 0;
  int was_number = 0;
  int was_znack = 0;
  int symb_count = 0;
  int was_prefix = 0;
  while (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t' ||
         str[*j] == '\v' || str[*j] == '\r' || str[*j] == '\f') {
    (*j)++;
  }
  if (str[*j] == '\0') {
    return -1;
  }
  for (; str[*j] != '\0'; (*j)++) {
    if (width != 0) {
      if (symb_count == width) {
        break;
      }
    }
    if (str[*j] == '\0') {
      break;
    } else if (str[*j] == '0' && str[*j + 1] != '\0' &&
               (str[*j + 1] == 'x' || str[*j + 1] == 'X') && was_prefix == 0 &&
               was_number == 0) {
      if (width != 0) {
        if (width - symb_count >= 2) {
          symb_count += 2;
          (*j)++;
          was_prefix = 1;
        } else if (width - symb_count == 1 && width > 0) {
          symb_count += 1;
          result_char_x = result_char_x * 16;
          was_number = 1;
        }
      } else if (width == 0) {
        symb_count += 2;
        (*j)++;
        was_prefix = 1;
      }
    } else if (str[*j] >= '0' && str[*j] <= '9') {
      result_char_x = result_char_x * 16 + (str[*j] - '0');
      was_number = 1;
      symb_count += 1;
    } else if (str[*j] >= 'a' && str[*j] <= 'f') {
      result_char_x = result_char_x * 16 + (str[*j] - 'a' + 10);
      was_number = 1;
      symb_count += 1;
    } else if (str[*j] >= 'A' && str[*j] <= 'F') {
      result_char_x = result_char_x * 16 + (str[*j] - 'A' + 10);
      was_number = 1;
      symb_count += 1;
    } else if ((str[*j] == '-' || str[*j] == '+') && was_number == 0 &&
               was_znack == 0 && was_prefix == 0) {
      if (str[*j] == '-') {
        otr_numb = 1;
      }
      was_znack = 1;
      symb_count += 1;
    } else {
      break;
    }
  }
  if (was_number == 1) {
    if (otr_numb == 1 && x_abs == 0) {
      result_char_x = result_char_x * -1;
    }
    if (was_star == 1) {
      return 0;
    } else if (was_star == 0) {
      if (read_mod == 'l') {
        *(unsigned long*)address = (unsigned long)result_char_x;
      } else if (read_mod == 'h') {
        *(unsigned short*)address = (unsigned short)result_char_x;
      } else if (read_mod == '0') {
        *(unsigned int*)address = (unsigned int)result_char_x;
      }
      return 0;
    }
  } else {
    return 1;
  }
  return 1;
}

int s21_read_i(const char* str, void* address, int* j, int width, int was_star,
               char read_mod) {
  int res_hex = 0;
  int res_oct = 0;
  int res_des = 0;
  int is_otr = 0;
  int o_abs = 0;
  int x_abs = 0;
  unsigned long read_hex_oct;
  int obr_i_zn_res = 0;
  int obr_i_nozn_res = 0;
  while (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t' ||
         str[*j] == '\v' || str[*j] == '\r' || str[*j] == '\f') {
    (*j)++;
  }
  if (str[*j] == '\0') {
    return -1;
  }
  if (str[*j] == '+' || str[*j] == '-') {
    if (str[*j] == '-') {
      is_otr = 1;
      o_abs = 1;
      x_abs = 1;
    }
    if (str[*j + 1] == '0' && (str[*j + 2] == 'x' || str[*j + 2] == 'X') &&
        str[*j + 1] != '\0' && str[*j + 2] != '\0') {
      res_hex = s21_read_x(str, &read_hex_oct, j, width, was_star, x_abs, 'l');
      if (res_hex == 1) {
        return 1;
      } else if (res_hex == 0) {
        if (was_star == 1) {
          return 0;
        } else if (was_star == 0) {
          obr_i_zn_res =
              (s21_obr_i_zn(read_mod, is_otr, read_hex_oct, address));
          if (obr_i_zn_res == 1) {
            return 1;
          } else if (obr_i_zn_res == 0) {
            return 0;
          }
        }
      }
    } else if (str[*j + 1] == '0') {
      res_oct = s21_read_o(str, &read_hex_oct, j, width, was_star, o_abs, 'l');
      if (res_oct == 1) {
        return 1;
      } else if (res_oct == 0) {
        if (was_star == 1) {
          return 0;
        } else if (was_star == 0) {
          obr_i_zn_res =
              (s21_obr_i_zn(read_mod, is_otr, read_hex_oct, address));
          if (obr_i_zn_res == 1) {
            return 1;
          } else if (obr_i_zn_res == 0) {
            return 0;
          }
        }
      }
    } else {
      res_des = s21_read_d(str, address, j, width, was_star, read_mod);
      if (res_des == 1) {
        return 1;
      } else if (res_des == 0) {
        return 0;
      }
    }
  } else {
    if (str[*j] == '0' && (str[*j + 1] == 'x' || str[*j + 1] == 'X') &&
        str[*j + 1] != '\0') {
      res_hex = s21_read_x(str, &read_hex_oct, j, width, was_star, x_abs, 'l');
      if (res_hex == 1) {
        return 1;
      } else if (res_hex == 0) {
        if (was_star == 1) {
          return 0;
        } else if (was_star == 0) {
          obr_i_nozn_res = (s21_obr_i_nozn(read_mod, read_hex_oct, address));
          if (obr_i_nozn_res == 1) {
            return 1;
          } else if (obr_i_nozn_res == 0) {
            return 0;
          }
        }
      }
    } else if (str[*j] == '0') {
      res_oct = s21_read_o(str, &read_hex_oct, j, width, was_star, o_abs, 'l');
      if (res_oct == 1) {
        return 1;
      } else if (res_oct == 0) {
        if (was_star == 1) {
          return 0;
        } else if (was_star == 0) {
          obr_i_nozn_res = (s21_obr_i_nozn(read_mod, read_hex_oct, address));
          if (obr_i_nozn_res == 1) {
            return 1;
          } else if (obr_i_nozn_res == 0) {
            return 0;
          }
        }
      }
    } else {
      res_des = s21_read_d(str, address, j, width, was_star, read_mod);
      if (res_des == 1) {
        return 1;
      } else if (res_des == 0) {
        return 0;
      }
    }
  }
  return 1;
}

int s21_obr_i_zn(char read_mod, int is_otr, unsigned long read_hex_oct,
                 void* address) {
  if (read_mod == 'h') {
    if (is_otr == 1 && read_hex_oct <= SHRT_MAX) {
      *(short*)address = -(short)read_hex_oct;
      return 0;
    } else if (is_otr == 1 && read_hex_oct == 0 - (unsigned int)SHRT_MIN) {
      *(short*)address = SHRT_MIN;
      return 0;
    } else if (is_otr == 0 && read_hex_oct <= SHRT_MAX) {
      *(short*)address = (short)read_hex_oct;
      return 0;
    }
  } else if (read_mod == 'l') {
    if (is_otr == 1 && read_hex_oct <= LONG_MAX) {
      *(long*)address = -(long)read_hex_oct;
      return 0;
    } else if (is_otr == 1 && read_hex_oct == 0 - (unsigned long)LONG_MIN) {
      *(long*)address = LONG_MIN;
      return 0;
    } else if (is_otr == 0 && read_hex_oct <= LONG_MAX) {
      *(long*)address = (long)read_hex_oct;
      return 0;
    }
  } else if (read_mod == '0') {
    if (is_otr == 1 && read_hex_oct <= INT_MAX) {
      *(int*)address = -(int)read_hex_oct;
      return 0;
    } else if (is_otr == 1 && read_hex_oct == 0 - (unsigned int)INT_MIN) {
      *(int*)address = INT_MIN;
      return 0;
    } else if (is_otr == 0 && read_hex_oct <= INT_MAX) {
      *(int*)address = (int)read_hex_oct;
      return 0;
    }
  }
  return 1;
}

int s21_obr_i_nozn(char read_mod, unsigned long read_hex_oct, void* address) {
  if (read_mod == '0') {
    if (read_hex_oct <= INT_MAX) {
      *(int*)address = (int)read_hex_oct;
      return 0;
    }
  } else if (read_mod == 'h') {
    if (read_hex_oct <= SHRT_MAX) {
      *(short*)address = (short)read_hex_oct;
      return 0;
    }
  } else if (read_mod == 'l') {
    if (read_hex_oct <= LONG_MAX) {
      *(long*)address = (long)read_hex_oct;
      return 0;
    }
  }
  return 1;
}

int s21_read_p(const char* str, void** address, int* j, int width,
               int was_star) {
  unsigned long result_char_p = 0;
  int otr_numb = 0;
  int was_number = 0;
  int was_znack = 0;
  int symb_count = 0;
  int was_prefix = 0;
  while (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t' ||
         str[*j] == '\v' || str[*j] == '\r' || str[*j] == '\f') {
    (*j)++;
  }
  if (str[*j] == '\0') {
    return -1;
  }
  for (; str[*j] != '\0'; (*j)++) {
    if (width != 0) {
      if (symb_count == width) {
        break;
      }
    }
    if (str[*j] == '\0') {
      break;
    } else if (str[*j] == '0' && str[*j + 1] != '\0' &&
               (str[*j + 1] == 'x' || str[*j + 1] == 'X') && was_prefix == 0 &&
               was_number == 0) {
      if (width != 0) {
        if (width - symb_count >= 2) {
          symb_count += 2;
          (*j)++;
          was_prefix = 1;
        } else if (width - symb_count == 1 && width > 0) {
          symb_count += 1;
          result_char_p = result_char_p * 16;
          was_number = 1;
        }
      } else if (width == 0) {
        symb_count += 2;
        (*j)++;
        was_prefix = 1;
      }
    } else if (str[*j] >= '0' && str[*j] <= '9') {
      result_char_p = result_char_p * 16 + str[*j] - '0';
      was_number = 1;
      symb_count += 1;
    } else if (str[*j] >= 'a' && str[*j] <= 'f') {
      result_char_p = result_char_p * 16 + str[*j] - 'a' + 10;
      was_number = 1;
      symb_count += 1;
    } else if (str[*j] >= 'A' && str[*j] <= 'F') {
      result_char_p = result_char_p * 16 + str[*j] - 'A' + 10;
      was_number = 1;
      symb_count += 1;
    } else if ((str[*j] == '-' || str[*j] == '+') && was_number == 0 &&
               was_znack == 0 && was_prefix == 0) {
      if (str[*j] == '-') {
        otr_numb = 1;
      }
      was_znack = 1;
      symb_count += 1;
    } else {
      break;
    }
  }
  if (was_number == 1) {
    if (otr_numb == 1) {
      result_char_p = result_char_p * -1;
    }
    if (was_star == 1) {
      return 0;
    } else if (was_star == 0) {
      *address = (void*)result_char_p;
      return 0;
    }
  } else {
    return 1;
  }
  return 1;
}

int s21_read_f(const char* str, void* address, int* j, int width, int was_star,
               char read_mod) {
  long double result_char_f = 0;
  int was_numb = 0;
  int symb_count = 0;
  int was_zn = 0;
  int otr_numb = 0;
  int dot_count = 0;
  long double numb_fl_culc = 10.0;
  int run = 1;
  int was_exp = 0;
  int exp_otr_numb = 0;
  int was_exp_zn = 0;
  int was_numb_e = 0;
  int temp_num_e = 0;
  while (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t' ||
         str[*j] == '\v' || str[*j] == '\r' || str[*j] == '\f') {
    (*j)++;
  }
  if (str[*j] == '\0') {
    return -1;
  }
  while (run == 1) {
    if (str[*j] == '.' && dot_count == 1) {
      break;
    }
    if ((str[*j] == 'e' || str[*j] == 'E') && was_exp == 1) {
      break;
    }
    if (width != 0) {
      if (symb_count == width) {
        break;
      }
    }
    if (str[*j] == '\0') {
      break;
    } else if (str[*j] == '+' && was_numb == 0 && was_zn == 0 &&
               dot_count == 0 && was_exp == 0) {
      was_zn = 1;
      symb_count += 1;
    } else if (str[*j] == '-' && otr_numb == 0 && was_numb == 0 &&
               was_zn == 0 && dot_count == 0 && was_exp == 0) {
      otr_numb = 1;
      was_zn = 1;
      symb_count += 1;
    } else if (str[*j] == '.' && was_exp == 0) {
      dot_count += 1;
      symb_count += 1;
    } else if (str[*j] >= '0' && str[*j] <= '9' && dot_count == 0 &&
               was_exp == 0) {
      if (otr_numb == 1) {
        result_char_f = result_char_f * 10 + ('0' - str[*j]);
        was_numb = 1;
        symb_count += 1;
      } else {
        result_char_f = result_char_f * 10 + (str[*j] - '0');
        was_numb = 1;
        symb_count += 1;
      }
    } else if (str[*j] >= '0' && str[*j] <= '9' && dot_count == 1 &&
               was_exp == 0) {
      if (otr_numb == 1) {
        result_char_f = result_char_f + ('0' - str[*j]) / numb_fl_culc;
        numb_fl_culc *= 10;
        was_numb = 1;
        symb_count += 1;
      } else {
        result_char_f = result_char_f + (str[*j] - '0') / numb_fl_culc;
        numb_fl_culc *= 10;
        was_numb = 1;
        symb_count += 1;
      }
    } else if ((str[*j] == 'e' || str[*j] == 'E') && was_numb == 1) {
      symb_count += 1;
      was_exp = 1;
    } else if (str[*j] == '+' && was_exp_zn == 0 && was_exp == 1 &&
               (str[*j - 1] == 'e' || str[*j - 1] == 'E')) {
      was_exp_zn = 1;
      symb_count += 1;
    } else if (str[*j] == '-' && exp_otr_numb == 0 && was_exp_zn == 0 &&
               was_exp == 1 && (str[*j - 1] == 'e' || str[*j - 1] == 'E')) {
      exp_otr_numb = 1;
      was_exp_zn = 1;
      symb_count += 1;
    } else if (str[*j] >= '0' && str[*j] <= '9' && was_exp == 1) {
      temp_num_e = temp_num_e * 10 + (str[*j] - '0');
      was_numb_e = 1;
      symb_count += 1;
    } else if (str[*j] < '0' || str[*j] > '9') {
      break;
    }
    (*j)++;
  }
  if (was_numb == 1 && was_exp == 0) {
    if (result_char_f == 0.0 && otr_numb == 1) {
      result_char_f = result_char_f * -1;
    }
    if (was_star == 1) {
      return 0;
    } else if (was_star == 0) {
      if (read_mod == 'L') {
        *(long double*)address = (long double)result_char_f;
      } else if (read_mod == 'l') {
        *(double*)address = (double)result_char_f;
      } else if (read_mod == '0') {
        *(float*)address = (float)result_char_f;
      }
      return 0;
    }
  } else if (was_numb == 1 && was_exp == 1 && was_numb_e == 1) {
    if (was_star == 1) {
      return 0;
    } else if (was_star == 0) {
      if (otr_numb == 1 && result_char_f == 0.0) {
        result_char_f = result_char_f * -1;
      }
      if (exp_otr_numb == 1) {
        result_char_f = result_char_f * s21_exp_pow(temp_num_e, 1);
      } else {
        result_char_f = result_char_f * s21_exp_pow(temp_num_e, 0);
      }
      if (read_mod == 'L') {
        *(long double*)address = (long double)result_char_f;
      } else if (read_mod == 'l') {
        *(double*)address = (double)result_char_f;
      } else if (read_mod == '0') {
        *(float*)address = (float)result_char_f;
      }
      return 0;
    }
  }
  return 1;
}

long double s21_exp_pow(int st, int exp_otr_numb) {
  long double num = 1.0;
  for (int i = 0; i < st; i++) {
    num = num * 10.0;
  }
  if (exp_otr_numb == 1) {
    return 1.0 / num;
  }
  return num;
}
