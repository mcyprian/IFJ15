#!/bin/bash

test_cases=("source1 identificator_test operator_test double_test empty x  
single_token_test")
retVal=0
for tc in $test_cases ;do
    echo "Running $tc"
    output=$(./scanner_test test_data/$tc 2>/dev/null)
    diff_output=$(diff "test_data/output_$tc" <(echo "$output"))
    if [ -z "$diff_output" ]; then
        echo "test case $tc: PASSED"
    else
        retVal=1
        echo "tets case $tc: FAILED"
        echo "$diff_output"
    fi
done

exit $((retVal))
