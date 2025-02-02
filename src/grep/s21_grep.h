#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE 4096

typedef struct options {
  char pattern[STR_SIZE];
  int e_flag, i_flag, v_flag, c_flag, l_flag, n_flag, h_flag, s_flag, f_flag,
      o_flag;
  int matches_count;
  int lines_count;
  int files_match_count;
  int files_count;
} options;

int parcing_options(int argc, char* argv[], options* opts);
void add_pattern(char* str, options* opts);
int add_patterns_from_file(char* filename, options* opts);
void read_main_files(int argc, char* argv[], options* opts);
void debag_printing(options opts);
void regex_and_process_flags(char* filename, char* buffer, options* opts);
void process_o_flag(char* str, regex_t regex, options* opts);
void matches_info(char* filename, options* opts);

#endif