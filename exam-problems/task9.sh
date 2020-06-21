#!/bin/bash

find ~pesho -type f -links +1 -printf "%T@ %i\n" | sort -n | tail -n 1 | cut -d ' ' -f2

