#!/bin/bash
#ok	-->ftp/project/record-dir/{summit.txt diff.txt and image packet}
#error	-->ftp/project/record-dir/{summith.txt diff.txt}
#

svndate={`date +%F --date="-1 day"`}:{`date +%Y-%m-%d\ %H:%M:%S`}
summit_detail=summit_detail_`date +%F --date="-1 day"`_`date +%F_%H%M%S`.txt
summit_summarize=summit_summarize_`date +%F --date="-1 day"`_`date +%F_%H%M%S`.txt
diff=diff_`date +%F --date="-1 day"`_`date +%F_%H%M%S`.txt
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/lib/jvm/java-6-sun/jre/bin:/usr/lib/jvm/java-6-sun/bin:~/bin:/opt/java/jdk1.6.0_26/jre/bin

compiledir="$1"
prjdirname="$2"
svn_android="$3"
ftppath="$4"

ftp_prj_ok_dir="$ftppath"/"$prjdirname"-compile_ok
ftp_prj_fail_dir="$ftppath"/"$prjdirname"-compile_fail

echo "=====================toftp begin==================="
cd "$compiledir"

find release -name system.img
keystr=`find release -name system.img | sed -n 's/\(.*\)\(system.img.*\)/\2/p'`

if [[ "$keystr" == "system.img" ]]; then
	cd release
	echo "$*" >> `ls`/$summit_detail
	echo "$*" >> `ls`/$summit_summarize

	svn log -v -r "$svndate" "$svn_android" >> `ls`/$summit_detail
	svn log -r "$svndate" "$svn_android" >> `ls`/$summit_summarize
	svn diff -r "$svndate" "$svn_android" >> `ls`/$diff

	mkdir -p $ftp_prj_ok_dir
	echo "$*" >> $ftp_prj_ok_dir/$summit_detail
	echo "$*" >> $ftp_prj_ok_dir/$summit_summarize
	svn log -v -r "$svndate" $svn_android >> $ftp_prj_ok_dir/$summit_detail
	svn log -r "$svndate" $svn_android >> $ftp_prj_ok_dir/$summit_summarize
	svn diff -r "$svndate" $svn_android >> $ftp_prj_ok_dir/$diff
	tar czvf "$prjdirname.tar.gz" `ls`
	cp *.tar.gz $ftp_prj_ok_dir

else
	mkdir -p $ftp_prj_fail_dir/errlog
	cp out/target/product/*.log $ftp_prj_fail_dir/errlog 2>>~/checkout_log
	cp out/target/product/*.log_err $ftp_prj_fail_dir/errlog 2>>~/checkout_log
	echo "$*" >> $ftp_prj_fail_dir/$summit_detail
	echo "$*" >> $ftp_prj_fail_dir/$summit_summarize
	svn log -v -r "$svndate" $svn_android  >> $ftp_prj_fail_dir/$summit_detail
	svn log -r "$svndate" $svn_android >> $ftp_prj_fail_dir/$summit_summarize
	svn diff -r "$svndate" $svn_android >> $ftp_prj_fail_dir/$diff
fi
echo "=====================toftp end==================="
