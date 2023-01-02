.PHONY: all clean test check s21_grep
CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: clean s21_grep

s21_grep:
	${CC} ${CFLAGS} -g s21_grep.c -o s21_grep -lpcre

s21_grep_test:
	sh s21_grep_tests.sh

clean:
	rm -rf s21_grep.d* s21_grep_test s21_grep RESUKT.txt log.txt
 
check: s21_grep s21_grep_test
	python3 ../../materials/linters/cpplint.py --extensions=c s21_grep.c
	CK_FORK=no leaks -atExit -- ./s21_grep
	CK_FORK=no leaks -atExit -- ./s21_grep_test

test: s21_grep s21_grep_test