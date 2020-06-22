#!/bin/bash

if [ $# -ne 2 ]; then
	echo "You must give 2 arguments"
	exit 1
fi

if [ ! -f "${1}" ]; then
	echo "First argument must be a file"
	exit 2
fi

if [ ! -r "${1}" ]; then
	echo "First argument must be a readable file"
	exit 2
fi

if [ ! -d "${2}" ]; then
	echo "Second argument must be a directory"
	exit 3 
fi

if [ ! -z $(ls -A "${2}") ]; then
	echo "Second argument must be an empty directory"
	exit 3 
fi

FILE="${1}"
DIR="${2}"

DICT_FILE="${DIR}"/dict.txt
touch "${DICT_FILE}"
NUM=0
while read FIRST_NAME LAST_NAME; do
	echo "${FIRST_NAME} ${LAST_NAME};${NUM}" >> "${DICT_FILE}"	
	NUM_FILE="${DIR}"/$NUM.txt
	touch "${NUM_FILE}"
	egrep "^${FIRST_NAME} ${LAST_NAME}\>" "${FILE}" >> "${NUM_FILE}"
	NUM=$(($NUM + 1))
done < <(cat "${FILE}" | sort | cut -d ':' -f 1 | cut -d ' ' -f 1,2 | uniq)
