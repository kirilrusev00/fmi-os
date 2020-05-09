#!/bin/bash

if [ ! -d "${1}" ]; then
	echo "First argument must be a directory"
	exit 1
fi

HASHES=$(mktemp)

find "${1}" -maxdepth 1 -type f -exec md5sum {} \; | sort > "${HASHES}"

cat "${HASHES}" | awk '{ print $1 }' | uniq -d | while read DUPLICATED_HASH; do
	grep "${DUPLICATED_HASH}" "${HASHES}" \
		| awk '{ print $2 }' \
		| tail -n +2 \
		| xargs rm --
done

rm "${HASHES}"
