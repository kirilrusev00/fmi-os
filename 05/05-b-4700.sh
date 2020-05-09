#!/bin/bash

if [ $# -eq 1 ]; then
	DELIMITER=" "
elif [ $# -eq 2 ]; then
	DELIMITER="${2}"
else
	echo "Not valid number of arguments"
	exit 2
fi

echo "${1}" | grep -o . | tac | xargs -n3 | tr -d ' ' | tr '\n' "${DELIMITER}" | grep -o . | tac | tail -n +2 | tr -d '\n'; echo

###

#echo "${1}" | rev | sed -E "s/([0-9]{3})/\1$DELIMITER" | rev \
#	| sed -E "s/^$DELIMITER//" \
#	| sed -E "s/^-$DELIMITER/-/"
