#!/bin/bash

ps -eo stime,cmd | sort -k 1 | tail -n +1 | head | awk '{print $2}
