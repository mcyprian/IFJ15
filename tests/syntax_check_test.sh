#!/bin/bash

set -e

../bin/interpreter ./test_data/syntax_check_data/test
../bin/interpreter ./test_data/syntax_check_data/test2
../bin/interpreter ./test_data/syntax_check_data/test3
