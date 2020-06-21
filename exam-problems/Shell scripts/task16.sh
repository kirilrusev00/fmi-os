#!/bin/bash

[ $# -eq 3 ] || exit 1
[ -r "${1}" ] || exit 1

F="${1}"
