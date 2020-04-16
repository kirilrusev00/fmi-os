#!/bin/bash

pfile="$(mktemp)"

ps --no-header -e -o pid,ppid > "${pfile}"

function get_children {
	cat "${pfile}" | awk -v pid="${1}" '$2 == pid { print $1 }'
}

function count_children {
	get_children "${1}" | wc -l	
#ps --ppid "${1}" --no-header | wc -l
}

while read -d $'\n' line; do
	read -d ',' pid ppid < <(echo "${line}")
	# <<<... can be used instead of <(echo ...)
	#echo "pid: ${pid}, ppid: ${ppid}"
	[[ $(count_children "${pid}") -gt $(count_children "${ppid}") ]] && \
		echo "${pid}"
done < "${pfile}"

rm "${pfile}"


#ps --no-header -e -o pid,ppid | while read -d $'\n' pid_and_ppid; do
	#echo "pid: ${pid}"
#	read pid ppid < <(echo "${pid_and_ppid}")

#	[ $(count_children "${pid}") -gt $(count_children $"${ppid}") ] && echo "${pid}"
#done 2>/dev/null

#count_children "{$1}
