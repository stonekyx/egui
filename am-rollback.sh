#!/bin/bash

pathdir="${1/%\/local.mk/}"

pathdir="${pathdir/#\.\//}"

sed -i -e 's|%C%|'"${pathdir//\//_}"'|' -e 's|%D%|'"$pathdir"'|' $1
