#/bin/bash

read -p "Enter 3 file names: " f1 f2 f3

#if [ ! -f f1 ]; then
#	echo "You should enter 3 file names"
#	exit 1
#fi

paste f1 f2 | sort > f3
