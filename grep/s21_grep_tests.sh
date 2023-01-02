#!/bin/bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
TEST_FILE="for_tests/test.txt for_tests/opoop.txt"
PATTERN="lo"
FILE_FLAG_F="for_tests/t.txt"
echo "" > log.txt

#================BASE FLAGS test=======================

for var in -e -i -v -c -l -n
do
          TEST1="$var $PATTERN $TEST_FILE"
          ./s21_grep $TEST1 > s21_grep.txt
          CK_FORK=no leaks -atExit -- ./s21_grep $TEST1 >> RESUKT.txt
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

done

#=================BONUS FLAGS test======================

for var in -h -s -o
do
          TEST1="$var $PATTERN $TEST_FILE"
          ./s21_grep $TEST1 > s21_grep.txt
          CK_FORK=no leaks -atExit -- ./s21_grep $TEST1 >> RESUKT.txt
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
done

for var in -f
do
          TEST1="$var $FILE_FLAG_F $TEST_FILE"
          ./s21_grep $TEST1 > s21_grep.txt
          CK_FORK=no leaks -atExit -- ./s21_grep $TEST1 >> RESUKT.txt
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
done

echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"