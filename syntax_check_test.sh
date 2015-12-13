#!/bin/bash

assert_retval() {
    echo "ASSERT $2 $3"
    $2
    iRet=$?
    if [ $iRet -eq $3 ]; then
        return 0
    else
        echo "TEST $1 returns $iRet, expected $3" >> errors
        exit 1
    fi
}

fb_expected_retVals=(0 0 0 3 3 0 0 1 1 1 2 2 2 2 3 2 3 3 3 3 3 3 3 3 4 4 4 4 4
5 0 10 3 3 3 8 2 2 2 2 2 2 2 2 7 1 1 1 1 1 1 8 8 0 8 0 9 9 9 9 0 10 3 2 1 4 8 0)


for ((num=0; num < 68; ++num)); do
    assert_retval "$num" "./interpreter
    ifj_tests/fb_test$num" ${fb_expected_retVals[$num]}
done

exit 0
