#!/bin/bash

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
	echo "Arguments must be 1 or 2"
	exit 1
fi

DIR="${1}"

if [ $# -eq 2 ]; then
	NUM="${2}"

	find "${DIR}" -type f -links $NUM -o -links +$NUM 2> /dev/null
fi

if [ $# -eq 1 ]; then 
	find -L "${DIR}" -type l 2> /dev/null
fi
