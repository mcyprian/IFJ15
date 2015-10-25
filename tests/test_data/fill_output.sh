#!/bin/bash

file=$1

token_types=('double' 'int' 'string' 'cin' 'cout' 'do' 'else' 'for' 'if'
'return' 'while' 'L_int' 'L_double' 'L_string' 'id' '=' '/' '==' '>\b' '>='
'<\b' '<=' '<<' '+' '>>' '____' '-' '!=' '!' 'auto' ',' ')' '}' '(' '{' ';'
 'start' 'error' 'EOF') 

isin() {
    local elem=$1
    local seeking=$2; shift
    local in=1
    for elem; do
        echo "$elem == $seeking"
        if [[ $elem == $seeking ]]; then
            in=0
            break
        fi
    done
    echo "isin return $in"
    return $in
}

skip=(11 12 13 14 25 36 37)
i=0

for token in ${token_types[@]} ; do
    if [ $i -eq 11 -o $i -eq 12 -o $i -eq 13 -o $i -eq 14 ]; then
        i=$((i+1))
        continue
    elif [ $i -eq 25  -o $i -eq 36 -o $i -eq 39 ]; then
        i=$((i+1))
        continue
    fi
    echo "$token $i"
    sed -i -e "s/$token/$token $i/g" $file
    i=$((i+1))
done

exit 0
