#!/bin/bash

FOO="${1}"

[ $# -eq 1 ] || exit 1

#[ $(id -u) -eq 0 ] || exit 2

PROC=$(mktemp)
ps -e -o user= -o time= -o pid= > "${PROC}"

# a) 
NUM_PROCESSES_FOO=$(egrep -o "${FOO}" "${PROC}" | wc -l)
echo "Usernames with more processes than ${FOO}:"
cut -d ' ' -f 1 "${PROC}" | sort | uniq -c | awk -v "num=${NUM_PROCESSES_FOO}" '$1>num {print $2}'

# b)
AVG_TIME=$(cat "${PROC}" | awk '{ print $2 }' \
| awk -F: 'BEGIN {count=0; sec=0} {count+=1; sec+=$1*3600; sec+=$2*60; sec+=$3} END {print sec/count;}')
echo "Avg time: ${AVG_TIME}"

# c)
function kill_process () {
	PID=${1}
	kill -15 ${PID}
	sleep 2
	kill -9 ${PID}
}
while read PROCESS; do
	TIME=$(echo "${PROCESS}" | awk '{ print $2 }' \
| awk -F: 'BEGIN {sec=0} {sec+=$1*3600; sec+=$2*60; sec+=$3} END {print sec}')
	if [ $TIME -gt $(echo "$AVG_TIME * 2" | bc) ]; then
		kill_process $(echo "${PROCESS}" | awk '{ print $3 }')
	fi
done < <(egrep "${FOO}" "${PROC}")


rm "${PROC}"

