#include <regex.h>
#include <stdio.h>

typedef struct {
  int i;
  int v;
  int n;
  int h;
  int c;
  int l;
  int s;
  int f;
  int o;
} Flags;

int process_files(int files_count, Flags flags, int patterns_count,
                  char* files[], regex_t* proc_string);

int parse_arg(int argc, char* argv[], int* patterns_count, char* patterns[],
              Flags* flags, char* files[], int* files_count,
              char buffer_f[][1024]);

int f_flag(char* f_filename, int* patterns_count, char buffer_f[][1024],
           char* patterns[]);

void prefix_n_output(int show_name, int line_numb, char* filename, Flags flags);

void best_match(int* coincidence, int* coincidence_start, int* coincidence_end,
                regoff_t match_start, regoff_t match_end);

int pattern_in_string(char* line_buffer, regex_t* proc_string,
                      int patterns_count, Flags flags);

int pattern_in_string_f(char* line_buffer, regex_t* proc_string,
                        int patterns_count, char* filename, int show_name,
                        int line_numb, Flags flags);

int processing_final_string(char* line_buffer, regex_t* proc_string,
                            int patterns_count, char* filename, int show_name,
                            int line_numb, Flags flags, int* lines_count,
                            int buff_ind);

int readfile(FILE* file, regex_t* proc_string, char* filename, int show_name,
             int patterns_count, Flags flags);

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "error");
    return 1;
  }
  Flags flags = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  char* patterns[1024];
  int patterns_count = 0;
  char* files[1024];
  int files_count = 0;
  char buffer_f[1024][1024];
  if (parse_arg(argc, argv, &patterns_count, patterns, &flags, files,
                &files_count, buffer_f) == 1) {
    return 1;
  }
  regex_t proc_string[1024];
  for (int i = 0; i < patterns_count; i++) {
    int setting = 0;
    if (flags.i == 1) {
      setting = REG_ICASE;
    }
    if (regcomp(&proc_string[i], patterns[i], setting) != 0) {
      fprintf(stderr, "error with proc");
      for (int j = 0; j < i; j++) {
        regfree(&proc_string[j]);
      }
      return 1;
    }
  }
  int exit_status =
      process_files(files_count, flags, patterns_count, files, proc_string);
  for (int i = 0; i < patterns_count; i++) {
    regfree(&proc_string[i]);
  }
  return exit_status;
}

int process_files(int files_count, Flags flags, int patterns_count,
                  char* files[], regex_t* proc_string) {
  int exit_status = 0;
  int show_name = 0;
  if (files_count > 1) {
    if (flags.h == 1) {
      show_name = 0;
    } else {
      show_name = 1;
    }
  }
  if (files_count == 0) {
    FILE* file = stdin;
    int res = readfile(file, proc_string, NULL, 0, patterns_count, flags);
    if (res == 1) {
      exit_status = 1;
    }
  } else {
    for (int i = 0; i < files_count; i++) {
      char* filename = files[i];
      FILE* file = fopen(filename, "r");
      if (file == NULL) {
        if (flags.s == 1) {
          exit_status = 1;
          continue;
        } else {
          fprintf(stderr, "error with file:  %s", filename);
          exit_status = 1;
          continue;
        }
      }
      int res = readfile(file, proc_string, filename, show_name, patterns_count,
                         flags);
      fclose(file);
      if (res == 1) {
        exit_status = 1;
      }
    }
  }
  return exit_status;
}

