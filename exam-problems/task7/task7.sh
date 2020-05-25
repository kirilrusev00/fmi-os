#!/bin/bash

cat passwd | fgrep '/Inf/' | cut -d ':' -f 1-5 | cut -d ',' -f1 | egrep "a$" | cut -d ' ' -f 1 | cut -c 2- | cut -c 2-3 | sort | uniq -c | sort -n | tail -n 1
