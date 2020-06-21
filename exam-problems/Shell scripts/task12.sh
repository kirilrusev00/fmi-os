#!/bin/bash

function last_changed() {
		#~velin
	find ~ -type f -printf "%T@ %i %p\n" | sort -n | tail -n 1 | awk '{print $2}'
}

	#~velin
find ~ -inum $(last_changed) | egrep -o '.' | fgrep '/' | wc -l