int parse_arg(int argc, char* argv[], int* patterns_count, char* patterns[],
              Flags* flags, char* files[], int* files_count,
              char buffer_f[][1024]) {
  int has_ef = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      for (int j = 1; argv[i][j] != '\0'; j++) {
        if ((argv[i][j] == 'e') || (argv[i][j] == 'f')) {
          has_ef = 1;
          if (argv[i][j + 1] != '\0') {
            break;
          } else if (argv[i][j + 1] == '\0') {
            if (i + 1 < argc) {
              i++;
              break;
            } else {
              fprintf(stderr, "no pattern");
              return 1;
            }
          }
        }
      }
    }
  }
  for (int j = 1; j < argc; j++) {
    if (argv[j][0] == '-' && argv[j][1] != '-') {
      for (int k = 1; argv[j][k] != '\0'; k++) {
        if (argv[j][k] == 'e') {
          if (argv[j][k + 1] != '\0') {
            if (*patterns_count > 1023) {
              fprintf(stderr, "no memmory for new pattern");
              return 1;
            } else {
              patterns[*patterns_count] = &argv[j][k + 1];
              *patterns_count += 1;
              break;
            }
          } else if (j + 1 < argc) {
            if (*patterns_count > 1023) {
              fprintf(stderr, "no memmory for new pattern");
              return 1;
            } else {
              patterns[*patterns_count] = argv[j + 1];
              *patterns_count += 1;
              j++;
              break;
            }
          } else {
            fprintf(stderr, " no pattern after -e");
            return 1;
          }
        } else if (argv[j][k] == 'f') {
          char* f_filename;
          if (argv[j][k + 1] != '\0') {
            f_filename = &argv[j][k + 1];
          } else if (j + 1 < argc) {
            f_filename = argv[j + 1];
            j++;
          } else {
            fprintf(stderr, "no file after -f");
            return 1;
          }
          if (f_flag(f_filename, patterns_count, buffer_f, patterns) == 1) {
            return 1;
          }
          break;
        } else if (argv[j][k] == 'i') {
          flags->i = 1;
        } else if (argv[j][k] == 'v') {
          flags->v = 1;
        } else if (argv[j][k] == 'n') {
          flags->n = 1;
        } else if (argv[j][k] == 'h') {
          flags->h = 1;
        } else if (argv[j][k] == 'c') {
          flags->c = 1;
        } else if (argv[j][k] == 'l') {
          flags->l = 1;
        } else if (argv[j][k] == 's') {
          flags->s = 1;
        } else if (argv[j][k] == 'o') {
          flags->o = 1;
        } else {
          fprintf(stderr, "wrong flag given");
          return 1;
        }
      }
    } else if (argv[j][0] != '-' && has_ef == 1) {
      if (*files_count > 1023) {
        fprintf(stderr, "no memmory for new file");
        return 1;
      } else {
        files[*files_count] = argv[j];
        *files_count += 1;
      }
    } else if (argv[j][0] != '-' && has_ef == 0 && *patterns_count == 0) {
      if (*patterns_count > 1023) {
        fprintf(stderr, "no memmory for new pattern");
        return 1;
      } else {
        patterns[*patterns_count] = argv[j];
        *patterns_count += 1;
      }
    } else if (argv[j][0] != '-' && has_ef == 0 && *patterns_count == 1) {
      if (*files_count > 1023) {
        fprintf(stderr, "no memmory for new file");
        return 1;
      } else {
        files[*files_count] = argv[j];
        *files_count += 1;
      }
    } else {
      fprintf(stderr, "wrong flag");
      return 1;
    }
  }
  if (*patterns_count == 0 && has_ef == 0) {
    fprintf(stderr, "no patterns given");
    return 1;
  }
  return 0;
}

int f_flag(char* f_filename, int* patterns_count, char buffer_f[][1024],
           char* patterns[]) {
  FILE* f_file = fopen(f_filename, "r");
  if (f_file == NULL) {
    fprintf(stderr, "error with f_file");
    return 1;
  }
  int symbol_f = fgetc(f_file);
  int buffer_f_ind = 0;
  while (symbol_f != EOF) {
    if (symbol_f != '\n') {
      if ((*patterns_count > 1023) || (buffer_f_ind > 1022)) {
        fclose(f_file);
        fprintf(stderr, "no memmory for mew symb");
        return 1;
      } else {
        buffer_f[*patterns_count][buffer_f_ind] = symbol_f;
        buffer_f_ind += 1;
      }
    } else if (symbol_f == '\n') {
      if (*patterns_count > 1023) {
        fclose(f_file);
        fprintf(stderr, "no memmory for mew symb");
        return 1;
      } else {
        buffer_f[*patterns_count][buffer_f_ind] = '\0';
        patterns[*patterns_count] = buffer_f[*patterns_count];
        *patterns_count += 1;
        buffer_f_ind = 0;
      }
    }
    symbol_f = fgetc(f_file);
  }
  if (ferror(f_file)) {
    fprintf(stderr, "error with patterns file");
    fclose(f_file);
    return 1;
  }
  if (buffer_f_ind > 0) {
    if (*patterns_count > 1023) {
      fclose(f_file);
      fprintf(stderr, "no memmory for mew symb");
      return 1;
    } else {
      buffer_f[*patterns_count][buffer_f_ind] = '\0';
      patterns[*patterns_count] = buffer_f[*patterns_count];
      *patterns_count += 1;
    }
  }
  fclose(f_file);
  return 0;
}

int pattern_in_string(char* line_buffer, regex_t* proc_string,
                      int patterns_count, Flags flags) {
  for (int i = 0; i < patterns_count; i++) {
    int result = regexec(&proc_string[i], line_buffer, 0, NULL, 0);
    if (result == 0) {
      if (flags.v == 1) {
        return -1;
      } else {
        return 1;
      }
    } else if (result != REG_NOMATCH && result != 0) {
      return -2;
    }
  }
  if (flags.v == 1) {
    return 1;
  } else {
    return -1;
  }
}

