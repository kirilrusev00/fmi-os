#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Wrong number of arguments"
	exit 1
fi

USER="${1}"

until who -u | egrep -q "^${USER}"; do
	sleep 1
done

echo "User ${USER} is logged in"
