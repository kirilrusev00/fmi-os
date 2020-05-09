#!/bin/bash

if [ $# -ne 1 ]; then
	echo "You should enter 1 argument"
	exit 1
fi

if [ ! -d "${1}" ]; then
    echo "Argument must be a directory"
    exit 1
fi

DIR=${1}

find "${DIR}" -xtype l

