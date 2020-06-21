#!/bin/bash

find ~ -type f -printf "%n %p\n" | sort -n | tail -n 5 | awk '{print $2}'
	#.
