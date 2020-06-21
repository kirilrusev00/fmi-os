#!/bin/bash

if [ $# -ne 2 ]; then
	echo "You must give 2 arguments"
	exit 1
fi

if [ ! -d "${1}" ]; then
	echo "First argument must be a directory"
	exit 2
fi

DIR="${1}"
STR="${2}"

find "${DIR}" -name "vmlinuz-[0-9]*.[0-9]*.[0-9]*-${STR}" -printf "%f\n" \
 2> /dev/null | sort -nr -t '.' -k 2 | head -n 1
