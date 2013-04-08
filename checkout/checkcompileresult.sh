#!/bin/bash
keystr=`find release -name system.img | sed -n 's/\(.*\)\(system.img.*\)/\2/p'`
if [[ "$keystr" == "system.img" ]]; then
	echo "compile_ok"
	cd "$1"
	mv "$2" "$2"-compile_ok
else
	cd "$1"
	echo "comiple_error"
	mv "$2" "$2"-compile_error
fi
