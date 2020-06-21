#!/bin/bash

find_symlinks() {
	DIR="${1}"

	find "${DIR}" -type l 2> /dev/null | xargs -I {} stat {} -c '%N'

	echo "Broken symlinks: $(find -L "${DIR}" -type l 2> /dev/null | wc -l)"
}

case $# in
	1)
		if [ ! -d "${1}" ]; then
			echo "First argument must be a directory"
			exit 2
		fi
		find_symlinks "${1}"
		;;
	2)
		if [ ! -d "${1}" ]; then
			echo "First argument must be a directory"
			exit 2
		fi
		find_symlinks "${1}" >> "${2}"
		;;
	*)
		echo "You must give 1 or 2 arguments"
		exit 1
esac
