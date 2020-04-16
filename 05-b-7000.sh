#!/bin/bash

read -p "Enter a string: " string

for i; do
	echo "${i}: $(egrep "${string}" "${i}" | wc -l)"
done	
