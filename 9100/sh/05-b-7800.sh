#!/bin/bash

#echo $PATH

echo "${PATH}" | while read -d ':' dir; do
	echo "$dir"
done
