#!/bin/bash

read -p "Enter file name: " fileName

if [ ! -f "${fileName}" ]; then
	echo "You should enter file name."
	exit 10
fi

read -p "Enter string: " str

fgrep "${str}" "${fileName}" --color
echo "$?"
