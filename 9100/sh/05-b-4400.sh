#!/bin/bash

if [ $# -lt 1 -o $# -gt 2 ]; then
	echo "You should enter 1 or 2 directory names"
	exit 1
fi

if [ ! "${1}" -d ]; then
	echo "First argument should be a directory name"
       exit 2
fi

SOURCE="${1}"

if [ $# -eq 2 ]; then
	if [ ! "${2}" -d ]; then
		echo "Second argument should be a directory name"
		exit 3
	else
		DESTINATION="${2}"
	fi
fi

if [ $# -eq 1 ]; then
	DESTINATION="target-$(date +'%Y-%m-%d-%H-%M')"
fi

echo "Source directory: ${SOURCE}"
echo "Destination directory: ${DESTINATION}"
echo -e "\nChanged files:"
find "${SOURCE}" -type f -mmin -45 -exec echo {} \;
#find "${SOURCE}" -type f -mmin -45 -exec cp {} "${DESTINATION}" \;
echo -e "\nSuccessfully copied files"

tarName=$(basename "${DESTINATION}")
tar -zcvf "${tarName}".tar.gz "${DESTINATION}"
