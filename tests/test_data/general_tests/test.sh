#!/bin/bash

echo $1

../../../bin/interpreter tests_ok/"$1" > out 2> /dev/null

if [ $? -ne 0 ]; then
	echo WRONG RETURN VALUE
fi

if [ "`cat out`" != "`cat tests_ok/"$1".out`" ]; then
	echo WRONG OUTPUT
fi

echo ===============================
