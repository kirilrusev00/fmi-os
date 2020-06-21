#!/bin/bash

[ $# -eq 2 ] || exit 1

INT1=${1}
INT2=${2}

ALL_FILES=$(mktemp)

find . -type f -maxdepth 1 2> /dev/null > "${ALL_FILES}"

mkdir a
mkdir b
mkdir c

function count_lines {
	wc -l "${1}" | cut -d ' ' -f1
}

while read FILE; do
	LINES=$(count_lines "${FILE}")
	if [ $LINES -lt $INT1 ]; then
		mv "$FILE" a
	elif [ $LINES -gt $INT2 ]; then
		mv "$FILE" c
	else
		mv "$FILE" b
	fi
done < "${ALL_FILES}"

rm "${ALL_FILES}"
