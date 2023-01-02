CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE
SRCS=s21_cat.c

.PHONY: 
	all s21_cat test clean checkcheck check

all:  clean s21_cat

s21_cat: clean
	$(CC) $(CFLAGS) $(SRCS) -o s21_cat

test: all
	sh s21_cat_tests.sh

checkcheck: s21_cat
	cppcheck --enable=all --suppress=missingIncludeSystem s21_cat.c s21_cat.h
	CK_FORK=no leaks -atExit -- s21_cat

check: checkcheck clean

clean: 
	rm -rf *.o *.gch *.out log.txt s21_cat RESULT.txt
