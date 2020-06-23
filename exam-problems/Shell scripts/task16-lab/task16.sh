#!/bin/bash

if [ "$#" -ne 3 ]; then
	echo "You have to input 3 arguments: file name, string1 and string2"
	exit 1
fi

if [ ! -f "${1}" ]; then
	echo "Expected file name as first argument"
	exit 1
fi

FILE="${1}"
STRING1="${2}"
STRING2="${3}"
TEMP=$(mktemp)

cat "${FILE}" | grep -q "^${STRING1}="

if [ $? -eq 0 ]; then
	#record terms from string1
	cat "${FILE}" | grep "^${STRING1}=" | awk -F '=' '{ print $2 }' >> "${TEMP}"

	cat "${FILE}" | grep -q "^${STRING2}="

	if [ $? -eq 0 ]; then
		#record terms from string2
		cat "${FILE}" | grep "^${STRING2}=" | awk -F '=' '{ print $2 }' >> "${TEMP}"
	fi
fi

while read UNIQUE; do
	#we want to replace whole line, so we remember it
	WANT_TO_REPLACE=$(cat "${FILE}" | grep "^${STRING2}=")

	cat "${FILE}" | grep -q "^${STRING2}=${UNIQUE} "
	if [ $? -eq 0 ]; then
		NEW=$(cat "${FILE}" | grep "^${STRING2}=" | sed -E "s/=${UNIQUE} /=/g")
		sed -i "s/${WANT_TO_REPLACE}/${NEW}/g" "${FILE}"
		WANT_TO_REPLACE=$(cat "${FILE}" | grep "^${STRING2}=")
	fi

	cat "${FILE}" | grep "^${STRING2}=" | grep -q " ${UNIQUE}$"
    if [ $? -eq 0 ]; then
        NEW=$(cat "${FILE}" | grep "^${STRING2}=" | sed -E "s/${UNIQUE}$//g")
        sed -i "s/${WANT_TO_REPLACE}/${NEW}/g" "${FILE}"
        WANT_TO_REPLACE=$(cat "${FILE}" | grep "^${STRING2}=")
    fi

	cat "${FILE}" | grep "^${STRING2}=" | grep -q " ${UNIQUE} "
    if [ $? -eq 0 ]; then
        NEW=$(cat "${FILE}" | grep "^${STRING2}=" | sed -E "s/ ${UNIQUE} / /g")
        sed -i "s/${WANT_TO_REPLACE}/${NEW}/g" "${FILE}"
    fi

done < <(cat "${TEMP}" | grep -o ".* *" | tr " " "\n" | sort | uniq -d | sed '/^$/d')

rm "${TEMP}"








