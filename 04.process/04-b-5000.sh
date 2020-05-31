#!/bin/bash

ps -e -g students -o drs | awk 'BEGIN {s=0} {s+=$1} END {print s}'
