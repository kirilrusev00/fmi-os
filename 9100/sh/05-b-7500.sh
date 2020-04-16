#!/bin/bash

if [ $# -eq 2 ]; then
	LEFTNUM=$1
	RIGHTNUM=$2
else
	LEFTNUM=0
	RIGHTNUM=100
fi

#echo "$LEFTNUM"
#echo "$RIGHTNUM"

RANDOMNUM=$(( (RANDOM % $RIGHTNUM) + $LEFTNUM))
#echo "$RANDOMNUM"

COUNT=0

while true; do
	COUNT=$(($COUNT + 1))
	read -p "Guess? " NUMBER
	if [ $NUMBER -lt $RANDOMNUM ]; then
		echo "...bigger!"
	elif [ $NUMBER -gt $RANDOMNUM ]; then
		echo "...smaller!"
	else
		echo "RIGHT! Guessed $RANDOMNUM in $COUNT tries!"
		break
	fi
done
