#!/bin/bash

find . -printf "%u\n" 2> /dev/null | grep "^$(whoami)$" | wc -l 
