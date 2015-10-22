#!/bin/sh -e
set -v

export CC=$(which gcc)

cmake .

make

make test

doxygen dox 1>/dev/null 2>/dev/null

unset CC

set +v
