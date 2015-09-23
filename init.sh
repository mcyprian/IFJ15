#!/bin/sh -e
set -v

export CC=$(which gcc)

cmake .

make

unset CC

set +v
