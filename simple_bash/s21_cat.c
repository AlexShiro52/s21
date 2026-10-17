#include <stdio.h>
#include <string.h>

typedef struct {
  int b;
  int n;
  int s;
  int E;
  int T;
  int v;
} flags;

int parse_flags(int argc, char* argv[], flags* my_flags);

void readfile(FILE* file, flags my_flags, int* line_number, int* line_start,
              int* empty_lines);

int main(int argc, char* argv[]) {
  flags my_flags = {0, 0, 0, 0, 0, 0};
  int exit_status = 0;
  int line_number = 1;
  int line_start = 1;
  int empty_lines = 0;
  int file_c = parse_flags(argc, argv, &my_flags);
  if (file_c == -1) {
    return 1;
  }
  if (file_c == 0) {
    FILE* file = stdin;
    readfile(file, my_flags, &line_number, &line_start, &empty_lines);
  } else if (file_c > 0) {
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
        continue;
      }
      char* filename = argv[i];
      FILE* file = fopen(filename, "r");
      if (file == NULL) {
        fprintf(stderr, "файла %s не существует\n", filename);
        exit_status = 1;
        continue;
      }
      readfile(file, my_flags, &line_number, &line_start, &empty_lines);
      fclose(file);
    }
  }
  return exit_status;
}

void readfile(FILE* file, flags my_flags, int* line_number, int* line_start,
              int* empty_lines) {
  int symb = fgetc(file);
  int flagdiap = 0;
  if (my_flags.b == 1) {
    my_flags.n = 0;
  }
  while (symb != EOF) {
    if (*line_start == 1 && symb == '\n') {
      *empty_lines += 1;
    }
    if (my_flags.s == 1 && *empty_lines > 1) {
      if (symb != '\n') {
        *empty_lines = 0;
      }
      if (symb == '\n') {
        symb = fgetc(file);
        continue;
      }
    }
    if (my_flags.b == 1 && *line_start == 1 && symb != '\n') {
      printf("%6d\t", *line_number);
      *line_number += 1;
      *line_start = 0;
    }
    if (my_flags.n == 1 && *line_start == 1) {
      printf("%6d\t", *line_number);
      *line_number += 1;
      *line_start = 0;
    }
    if (my_flags.E == 1 && symb == '\n') {
      fputc('$', stdout);
    }
    if (symb == '\n') {
      *line_start = 1;
    }
    if (symb != '\n') {
      *line_start = 0;
      *empty_lines = 0;
    }
    if (my_flags.v == 1 && symb >= 128) {
      fputc('M', stdout);
      fputc('-', stdout);
      symb = symb - 128;
      flagdiap = 1;
    }
    if (my_flags.T == 1 && symb == '\t') {
      fputc('^', stdout);
      fputc('I', stdout);
    } else if (my_flags.v == 1 && symb == 127) {
      fputc('^', stdout);
      fputc('?', stdout);
    } else if (my_flags.v == 1 && symb >= 0 && symb < 32 && flagdiap == 1) {
      fputc('^', stdout);
      symb = symb + '@';
      fputc(symb, stdout);
    } else if (my_flags.v == 1 && symb >= 0 && symb < 32 && symb != 10 &&
               symb != 9 && flagdiap == 0) {
      fputc('^', stdout);
      symb = symb + '@';
      fputc(symb, stdout);
    } else {
      fputc(symb, stdout);
    }
    symb = fgetc(file);
    flagdiap = 0;
  }
}

int parse_flags(int argc, char* argv[], flags* my_flags) {
  int file_c = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == '-') {
      if (strcmp(argv[i], "--number") == 0) {
        my_flags->n = 1;
      } else if (strcmp(argv[i], "--number-nonblank") == 0) {
        my_flags->b = 1;
      } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
        my_flags->s = 1;
      } else {
        fprintf(stderr, "error");
        return -1;
      }
    } else if (argv[i][0] == '-' && argv[i][1] != '-') {
      for (int j = 1; argv[i][j] != '\0'; j++) {
        if (argv[i][j] == 'n') {
          my_flags->n = 1;
        } else if (argv[i][j] == 'b') {
          my_flags->b = 1;
        } else if (argv[i][j] == 'e') {
          my_flags->E = 1;
          my_flags->v = 1;
        } else if (argv[i][j] == 's') {
          my_flags->s = 1;
        } else if (argv[i][j] == 't') {
          my_flags->T = 1;
          my_flags->v = 1;
        } else if (argv[i][j] == 'E') {
          my_flags->E = 1;
        } else if (argv[i][j] == 'T') {
          my_flags->T = 1;
        } else if (argv[i][j] == 'v') {
          my_flags->v = 1;
        } else {
          fprintf(stderr, "error");
          return -1;
        }
      }
    }
    if (argv[i][0] != '-') {
      file_c += 1;
    }
  }
  return file_c;
}
