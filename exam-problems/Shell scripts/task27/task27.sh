#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Not enough arguments! Usage: ${0} <file1>.csv <file2>.csv"
	exit 1
fi

if [ ! -f "${1}" ] || [ ! -r "${1}" ]; then
	echo "File ${1} does not exists or not readable"
	exit 2
fi

( [ -e "${2}" ] || touch "${2}" ) && [ ! -w "${2}" ] && echo cannot write to file ${2} && exit 1

a="${1}"
b="${2}"

while read line; do
	to_write=$(echo "${line}" | cut -d ',' -f 2-)
	if egrep -q ",${to_write}$" "${b}"; then
		continue
	else
		echo "${line}" >> ${b}
	fi
done < <(cat "${a}" | sort -t ',' -nk1)
