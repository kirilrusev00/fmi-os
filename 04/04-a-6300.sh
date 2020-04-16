#!/bin/bash

ps -eo stime,cmd | sort -k 1 -n | tail -n +2 | head -n 1
