#!/bin/bash

if [ $# -lt 1 ]; then 
	echo "Not enough arguments. Usage: ${0} [-n N] FILE1..." 
	exit 1
fi

N=10

if [ "${1}" = "-n" ]; then
	if [ ! -z $(echo "${2}" | grep "[^0-9]") ]; then
		echo "Second argument must be an integer"
		exit 2
	else
		N="${2}"
		shift 2
	fi
fi

if [ $# -lt 1 ]; then 
	echo "Not enough arguments. Usage: ${0} [-n N] FILE1..." 
	exit 1
fi

IDF=$(mktemp)

for FILE; do
	echo "${FILE}" | sed -E "s/().log/\1/" >> "${IDF}"
done

OUTPUT=$(mktemp)

while read idf; do
	cat "${idf}.log" | tail -n $N | sed -E "s/([0-9\-]+ [0-9\:]+ )()/\1${idf} \2/" >> "${OUTPUT}"
done < "${IDF}"

cat "${OUTPUT}" | sort

rm "${IDF}"
rm "${OUTPUT}"
