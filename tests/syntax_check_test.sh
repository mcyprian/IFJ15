#!/bin/bash

assert_retval() {
    echo "ASSERT $2 $3"
    $2
    iRet=$?
    if [ $iRet -eq $3 ]; then
        return 0
    else
        echo "TEST $1 returns $iRet, expected $3" 
        exit 1
    fi
}

# test            0 1 2 3 4 5 6 7 8 9 10 11 12 13
expected_retVals=(0 0 0 0 0 0 0 0 0 2 0  2  9  0)

for ((num=1; num<=12; ++num)); do
    assert_retval "$num" "../bin/interpreter ./test_data/syntax_check_data/test$num" ${expected_retVals[$num]}
done
exit 0
