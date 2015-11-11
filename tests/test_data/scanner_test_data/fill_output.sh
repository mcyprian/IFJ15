#!/bin/bash

file=$1

token_types=('==' '>' '<' '>=' '<=' '+' '-' 'mul' '/' '!=' ')' '('
'end_of_expr' 'id' 'L_INT' 'L_DOUBLE' 'L_STRING' '=' '<<' '>>' 'double' 'int'
'string' 'cin' 'cout' 'do' 'else' 'for' 'if' 'return' 'while' 'UO_EXCLM'
'auto' ',' '}' '{' ';') 

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
    if [ $i -eq 13 -o $i -eq 14 -o $i -eq 15 -o $i -eq 16 ]; then
        i=$((i+1))
        continue
    elif [ $i -eq 7  -o $i -eq 12 -o $i -eq 31 ]; then
        i=$((i+1))
        continue
    fi
    echo "$token $i"
    sed -i -e "s/$token/$i/g" $file
    i=$((i+1))
done

exit 0
