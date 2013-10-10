#!/bin/bash

if [ -e "$HOME/.egui" ]
then
    echo "\"$HOME/.egui\" already exists."
    exit 1
fi

filepos=${0%[\^\/]*}

if [[ "$filepos" =~ ^/.* ]]
then
    ln -s "$filepos" "$HOME/.egui"
else
    ln -s "`pwd`/$filepos" "$HOME/.egui"
fi

if [ $? -eq 0 ]
then
    echo "Default resource is installed as \"$HOME/.egui\"."
else
    echo "Problem occurred."
fi
