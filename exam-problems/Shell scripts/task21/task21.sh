#!/bin/bash

# [ $(id -u) -eq 0 ] || { "Not executed by root"; exit 0; }

PROC=$(mktemp)
ps -e -o uid,pid,rss --no-header | sort -n > "${PROC}"

if [ $(cat "${PROC}" | wc -l) -eq 0 ]; then
	echo "There are no processes"
	exit 0
fi

CURR_UID=$(cat "${PROC}" | head -n 1 | awk '{print $1}')
CURR_COUNT=0
CURR_SUM=0
DATA=$(mktemp)

while read USER_ID PID RSS; do
	if [ "${USER_ID}" -eq "${CURR_UID}" ]; then
		CURR_COUNT=$(("${CURR_COUNT}" + 1))
		CURR_SUM=$(("${CURR_SUM}" + "${RSS}"))
	else
		echo "${CURR_UID} ${CURR_COUNT} ${CURR_SUM}" >> "${DATA}"
		CURR_UID="${USER_ID}"
		CURR_COUNT=1
		CURR_SUM="${RSS}"
	fi
done < "${PROC}"

echo "${CURR_UID} ${CURR_COUNT} ${CURR_SUM}" >> "${DATA}"
cat "${DATA}"

# b)
while read USER_ID COUNT SUM; do
	AVG=$(("${SUM}" / "${COUNT}"))
	MAX_PROC=$(cat "${PROC}" \
				| grep -e "^[ ]*${USER_ID} " \
				| sort -n -k3 \
				| tail -n 1)
	MAX_PROC_RSS="$(echo "${MAX_PROC}" | awk '{print $3}')"
	MAX_PROC_PID="$(echo "${MAX_PROC}" | awk '{print $2}')"
	if [ $MAX_PROC_RSS -gt $(($AVG * 2)) ]; then
		kill -s SIGTERM "${MAX_PROC_PID}"
		sleep 2
		kill -s SIGKILL "${MAX_PROC_PID}"
	fi
done < "${DATA}"

rm -- "${PROC}"
rm -- "${DATA}"
