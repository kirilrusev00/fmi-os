#!/bin/bash

[ $# -eq 3 ] || exit 1

F=$1
K=$2
S=$3
LINE=$(grep $K $F)

echo $LINE

VAL=$(echo $LINE | cut -d '=' -f 2 | tr -s ' ')
echo "[$VAL]"

NEWVAL=$(echo $VAL | sed -E "s/(^| )$S( |$)/\1\2/" | tr -s ' ')
NEWVAL=$(echo $NEWVAL | sed -E "s/^ //" | sed -E "s/ $//")

echo "[$NEWVAL]"
