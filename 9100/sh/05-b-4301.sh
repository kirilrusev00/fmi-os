#!/bin/bash

if [ $# -ne 3 ]; then
       	echo "Not correct number of arguments"  
	exit 3
fi

if [ ! -w "${1}" ]; then
	echo "Address book file not writeable"
	exit 4
fi

BOOK="${1}"
NAME="${2}"
NICK="${3}"
tmpFile="$(mktemp)"

cat passwd.txt | egrep "${NAME}" | cut -d ':' -f1 > tmpFile

if [ $(cat tmpFile | wc -l) -eq 0 ]; then
	echo "No user found"
	rm tmpFile
	exit 1
fi

if [ $(cat tmpFile | wc -l) -eq 1 ]; then
	ENTRY_TO_ADD="${NICK} $(cat tmpFile)"
	echo "${ENTRY_TO_ADD}" >> "${BOOK}"
	rm tmpFile
	exit 0
fi

echo "Choose user: "
COUNT=1
while read -r line; do 
	echo "${COUNT} ${line}"
	COUNT=$((COUNT + 1))
done < tmpFile
echo "0 None of the above"

read CHOICE

if [ $CHOICE -eq 0 ]; then
	echo "No user added"
else 
	USER_TO_ADD="$(cat tmpFile | tail -n +"$CHOICE" | head -n 1)"
	echo "$USER_TO_ADD"
	echo "${NICK} ${USER_TO_ADD}" >> "${BOOK}"
fi

rm tmpFile
