#!/bin/bash
FILE="tmp.txt"
if [[ -f "${FILE}" ]]; then
	run_index="$(cat "${FILE}")"
else
	run_index=0
fi

run_index="$(( run_index + 1 ))"
echo "${run_index}" > "${FILE}"

case "${run_index}" in
	1) sleep 3; exit 2 ;;
	2) sleep 3; exit 3 ;;
	3) sleep 0.5; exit 0 ;;
	4) sleep 1; exit 1 ;;
	5) sleep 1; exit 0 ;;
	6) sleep 1; exit 1 ;;
	7) sleep 1; exit 1 ;;
	*) echo "you must not see this" >&2 ; sleep 1 ; exit 1 ;;
esac
