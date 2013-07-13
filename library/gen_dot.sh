#!/bin/bash

echo "digraph G {"
for i in *.so
do
    for j in $(./gen_dep.sh $i)
    do
        if [[ "$i" =~ ^lib(.*)\.so$ ]]
        then
            ii=${BASH_REMATCH[1]}
            if [[ "$j" =~ ^lib(.*)\.so$ ]]
            then
                jj=${BASH_REMATCH[1]}
                echo "    \"$ii\" -> \"$jj\";"
            fi
        fi
    done
done
echo "}"
