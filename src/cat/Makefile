CC = gcc 
FLAGS = -Wall -Werror -Wextra

all: s21_cat

s21_cat:
	$(CC) $(FLAGS) s21_cat.c -o s21_cat

test: s21_cat
	cp s21_cat ./test_cat/s21_cat
	cd test_cat && sh test_func_cat.sh
	rm -rf test_cat/s21_cat test_cat/*.log

clean: 
	rm -rf s21_cat test_cat/s21_cat test_cat/*.log
