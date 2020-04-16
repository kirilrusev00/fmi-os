#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Not 2 arguments"
	exit 2
fi

if [ ! -f "${1}" ]; then
	echo "First argument is not a valid file"
	exit 3
fi

USER=$(egrep "${2}" "${1}" | cut -d ' ' -f 2)

#echo "${USER}"

if [ -z "${USER}" ]; then
	echo "User not in address book"
	exit 1
fi

echo "${USER}"


