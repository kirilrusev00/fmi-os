#!/bin/bash

if [ $(id -u) -ne 0 ]; then
    echo "Script not executed by root"
    exit 2
fi

if [ $# -ne 1 ]; then
	echo "You must give 1 argument"
	exit 1
fi

if [ ! -z $(echo "${1}" | egrep -- '[^0-9]') ]; then
	echo "You must give a positive number"
	exit 1
fi

NUM="${1}"

function kill_process {
	PID=${1}
	echo "killing ${PID}"
	kill -s SIGTERM "${PID}"
	sleep 2
	kill -s SIGKILL "${PID}"
}

for U in $(ps -e -o user --no-headers | sort | uniq); do
	TOTAL_RSS=0

	while read CPID CRSS; do
		TOTAL_RSS=$(expr ${TOTAL_RSS} + ${CRSS})

		MAX_RSS=${CRSS}
		MAX_RSS_PID=${CPID}
	done < <(ps -u ${U} -o pid,rss --no-headers | sort -n -k 2)

	#echo $U $TOTAL_RSS $MAX_RSS $MAX_RSS_PID

	if [ $TOTAL_RSS -gt $NUM ]; then
		kill_process ${MAX_RSS_PID}
	fi
done

