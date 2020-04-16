#!/bin/bash

export LOG_FILE=~/logs/remove.log

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
		rm "${i}"
		echo "$(date +"[%Y-%m-%d %H:%M:%S]") Removed file $(basename ${i})" >> "${LOG_FILE}"
	elif [ -d "${i}" ]; then
		if [ -z "$(find "${i}" -mindepth 1 -maxdepth 1)" ]; then
			rm -d "${i}"
			echo "$(date +"[%Y-%m-%d %H:%M:%S]") Removed directory $(basename ${i})/" >> "${LOG_FILE}"
		elif [ ! -z ${FLAG} ]; then
			rm -R "${i}"
			echo "$(date +"[%Y-%m-%d %H:%M:%S]") Removed directory recursively $(basename ${i})/" >> "${LOG_FILE}"
		fi
	fi
done

echo "Log file:"
cat "${LOG_FILE}"

