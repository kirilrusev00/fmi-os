#!/bin/bash

echo "Enter name of user: "
read user;

who | awk '{print $1}' | grep -w -F "${user}" | wc -l
