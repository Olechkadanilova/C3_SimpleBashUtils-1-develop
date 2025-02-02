#ifndef S21_CAT_H
#define S21_CAT_H

#include <stdio.h>
#include <string.h>

typedef struct flags {
  int b;      // нумерует только непустые строки !
  int e;      //  отображает символы конца строки как $, обработка вместе с
              // флагом -v
  int n;      // нумерация выходных строк
  int s;      // сжимает несколько смежных пустых строк
  int t;      //  отображает табы как ^I
  int v;      // отображает скрытые/неотображаемые символы
  int count;  // счетчик аргументов, считанных как флаги
  int empty_lines;  // счетчик пустых строк
} flags;

int read_file(char* file_name, flags* flags_s);
int parsing_options(int argc, char* argv[], flags* flags_s);
int parsing_flags(char* str, flags* flags_s);
void processing_flags(int* c, int prev, FILE* file, flags* flags_s);
void process_flag_s(int* c, FILE* file, flags* flags_s);
void process_flag_v_e_t(int* c, flags* flags_s);
int parsing_long_flags(char* str, flags* flags_s);
void process_flag_n_b(int prev, flags* flags_s);
void empty_lines_counting(int cur, int prev, flags* flags_s);

#endif