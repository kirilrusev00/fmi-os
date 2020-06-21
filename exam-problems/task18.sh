#!/bin/bash

USERS=$(mktemp)
cat /etc/passwd | cut -d: -f1,6 | tr ':' ' ' > "${USERS}" 

function find_most_recent {
	find "${1}" -type f -printf "%T@ %f\n" 2> /dev/null | sort -nr | head -n 1
}

MOST_RECENT=$(mktemp)

while read name home; do
	[ -d "${home}" -a -r "${home}" ] || continue

	cur_file="$(find_most_recent "${home}")"

	[ -n "${cur_file}" ] || continue

	echo "${name} ${cur_file}" >> "${MOST_RECENT}"

done < "${USERS}"

cat "${MOST_RECENT}" | sort -nr -k2 | head -n 1 | cut -d' ' -f1,3-

rm -- "${MOST_RECENT}"
rm -- "${USERS}"
