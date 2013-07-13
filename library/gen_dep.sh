#!/bin/bash

./list_undef.sh $1 | awk '{ print $NF; }' | xargs -n 1 ./find_symbol.sh | sort | uniq
