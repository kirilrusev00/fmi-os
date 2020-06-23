#!/bin/bash

INPUT=$(mktemp)
DATA=$(mktemp)

cat | egrep "^[-+]?[0-9]+$" | sort -n | uniq > "${INPUT}"

[ $(cat "${INPUT}" | wc -l) -ne 0 ] || { echo "No numbers in input"; exit 0; }

while read NUM; do
	EXPR=$(echo "${NUM}" | sed -E 's/[-+]//' | sed -E 's/(.)/\1+/g' | sed 's/.$//' )
	SUM=$(( "${EXPR}" ))
	echo "${SUM} ${NUM}" >> "${DATA}"
done < "${INPUT}"

max_sum="$(cat "${DATA}" | sort -t ' ' -nk1 | tail -n 1 | awk '{print $1}')"
result="$(cat "${DATA}" \
		| egrep "^${max_sum} " \
		| sort -t ' ' -nk2 \
		| head -n 1 \
		| cut -d ' ' -f 2 )"

echo "Result: ${result}"

rm -- "${INPUT}"
rm -- "${DATA}"
