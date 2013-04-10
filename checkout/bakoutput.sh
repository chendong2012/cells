#!/bin/bash
#ok	-->ftp/project/record-dir/{summit.txt diff.txt and image packet}
#error	-->ftp/project/record-dir/{summith.txt diff.txt}
#

svndate={`date +%F --date="-1 day"`}:{`date +%Y-%m-%d\ %H:%M:%S`}
summit_detail=summit_detail_`date +%F --date="-1 day"`_`date +%F_%H%M%S`.txt
summit_summarize=summit_summarize_`date +%F --date="-1 day"`_`date +%F_%H%M%S`.txt
diff=diff_`date +%F --date="-1 day"`_`date +%F_%H%M%S`.txt

compiledir="$1"
prjdirname="$2"
svn_android="$3"
output="$4"

ftp_prj_ok_dir="$output"/"$prjdirname"-compile_ok
ftp_prj_error_dir="$output"/"$prjdirname"-compile_error

echo "=====================backup output begin==================="
cd "$compiledir"

find release -name system.img
keystr=`find release -name system.img | sed -n 's/\(.*\)\(system.img.*\)/\2/p'`

if [[ "$keystr" == "system.img" ]]; then
	cd release
	mkdir -p "$ftp_prj_ok_dir"
	echo "$*" >> "$ftp_prj_ok_dir"/"$summit_detail"
	echo "$*" >> "$ftp_prj_ok_dir"/"$summit_summarize"
	svn log -v -r "$svndate" "$svn_android" >> "$ftp_prj_ok_dir"/"$summit_detail"
	svn log -r "$svndate" "$svn_android" >> "$ftp_prj_ok_dir"/"$summit_summarize"
	svn diff -r "$svndate" "$svn_android" >> "$ftp_prj_ok_dir"/"$diff"
	cp *.tar.gz "$ftp_prj_ok_dir"
else
	mkdir -p "$ftp_prj_error_dir"/errlog
	cp out/target/product/*.log "$ftp_prj_error_dir"/errlog
	cp out/target/product/*.log_err "$ftp_prj_error_dir"/errlog
	echo "$*" >> "$ftp_prj_error_dir"/"$summit_detail"
	echo "$*" >> "$ftp_prj_error_dir"/"$summit_summarize"
	svn log -v -r "$svndate" "$svn_android" >> "$ftp_prj_error_dir"/"$summit_detail"
	svn log -r "$svndate" "$svn_android" >> "$ftp_prj_error_dir"/"$summit_summarize"
	svn diff -r "$svndate" "$svn_android" >> "$ftp_prj_error_dir"/"$diff"
fi
echo "=====================backup output end==================="
