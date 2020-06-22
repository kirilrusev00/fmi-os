#!/bin/bash

[ $# -eq 3 ] || echo "Usage: ${0} <dirname> <dirname> <string>" && exit 1
[ -d "${1}" ] || echo "First argument must be a directory!" && exit 2
[ -d "${2}" ] || echo "Second argument must be a directory!" && exit 3
[ -z $(ls -A "${2}") ] || echo "Second argument must be an empty directory" && exit 4
[ $(id -u) -eq 0 ] || echo "Not executed as root" && exit 0

SRC="${1}"
DST="${2}"
ABC="${3}"

while read FILE; do
	FILE_IN_DST=$(echo "${FILE}" | sed -E "s%${SRC}\/%${DST}\/%")
	mkdir -p $(dirname "${FILE_IN_DST}")
	mv -- "${FILE}" "${FILE_IN_DST}"
done < <(find "${SRC}" -type f -name "*${ABC}*" 2> /dev/null)
