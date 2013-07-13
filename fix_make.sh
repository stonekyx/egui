#!/bin/bash

diff "$1" <(./fix_make "$1" | tee /tmp/Makefile)
mv -iv /tmp/Makefile "$1"
