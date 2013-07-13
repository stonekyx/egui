#!/bin/bash

for i in *.so
do
    if (readelf -s --dyn-syms $i | grep -v UND | grep $1 &> /dev/null)
    then
        echo $i
        break
    fi
done
