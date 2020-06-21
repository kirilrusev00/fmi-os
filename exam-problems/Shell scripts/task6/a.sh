#!/bin/bash

find . -type f -size 0 | xargs rm # изпълнява се веднъж
						#-exec rm {} \; изпълнява се еднократно за всеки файл
