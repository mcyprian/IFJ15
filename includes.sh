#!/bin/bash

for cfile in $(ls *.c *.h); do
    if [ "$1" == '-p' ]; then
        sed -i '1 a\ * project: Implementace interpretu imperativního jazyka IFJ15' $cfile
    fi
    for header in $(ls *.h); do
        sed -i "s/<$header>/\"$header\"/g" $cfile
    done
done

exit 0
