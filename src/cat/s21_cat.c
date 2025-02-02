#include "s21_cat.h"

int main(int argc, char* argv[]) {
  flags flags_s = {0};

  if (argc > 1) {
    if (parsing_options(argc, argv, &flags_s) == 0) {
      for (int i = 1 + flags_s.count; i < argc; i++) {
        if (read_file(argv[i], &flags_s) == 1) {
          fprintf(stderr, "%s: No such file or directory\n", argv[i]);
        }
      }
    } else {
      fprintf(stderr, "Invalid option\n");
    }
  } else {
    fprintf(stderr, "Not enough arguments\n");
  }
  return 0;
}

int read_file(char* file_name, flags* flags_s) {
  int error = 0;
  FILE* file = fopen(file_name, "r");
  if (file != NULL) {
    int symbol;
    int prev_symbol = '\n';
    flags_s->empty_lines = 0;
    flags_s->count = 1;
    while ((symbol = fgetc(file)) != EOF) {
      processing_flags(&symbol, prev_symbol, file, flags_s);
      if (symbol != EOF) fputc(symbol, stdout);
      prev_symbol = symbol;
    }
    fclose(file);
  } else {
    error = 1;
  }
  return error;
}

void processing_flags(int* c, int prev, FILE* file, flags* flags_s) {
  empty_lines_counting(*c, prev, flags_s);
  process_flag_s(c, file, flags_s);
  if (*c != EOF) {
    process_flag_n_b(prev, flags_s);
    process_flag_v_e_t(c, flags_s);
  }
}

void process_flag_v_e_t(int* c, flags* flags_s) {
  if (*c == '\n') {
    if (flags_s->e) printf("$");
  } else if (*c == '\t') {
    if (flags_s->t) {
      printf("^");
      *c = 'I';
    }
  } else if (flags_s->v) {
    if (*c <= 31 && *c >= 0) {
      printf("^");
      *c += 64;
    } else if (*c == 127) {
      printf("^");
      *c = '?';
    } else if (*c >= 128 && *c < 128 + 32) {
      printf("M-^");
      *c -= 64;
    }
  }
}

void process_flag_s(int* c, FILE* file, flags* flags_s) {
  if (flags_s->s && flags_s->empty_lines > 1) {
    while (*c == '\n') *c = fgetc(file);
    flags_s->empty_lines = 0;
  }
}

void process_flag_n_b(int prev, flags* flags_s) {
  if (prev == '\n') {
    if ((flags_s->b && flags_s->empty_lines == 0) || flags_s->n) {
      printf("%6d\t", flags_s->count++);
    } else if (flags_s->b && flags_s->empty_lines > 0 && flags_s->e) {
      printf("%6s\t", "");
    }
  }
}

void empty_lines_counting(int cur, int prev, flags* flags_s) {
  if ((cur == '\n') && prev == '\n') {
    flags_s->empty_lines++;
  } else {
    flags_s->empty_lines = 0;
  }
}

int parsing_options(int argc, char* argv[], flags* flags_s) {
  int error = 0, end = 0;
  for (int i = 1; i < argc && !error && !end; i++) {
    if (argv[i][0] == '-') {
      flags_s->count++;
      if (argv[i][1] == '-') {
        error = parsing_long_flags(argv[i] + 2, flags_s);
      } else
        error = parsing_flags(argv[i] + 1, flags_s);
    } else
      end = 1;
  }
  if (flags_s->b) flags_s->n = 0;
  if (flags_s->t || flags_s->e) flags_s->v = 1;
  return error;
}

int parsing_long_flags(char* str, flags* flags_s) {
  int error = 0;
  if (strcmp(str, "number-nonblank") == 0) {
    flags_s->b = 1;
  } else if (strcmp(str, "number") == 0) {
    flags_s->n = 1;
  } else if (strcmp(str, "squeeze-blank") == 0) {
    flags_s->s = 1;
  } else {
    error = 1;
  }
  return error;
}

int parsing_flags(char* str, flags* flags_s) {
  int length = strlen(str);
  int error = 0;
  for (int i = 0; i < length && !error; i++) {
    if (str[i] == 'b') {
      flags_s->b = 1;
    } else if ((str[i] == 'e') || (str[i] == 'E')) {
      flags_s->e = 1;
    } else if (str[i] == 'n') {
      flags_s->n = 1;
    } else if (str[i] == 's') {
      flags_s->s = 1;
    } else if ((str[i] == 't') || (str[i] == 'T')) {
      flags_s->t = 1;
    } else if (str[i] == 'v') {
      flags_s->v = 1;
    } else {
      error = 1;
    }
  }
  return error;
}
