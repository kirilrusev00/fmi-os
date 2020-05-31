#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Give 3 numbers"
	exit 4
fi

#check for symbols different from digits, space and -
COUNT=$(echo "$1 $2 $3" | egrep -c -- '[^0-9 \-]')

if [ $COUNT -ne 0 ]; then
	exit 3
fi

CHECK=$1
LEFT=$2
RIGHT=$3

if [ $LEFT -gt $RIGHT ]; then
	exit 2
fi

if [ $CHECK -lt $LEFT ] || [ $CHECK -gt $RIGHT ]; then
	exit 1
fi
