#!/bin/bash

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
	echo "Wrong number of arguments"
	exit 1
fi

DIR=""
FLAG_A=""

if [ $# -eq 1 ]; then
	if [ ! -d "${1}" ]; then
		echo "You must give a directory name"
		exit 2
	fi
	DIR="${1}"
fi

if [ $# -eq 2 ]; then
	if [ "$1" |= "-a" ]; then
		echo "First argument must be -a"
		exit 2
	fi
	if [ ! -d "${2}" ]; then
		echo "Second argument must be a directory name"
		exit 2
	fi
	
	FLAG_A="${1}"
	DIR="${2}"
fi

LIST="$(find "${DIR}" -mindepth 1 -maxdepth 1 -type d,f -printf "%f\n")"
if [ -z $FLAG_A ]; then
	LIST="$(echo "${LIST}" | egrep -v "^\.")"
fi

for i in $LIST; do
	if [ -f "${i}" ]; then
		echo "$(stat --printf "%n (%s bytes)\n" "${i}")"
	elif [ -z $FLAG_A ]; then
		echo "${i} ($(($(ls -l "${i}" | wc -l)-1)) entries)"
	else
		echo "${i} ($(($(ls -la "${i}" | wc -l)-1)) entries)"
	fi
done

