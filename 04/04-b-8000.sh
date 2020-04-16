#!/bin/bash

ps -e --no-headers -o 'tty pid comm' | grep ^? | tr -s ' ' | sort -k 3 | uniq -f 2 | cut -d ' ' -f 2-
