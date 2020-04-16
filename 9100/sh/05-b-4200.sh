#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid argument count"
	exit 1
fi

MAXCOUNT=0
CURRCOUNT=0

for i in $(echo "$(cat "$1" | grep -o "." | egrep "({|})" | tr "\n" " ")"); do
	if [ $i = "{" ]; then
		CURRCOUNT=$((CURRCOUNT+1))
		if [ $CURRCOUNT -gt $MAXCOUNT ]; then
			MAXCOUNT=$CURRCOUNT
		fi
	else
		CURRCOUNT=$((CURRCOUNT-1))
	fi
done

echo "The deepest nesting is ${MAXCOUNT} levels"

########

[ $# -eq 1 ] || exit 1

F=${1}

DATA=$(cat $F | sed -e 's/[^{}]//g' | tr -d '\n' | tr -d ' ')

#echo "foo: $DATA"

while fgrep -- '{}{' <(echo ${DATA}); do
	DATA=$(echo "${DATA}" | sed -e 's/{}{/{/')
done

#echo "bar: $DATA"

ALLCHARS=$(echo ${DATA} | tr -d '}' | wc -c)
echo $((--ALLCHARS))
#--ALLCHARS because we count the eof symbol

