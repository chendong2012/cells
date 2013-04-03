#!/bin/bash
find release -name system.img >> ~/checkout_log
keystr=`find release -name system.img | sed -n 's/\(.*\)\(system.img.*\)/\2/p'`
if [[ "$keystr" == "system.img" ]]; then
	echo "compile-ok"
	cd $1
	mv $2 $2-compile-ok
else
	cd $1
	echo "comiple-err"
	mv $2 $2-compile-error
fi
