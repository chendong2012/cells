#!/bin/bash
#ok	-->ftp/project/record-dir/{summit.txt diff.txt and image packet}
#error	-->ftp/project/record-dir/{summith.txt diff.txt}
#
compiledir="$1"
prjdirname="$2"
output="$3"

ftp_prj_ok_dir="$output"/"$prjdirname"-review_ok
ftp_prj_error_dir="$output"/"$prjdirname"-review_error

echo "=====================review output begin==================="
cd "$compiledir"

find release -name system.img
keystr=`find release -name system.img | sed -n 's/\(.*\)\(system.img.*\)/\2/p'`

if [[ "$keystr" == "system.img" ]]; then
	cd release
	mkdir -p "$ftp_prj_ok_dir"
	cp *.tar.gz "$ftp_prj_ok_dir"
else
	mkdir -p "$ftp_prj_error_dir"/errlog
	cp out/target/product/*.log "$ftp_prj_error_dir"/errlog
	cp out/target/product/*.log_err "$ftp_prj_error_dir"/errlog
fi
echo "=====================review output end==================="
