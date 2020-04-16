#!/bin/bash

if [ $# -ne 1 ]; then
	echo "You must give one argument"
	exit 1
fi


if ! id "${1}" &> /dev/null; then
	echo "Invalid user"
	exit 2
fi

USER="${1}"

ps -u "${USER}" -o pid,vsz,rss | tail -n +2 | sort -k2 -nr | while read PID VSZ RSS;do
	if [ $VSZ -eq 0 ]; then
		RESULT="inf"
	else
		RESULT=$(echo "scale=3; $RSS/$VSZ" | bc)
	fi
	echo "${PID} consumes $RESULT % of virtual memory"
done

