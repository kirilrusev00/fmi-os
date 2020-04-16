#!/bin/bash

read -p "Enter file name: " fileName

if [ ! -f "${fileName}" ]; then
	echo "You should enter file name."
fi

read -p "Enter string: " str

fgrep -q "${str}" "${fileName}"
echo "$?"