int pattern_in_string_f(char* line_buffer, regex_t* proc_string,
                        int patterns_count, char* filename, int show_name,
                        int line_numb, Flags flags) {
  regmatch_t match[1];
  int search_pos = 0;
  int result = 0;
  int coincidence = 0;
  int coincidence_start;
  int coincidence_end;
  while (1) {
    coincidence = 0;
    for (int i = 0; i < patterns_count; i++) {
      if (search_pos == 0) {
        result =
            regexec(&proc_string[i], &line_buffer[search_pos], 1, match, 0);
      }
      if (search_pos != 0) {
        result = regexec(&proc_string[i], &line_buffer[search_pos], 1, match,
                         REG_NOTBOL);
      }
      if (result == 0) {
        if ((match->rm_eo - match->rm_so == 0) &&
            line_buffer[search_pos + match->rm_eo] == '\0') {
          continue;
        } else {
          best_match(&coincidence, &coincidence_start, &coincidence_end,
                     match[0].rm_so, match[0].rm_eo);
        }
      } else if (result == REG_NOMATCH) {
        continue;
      } else {
        return 1;
      }
    }
    if (coincidence == 0) {
      break;
    }
    if (coincidence_end - coincidence_start != 0) {
      prefix_n_output(show_name, line_numb, filename, flags);
      for (int j = coincidence_start; j < coincidence_end; j++) {
        fprintf(stdout, "%c", line_buffer[j + search_pos]);
      }
      printf("\n");
      search_pos += coincidence_end;
    } else {
      search_pos += coincidence_end;
      search_pos += 1;
    }
  }
  return 0;
}

void best_match(int* coincidence, int* coincidence_start, int* coincidence_end,
                regoff_t match_start, regoff_t match_end) {
  if (*coincidence == 0) {
    *coincidence_start = match_start;
    *coincidence_end = match_end;
    *coincidence += 1;
  } else if (*coincidence > 0) {
    if (*coincidence_start > match_start) {
      *coincidence_start = match_start;
      *coincidence_end = match_end;
    } else if (*coincidence_start == match_start) {
      if ((*coincidence_end - *coincidence_start) < (match_end - match_start)) {
        *coincidence_start = match_start;
        *coincidence_end = match_end;
      }
    }
  }
}

int readfile(FILE* file, regex_t* proc_string, char* filename, int show_name,
             int patterns_count, Flags flags) {
  char line_buffer[1024];
  int buff_ind = 0;
  int line_numb = 1;
  int lines_count = 0;
  int symb = fgetc(file);
  while (symb != EOF) {
    if (symb != '\n') {
      if (buff_ind < 1023) {
        line_buffer[buff_ind] = symb;
        buff_ind += 1;
      } else {
        fprintf(stderr, "stack overflow");
        return 1;
      }
    } else {
      int res = processing_final_string(
          line_buffer, proc_string, patterns_count, filename, show_name,
          line_numb, flags, &lines_count, buff_ind);
      if (res == -1) {
        return 0;
      } else if (res == 1) {
        return 1;
      }
      line_numb += 1;
      buff_ind = 0;
    }
    symb = fgetc(file);
  }
  if (ferror(file)) {
    if (flags.s == 1) {
      return 1;
    } else {
      fprintf(stderr, "error with file");
      return 1;
    }
  }
  if (buff_ind > 0) {
    int res = processing_final_string(line_buffer, proc_string, patterns_count,
                                      filename, show_name, line_numb, flags,
                                      &lines_count, buff_ind);
    if (res == -1) {
      return 0;
    } else if (res == 1) {
      return 1;
    }
  }
  if (flags.c == 1 && flags.l == 0) {
    if (show_name == 1) {
      printf("%s:%d\n", filename, lines_count);
    } else if (show_name == 0) {
      printf("%d\n", lines_count);
    }
  }
  return 0;
}

int processing_final_string(char* line_buffer, regex_t* proc_string,
                            int patterns_count, char* filename, int show_name,
                            int line_numb, Flags flags, int* lines_count,
                            int buff_ind) {
  line_buffer[buff_ind] = '\0';
  int res = pattern_in_string(line_buffer, proc_string, patterns_count, flags);
  if (res == 1) {
    if (flags.l == 1 && filename != NULL) {
      printf("%s\n", filename);
      return -1;
    } else if (flags.l == 1 && filename == NULL) {
      printf("standard input\n");
      return -1;
    } else if (flags.c == 1) {
      *lines_count += 1;
    } else if (flags.o == 1) {
      if (pattern_in_string_f(line_buffer, proc_string, patterns_count,
                              filename, show_name, line_numb, flags) == 1) {
        fprintf(stderr, "error with flag o");
        return 1;
      }
    } else {
      prefix_n_output(show_name, line_numb, filename, flags);
      printf("%s\n", line_buffer);
    }
  } else if (res == -2) {
    fprintf(stderr, "error with pattern");
    return 1;
  }
  return 0;
}

void prefix_n_output(int show_name, int line_numb, char* filename,
                     Flags flags) {
  if (show_name == 0) {
    if (flags.n == 1) {
      printf("%d:", line_numb);
    }
  } else if (show_name == 1) {
    if (flags.n == 1) {
      printf("%s:%d:", filename, line_numb);
    } else {
      printf("%s:", filename);
    }
  }
}
