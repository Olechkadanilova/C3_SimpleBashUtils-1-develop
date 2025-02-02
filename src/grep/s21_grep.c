#include "s21_grep.h"

int main(int argc, char* argv[]) {
  options opts = {0};
  if (parcing_options(argc, argv, &opts) != 1)
    read_main_files(argc, argv, &opts);
  return 0;
}

void debag_printing(options opts) {
  printf(
      "i = %d\nv = %d\nc = %d\nl = %d\nn = %d\nh = %d\ns = %d\no = %d\ne = "
      "%d\nf = %d\n",
      opts.i_flag, opts.v_flag, opts.c_flag, opts.l_flag, opts.n_flag,
      opts.h_flag, opts.s_flag, opts.o_flag, opts.e_flag, opts.f_flag);
  printf("pattern: %s\n", opts.pattern);
}

int parcing_options(int argc, char* argv[], options* opts) {
  struct option long_options[] = {{0, 0, 0, 0}};
  int ch;
  int error = 0;
  while ((ch = getopt_long(argc, argv, "ivclnhsoe:f:", long_options, NULL)) !=
             -1 &&
         !error) {
    if (ch == 'i') {
      opts->i_flag = 1;
    } else if (ch == 'v') {
      opts->v_flag = 1;
    } else if (ch == 'c') {
      opts->c_flag = 1;
    } else if (ch == 'l') {
      opts->l_flag = 1;
    } else if (ch == 'n') {
      opts->n_flag = 1;
    } else if (ch == 'h') {
      opts->h_flag = 1;
    } else if (ch == 's') {
      opts->s_flag = 1;
    } else if (ch == 'o') {
      opts->o_flag = 1;
    } else if (ch == 'e') {
      opts->e_flag = 1;
      add_pattern(optarg, opts);
    } else if (ch == 'f') {
      opts->f_flag = 1;
      error = add_patterns_from_file(optarg, opts);
    } else if (ch == '?') {
      error = 1;
    }
  }
  if (opts->e_flag == 0 && opts->f_flag == 0 && !error) {
    add_pattern(argv[optind++], opts);
  }
  if (opts->v_flag) opts->o_flag = 0;
  return error;
}

void add_pattern(char* str, options* opts) {
  if (strlen(opts->pattern) != 0) {
    strcat(opts->pattern, "|");
  }
  if (strlen(str) == 0) {
    strcat(opts->pattern, ".");
  } else
    strcat(opts->pattern, str);
}

int add_patterns_from_file(char* filename, options* opts) {
  int error = 0;
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    if (!opts->s_flag)
      fprintf(stderr, "%s: No such file or directory\n", filename);
    error = 1;
  } else {
    char buffer[STR_SIZE];
    while (fgets(buffer, STR_SIZE, file)) {
      char* c = buffer + strlen(buffer) - 1;
      if (*c == '\n') *c = '\0';
      add_pattern(buffer, opts);
    }
  }
  return error;
}

void read_main_files(int argc, char* argv[], options* opts) {
  opts->files_count = argc - optind;
  for (int i = optind; i < argc; i++) {
    FILE* file = fopen(argv[i], "r");
    if (file != NULL) {
      opts->matches_count = 0;
      opts->lines_count = 1;
      char buffer[STR_SIZE];
      while (fgets(buffer, STR_SIZE, file)) {
        int len = strlen(buffer);
        if (buffer[len - 1] != '\n') {
          buffer[len + 1] = 0;
          buffer[len] = '\n';
        }
        regex_and_process_flags(argv[i], buffer, opts);
        opts->lines_count++;
      }
      if (!opts->matches_count) opts->files_match_count++;
      matches_info(argv[i], opts);
      fclose(file);
    } else if (!opts->s_flag)
      fprintf(stderr, "%s: No such file or directory\n", argv[i]);
  }
}

void matches_info(char* filename, options* opts) {
  if (opts->c_flag) {
    if (opts->files_count > 1 && !opts->h_flag) printf("%s:", filename);
    if (opts->l_flag)
      printf("%d\n", opts->matches_count > 0);
    else
      printf("%d\n", opts->matches_count);
  }
  if (opts->l_flag && opts->matches_count) {
    printf("%s\n", filename);
  }
}

void regex_and_process_flags(char* filename, char* buffer, options* opts) {
  regex_t regex;
  if (regcomp(&regex, opts->pattern, opts->i_flag ? REG_ICASE : REG_EXTENDED) ==
      0) {
    if (regexec(&regex, buffer, 0, NULL, 0) == opts->v_flag) {
      opts->matches_count++;
      if (!opts->c_flag && !opts->l_flag) {
        if (!opts->h_flag && opts->files_count > 1) printf("%s:", filename);
        if (opts->o_flag)
          process_o_flag(buffer, regex, opts);
        else {
          if (opts->n_flag) {
            printf("%d:", opts->lines_count);
          }
          fputs(buffer, stdout);
        }
      }
    }
  }
  regfree(&regex);
}

void process_o_flag(char* str, regex_t regex, options* opts) {
  while (regexec(&regex, str, 0, NULL, 0) == 0) {
    char buffer[STR_SIZE] = "";
    strcpy(buffer, str);
    int left = 0, right = strlen(buffer);
    while (!regexec(&regex, buffer + left, 0, NULL, 0)) {
      right--;
      buffer[right] = 0;
    }

    buffer[right] = str[right];
    right++;
    while (!regexec(&regex, buffer + left, 0, NULL, 0)) {
      left++;
    }
    left--;
    if (opts->n_flag) {
      printf("%d:", opts->lines_count);
    }
    printf("%s\n", buffer + left);
    int cut = right - left + 1;
    int len = strlen(str);
    for (int i = left; i <= len - cut; i++) str[i] = str[i + cut];
  }
}
