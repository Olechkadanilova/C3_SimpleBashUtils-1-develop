CC = gcc 
FLAGS = -Wall -Werror -Wextra

all: s21_grep

s21_grep:
	$(CC) $(FLAGS) s21_grep.c -o s21_grep

test: s21_grep
	cp s21_grep ./test_grep/s21_grep
	cd test_grep && sh test_func_grep.sh
	rm -rf test_grep/s21_grep test_grep/*.log

clean: 
	rm -rf s21_grep test_grep/s21_grep test_grep/*.log
