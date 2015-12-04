#!/bin/bash

for cfile in $(ls *.c *.h); do

    for header in $(ls *.h); do
        sed -i "s/<$header>/\"$header\"/g" $cfile
    done
done

exit 0
