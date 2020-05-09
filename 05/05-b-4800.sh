#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Not correct number of arguments"
	exit 3
fi

if [ ! -f "${1}" ]; then
	echo "First argument must be a file"
	exit 2
fi

if [ ! -d "${2}" ]; then
	echo "Second argument must be a directory"
	exit 1
fi

FILE="${1}"
DIRECTORY="${2}"
COUNT=0

while read -d $'\0' file; do
	diff "${file}" "${FILE}" &> /dev/null
	if [ $? -eq 0 ]; then
		COUNT=$(($COUNT+1))
		echo "${file}"
	fi
done < <(find "${DIRECTORY}" -type f -print0)

if [ "$COUNT" -eq 0 ]; then
	echo "No copies found"
fi

#FILEHASH="$(md5sum "${1}" | awk '{ print $1 }')"
#FILENAME="$(basename -a ${1})"
#grep "${FILEHASH}" <(find "${2}" -type f -exec md5sum {} \;) | awk '{ print $2 }'
