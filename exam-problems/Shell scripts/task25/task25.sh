#!/bin/bash

[ $# -eq 1 ] || { echo "Usage: $0 <LOGDIR>"; exit 1; }

if [ ! -d "${1}" ] || [ ! -r "${1}" ]; then
	echo "First argument must be a readable directory"
	exit 2
fi

LOGDIR="${1}"

# LOGDIR/protocol/account/friend/yyyy-mm-dd-hh-mm-ss.txt

stat_friends=$(mktemp)

while read friend; do
	lines="$(find "${LOGDIR}" -mindepth 4 -maxdepth 4 -type f \
		| fgrep "/${friend}/" \
		| xargs -I {} wc -l {} \
		| awk '{ print $1 }' \
		| awk 'BEGIN{sum = 0} {sum += $1} END{print sum}')"
	echo "${friend} ${lines}" >> "${stat_friends}"
done < <(find "${LOGDIR}" -mindepth 3 -maxdepth 3 -type d | cut -d '/' -f 4 | sort | uniq)

cat "${stat_friends}" | sort -rn -k2 | head 

rm -- "${stat_friends}"
