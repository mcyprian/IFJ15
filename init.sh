#!/bin/sh -e
set -v

export CC=$(which gcc)

cmake .

make

make test

unset CC

set +v
