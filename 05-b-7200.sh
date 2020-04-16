#!/bin/bash

for i; do
	if [ -f "${i}" ]; then
		if [ -r "${i}" ]; then
			echo "$(basename "${i}") is readable"
		else
			echo "$(basename "${i}") is not readable"
		fi
	elif [ -d "${i}" ]; then
		COUNT_FILES=$(find "${i}" -maxdepth 1 -type f 2> /dev/null | wc -l)
		echo "$(find "${i}" -maxdepth 1 -type f -size -"${COUNT_FILES}" -printf '%f\n' 2> /dev/null)"
	else
		echo "$(basename -a "${i}") is not a file/directory name!"
	fi
done
