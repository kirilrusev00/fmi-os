#/bin/bash

echo "Enter a direcrory name:"
read dirname

if [ ! -d "${dirname}" ]; then
	echo "Not a directory name"
fi

echo "Files: $(find "${dirname}" -type f 2>/dev/null | wc -l)"
echo "Dirs: $(find "${dirname}" -type d 2>/dev/null | wc -l)"

echo "All: $(find "${dirname}" 2>/dev/null | wc -l)"

echo "F&D: $(find "${dirname}" -mindepth 1 -type f -o -type d 2>/dev/null | wc -l)"
