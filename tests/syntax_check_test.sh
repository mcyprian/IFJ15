#!/bin/bash

set -e

../bin/interpreter ./test_data/syntax_check_data/test
../bin/interpreter ./test_data/syntax_check_data/test2
../bin/interpreter ./test_data/syntax_check_data/test3
../bin/interpreter ./test_data/syntax_check_data/test4
../bin/interpreter ./test_data/syntax_check_data/test5
../bin/interpreter ./test_data/syntax_check_data/test6
../bin/interpreter ./test_data/syntax_check_data/test7
../bin/interpreter ./test_data/syntax_check_data/test8
! ../bin/interpreter ./test_data/syntax_check_data/test9
../bin/interpreter ./test_data/syntax_check_data/test10
! ../bin/interpreter ./test_data/syntax_check_data/test11
../bin/interpreter ./test_data/syntax_check_data/test12
../bin/interpreter ./test_data/syntax_check_data/fcb.cc
