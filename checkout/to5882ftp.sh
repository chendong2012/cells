#!/bin/bash
#ok	-->ftp/project/record-dir/{summit.txt diff.txt and image packet}
#error	-->ftp/project/record-dir/{summith.txt diff.txt}
#

svndate={`date +%F --date="-1 day"`}:{`date +%Y-%m-%d\ %H:%M:%S`}
summit_detail=summit_detail_`date +%F --date="-1 day"`_`date +%F_%H%M%S`.txt
summit_summarize=summit_summarize_`date +%F --date="-1 day"`_`date +%F_%H%M%S`.txt
diff=diff_`date +%F --date="-1 day"`_`date +%F_%H%M%S`.txt
ftp5882=/home/public/ftp_root/drvbuild/WBW5882

PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/lib/jvm/java-6-sun/jre/bin:/usr/lib/jvm/java-6-sun/bin:~/bin:/opt/java/jdk1.6.0_26/jre/bin

echo *********************to5882ftp begin*********************** >> ~/checkout_log

find release -name system.img >> ~/checkout_log
keystr=`find release -name system.img | sed -n 's/\(.*\)\(system.img.*\)/\2/p'`
if [[ "$keystr" == "system.img" ]]; then
	cd release
	echo "$1" "$2" >> ~/checkout_log
	echo "$2" >> `ls`/$summit_detail
	echo "$2" >> `ls`/$summit_summarize

	svn log -v -r "$svndate" "$2" >> `ls`/$summit_detail
	svn log -r "$svndate" "$2" >> `ls`/$summit_summarize
	svn diff -r "$svndate" "$2" >> `ls`/$diff
	keystr=`echo "$1" | sed 's/\(.*\)\(5882\)\(.*\)/\2/'`
#	keystr=`ls | grep 5882`
#	echo "$keystr" >> ~/checkout_log
	echo "$keystr-------------------------------------------------------" >> ~/checkout_log
	if [[ "$keystr" == "5882" ]]; then
		echo =================ok >> ~/checkout_log
		mkdir -p $ftp5882/$1-compile-ok

		echo "$2" >> $ftp5882/$1-compile-ok/$summit_detail
		echo "$2" >> $ftp5882/$1-compile-ok/$summit_summarize

		svn log -v -r "$svndate" "$2" >> $ftp5882/$1-compile-ok/$summit_detail
		svn log -r "$svndate" "$2" >> $ftp5882/$1-compile-ok/$summit_summarize
		svn diff -r "$svndate" "$2" >> $ftp5882/$1-compile-ok/$diff
		tar czvf "$1.tar.gz" `ls`
		cp *.tar.gz $ftp5882/$1-compile-ok
	else 
		echo ========$keystr=========err1 >> ~/checkout_log
	fi
else
	echo =================compile err================ >> ~/checkout_log
	mkdir -p $ftp5882/$1-compile-fail/errlog

	ls out/target/product >> ~/checkout_log
	cp out/target/product/*.log $ftp5882/$1-compile-fail/errlog 2>>~/checkout_log
	cp out/target/product/*.log_err $ftp5882/$1-compile-fail/errlog 2>>~/checkout_log

	echo "$2" >> $ftp5882/$1-compile-fail/$summit_detail
	echo "$2" >> $ftp5882/$1-compile-fail/$summit_summarize
	svn log -v -r "$svndate" "$2" >> $ftp5882/$1-compile-fail/$summit_detail
	svn log -r "$svndate" "$2" >> $ftp5882/$1-compile-fail/$summit_summarize
	svn log -v -r "$svndate" "$2" 2>>~/checkout_log

	svn diff -r "$svndate" "$2" >> $ftp5882/$1-compile-fail/$diff
fi
	echo "====================end to5882ftp==============" >> ~/checkout_log
