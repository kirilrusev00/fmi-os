#!/bin/bash

function find_group() {
										#201
	sort -t ':' -k 3 -n passwd | head -n 2 | tail -n 1 | cut -d ':' -f 4 
}

egrep --color "s[^.]+:x:[[:digit:]]+:$(find_group):" passwd | cut -c 2- | sort -t ':' -k 1 -n | cut -d ':' -f 5,6 \
| sed -e 's/,[^:]*:/:/'
