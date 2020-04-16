#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Wrong number of arguments"
	exit 2
fi

FLAG="${1}"
case "${FLAG}" in
	"-r")
		COUNT=0
		;;
	"-g")
		COUNT=1
		;;
	"-b")
		COUNT=2
		;;
	"-x")
		COUNT=-1
		;;
	*)
		echo "Unknown colour"
		exit 1
esac

while read line; do
	case $COUNT in
		0)
			echo -e "\033[0;31m${line}"
			COUNT=1
			;;
		1)
			echo -e "\033[0;32m${line}"
			COUNT=2
			;;
		2)
			echo -e "\033[0;34m${line}"
			COUNT=0
			;;
		-1)
			echo "${line}"
			;;
	esac
done

echo -ne '\033[0m'
