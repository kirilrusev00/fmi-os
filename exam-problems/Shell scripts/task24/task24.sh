#!/bin/bash

# [ $(id -u) -eq 0 ] || { echo "Not executed by root"; exit 0; }

UIDS_TO_FIND=$(mktemp)
while read USER_INFO; do
	USER_ID=$(echo "${USER_INFO}" | cut -d ':' -f 3)
	HOME_DIR=$(echo "${USER_INFO}" | cut -d ':' -f 6)
	if [ ! -e "${HOME_DIR}" ]; then
		echo "${USER_ID}" >> "${UIDS_TO_FIND}"
		continue
	fi
	if [ "$(stat -c "%u" "${HOME_DIR}")" != "${USER_ID}" ]; then
		echo "${USER_ID}" >> "${UIDS_TO_FIND}"
        continue
	fi
	if [ "$(stat -c "%A" "${HOME_DIR}" | cut -c3)" != "w" ]; then
		echo "${USER_ID}" >> "${UIDS_TO_FIND}"
        continue
	fi
done < /etc/passwd 

PROC=$(mktemp)
ps -e -o uid,rss > "${PROC}"

function find_all_rss() {
	cat "${PROC}" \
	| egrep "^[ ]*${1} " \
	| awk 'BEGIN{sum = 0} {sum += $2} END{print sum}'
}

ROOT_ALL_RSS=$(find_all_rss 0)

while read USER_ID; do
	if [ $(find_all_rss ${USER_ID}) -gt $ROOT_ALL_RSS ]; then
		killall -u "${USER_ID}"
		sleep 2
		killall -u "${USER_ID}" -s SIGKILL
	fi
done < "${UIDS_TO_FIND}"

rm -- "${UIDS_TO_FIND}"
rm -- "${PROC}"
