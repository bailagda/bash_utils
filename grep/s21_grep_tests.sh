#!/bin/bash
COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
echo "" > log.txt

say -v Karen Hello, $LOGNAME, welcome to the project presentation!
for var in -c -e -n -o -v -i -s -h -l -o
do
          TEST1="$var for s21_grep.c Makefile"
          echo "$TEST1"
          ./s21_grep $TEST1 > s21_grep.txt
          grep $TEST1 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST1" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep.txt grep.txt

          TEST2="$var for s21_grep.c"
          echo "$TEST2"
          ./s21_grep $TEST2 > s21_grep.txt
          grep $TEST2 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST2" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep.txt grep.txt

          TEST3="$var void s21_grep.c Makefile"
          echo "$TEST3"
          ./s21_grep $TEST3 > s21_grep.txt
          grep $TEST3 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST3" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep.txt grep.txt
done

echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"