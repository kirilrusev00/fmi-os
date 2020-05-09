#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Not right number of arguments"
	exit 1
fi

if [ ! -d "${1}" ]; then
	echo "First argument not a directory"
	exit 2
fi

find "${1}" -type f -size +"${2}"c -printf "%f\n"
