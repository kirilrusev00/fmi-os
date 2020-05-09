#!/bin/bash

FLAG=""
STRING=""
if [ $# -eq 1 ]; then
	STRING="${1}"
elif [ $# -eq 2 ]; then
	FLAG="${1}"
	STRING="${2}"
else
	echo "Wrong number of arguments"
	exit 2
fi

case "${FLAG}" in
	"-r")
		echo -e "\033[0;31m${STRING}"
		;;
	"-g")
		echo -e "\033[0;32m${STRING}"
		;;
	"-b")
		echo -e "\033[0;34m${STRING}"
		;;
	"")
		echo "${STRING}"
		;;
	*)
		echo "Unknown colour"
		exit 1
esac

echo -ne '\033[0m'
