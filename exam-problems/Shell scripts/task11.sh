#!/bin/bash

find /home/SI -maxdepth 1 -type d -newerct '@1551168000' -not \
-newerct '@1551176100' -exec grep -F :{}:/etc/passwd \; \
| cut -d ':' -f 1,5 | cut -c 2- | cut -d ',' -f 1 | tr ':' '\t'
