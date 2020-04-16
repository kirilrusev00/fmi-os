#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Not 1 argument."
	exit 2
fi

if echo "${1}" | egrep -q '[^0-9A-Za-z]'; then
	echo "bad characters found"
	exit 1
fi

