#!/bin/bash

ps -eo user,comm | grep 'vim' | uniq -c | awk -F '$1>1 { print $2 }'
