#!/bin/bash

[ $(id -u) -eq 0 ] || exit 1

while read USR HOMEDIR; do
	if [ ! -d ${HOMEDIR} ] || sudo -u "${USR}" [ ! -w "${HOMEDIR}" ]; then
		echo "${USER}"
	fi 
done < <(cat /etc/passwd | awk -F : '{print $1,$6}')
