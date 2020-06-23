#!/bin/bash

[ $# -eq 3 ] || exit 1

F="$1"
S1=$(egrep "^$2=" $F | cut -d '=' -f2)

if [ $(egrep -c "^$3=" $F) -eq 0 ]; then
	echo "$3=" >> $F
	exit 0
fi

S2=$(egrep "^$3=" $F | cut -d '=' -f2)

NEWS2=$(comm -13 <(echo "$S1" | tr -s ' ' | tr ' ' '\n' | sort) <(echo $S2 | tr -s ' ' | tr ' ' '\n' | sort) | xargs)

sed -i -e "s/^$3=$S2/$3=$NEWS2/" $F
