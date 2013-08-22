#!/bin/bash

pathdir="${1/%\/local.mk/}"

pathdir="${pathdir/#\.\//}"

sed -i -e 's|%C%|'"${pathdir//\//_}"'|g' -e 's|%D%|'"$pathdir"'|g' $1
