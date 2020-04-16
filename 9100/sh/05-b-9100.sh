#!/bin/bash

if [ $# -ne 2 ]; then
	echo "You must give 2 arguments"
	exit 1
fi

if [ ! -d "${1}" ]; then
	echo "First argument is not a directory name"
	exit 2
fi

if [ ! -d "${2}" ]; then
	echo "Second argument is not a directory name"
	exit 3
fi

SOURCE="${1}"
DEST="${2}"

while read EXT; do
	mkdir -p "${DEST}"/$EXT
	find "${SOURCE}" -maxdepth 1 -type f -name "*.$EXT" | xargs -I {} cp {} "${DEST}"/$EXT
done < <(find "${SOURCE}" -maxdepth 1 -type f -name "[^.]*.[^.]*"  -printf "%f\n" | rev | cut -d '.' -f1 | rev | sort | uniq)

