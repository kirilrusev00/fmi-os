#!/bin/bash

ps -eo pid,comm --sort=vsz | sort -k 1 -n | tail -n 
