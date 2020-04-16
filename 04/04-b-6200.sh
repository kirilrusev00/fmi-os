#!/bin/bash

grep -xf <(ps -e -o user= | sort | uniq) <(grep -vxf <(who | cut -d ' ' -f 1) <(cat /etc/passwd | cut -d ':' -f 1 ))
