#!/bin/bash

processCount=$(ps --no-headers --group fmi | wc -l)

totalMemory=$(ps --no-headers -o 'pid group rss' --group fmi | awk '{totalMem = totalMem + $3} END {print totalMem}')

echo "processCount: ${processCount}"
echo "totalMemory: ${totalMemory}"

if [ "${processCount}" -eq 0 ]; then
	exit 1
fi

echo "result: $(( totalMemory / processCount ))"
