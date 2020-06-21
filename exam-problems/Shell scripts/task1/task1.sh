#!/bin/bash

echo "Броят на редовете е $(cat philip-j-fry.txt | grep '[02468]' | grep -v '[a-w]' | wc -l)"
