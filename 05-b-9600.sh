#!/bin/bash

export BACKUP_DIR=~/.backup

if [ $# -eq 0 ]; then
	echo "You must give some arguments"
	exit 1
fi

FLAG=""
if [ "${1}" == "-r" ]; then
	FLAG="-r"
	shift 1
fi

for i; do
	if [ -f "${i}" ]; then
		SUFFIX="_$(date +"%Y-%m-%d-%H-%M-%S").gz"
		gzip -c "${i}" > $BACKUP_DIR/"${i}$SUFFIX"
	elif [ -d "${i}" ]; then
		SUFFIX="_$(date +"%Y-%m-%d-%H-%M-%S").tgz"
		if [ ! -z ${FLAG} ] || [ -z "$(find "${i}" -mindepth 1 -maxdepth 1)" ]; then
			tar -zcf $BACKUP_DIR/"${i}$SUFFIX" "${i}" --remove-files
		else
			echo "error: ${i}/ is not empty, will not delete"
		fi
	else
		echo "${i} is not a valid file/directory name"
	fi
done
