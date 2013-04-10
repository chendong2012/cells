#!/bin/bash

svndate={`date +%F --date="-1 day"`}:{`date +%Y-%m-%d\ %H:%M:%S`}
summit_detail=summit_detail_`date +%F --date="-1 day"`_`date +%F_%H%M%S`.txt
summit_summarize=summit_summarize_`date +%F --date="-1 day"`_`date +%F_%H%M%S`.txt
diff=diff_`date +%F --date="-1 day"`_`date +%F_%H%M%S`.txt

compiledir="$1"
prjdirname="$2"
svn_android="$3"


echo "=====================package begin==================="
cd "$compiledir"

find release -name system.img
keystr=`find release -name system.img | sed -n 's/\(.*\)\(system.img.*\)/\2/p'`

if [[ "$keystr" == "system.img" ]]; then
	cd release
	echo "$*" > `ls`/$summit_detail
	echo "$*" > `ls`/$summit_summarize
	svn log -v -r "$svndate" "$svn_android" >> `ls`/"$summit_detail"
	svn log -r "$svndate" "$svn_android" >> `ls`/"$summit_summarize"
	svn diff -r "$svndate" "$svn_android" >> `ls`/"$diff"
	tar czvf "$prjdirname.tar.gz" `ls`
fi
echo "=====================package end==================="
