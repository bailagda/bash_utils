#!/bin/bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
TEST_FILE="bytes.txt"
echo "" > log.txt

say -v Victoria Hello, $LOGNAME, welcome to the project presentation!
for var in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
do
          TEST1="$var $TEST_FILE"
          ./s21_cat $TEST1 > s21_cat.txt
          CK_FORK=no leaks -atExit -- s21_cat > RESULT.txt
          cat $TEST1 > cat.txt
          DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
          if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST1" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_cat.txt cat.txt
done

echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"