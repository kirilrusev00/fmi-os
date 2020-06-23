#!/bin/bash

INPUT=$(mktemp)

cat | egrep "^[-+]?[0-9]+$" | sort -n | uniq > "${INPUT}"

[ $(cat "${INPUT}" | wc -l) -ne 0 ] || { echo "No numbers in input"; exit 0; }

MAX="$(cat "${INPUT}" | tail -n 1)"
MIN="$(cat "${INPUT}" | head -n 1)"

ABS_MAX="$(echo "${MAX}" | sed "s/-//")"
ABS_MIN="$(echo "${MIN}" | sed "s/-//")"

echo "Biggest:"
if [ $ABS_MIN -eq $ABS_MAX ]; then
	echo "${MIN}"
	echo "${MAX}"
elif [ $ABS_MIN -lt $ABS_MAX ]; then
	echo "${MAX}"
else
	echo "${MIN}"
fi

rm -- "${INPUT}"
