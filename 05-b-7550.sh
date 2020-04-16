#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Wrong number of arguments"
	exit 1
fi

USER="${1}"

if [ $(id -u "${USER}") -eq 1 ]; then
	echo "Username not valid"
	exit 2
fi

PROCESS_COUNT=0
while read -d $'\n' PID; do
	kill "${PID}"
	sleep 1
	kill -9 "${PID}"
	PROCESS_COUNT=(($PROCESS_COUNT+1))
done < <(ps -u "${USER}" -o pid=)

echo "Killed ${PROCESS_COUNT} processes of ${USER}"
