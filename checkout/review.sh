#!/bin/bash
#into mtkprojects directory
cd "$1"
prj_info="prj_info"
allreview=(`find -maxdepth 1 -type d | sed -n '/review$/p' | sed 's/\.\///'`)
for prjdirname in "$allreview[@]"; do
	cd "$prjdirname"
	prjpath=`getitem.sh "$prj_info" "prjpath"`
	project=`getitem.sh "$prj_info" "project"`
	cd "$prjpath"
	./mk  $prj n
	reviewoutput=`getitem.sh "$prj_info" "reviewoutput"`

	if [[ "$reviewoutput" != "" ]]; then
		reviewoutput.sh "$PWD" "$prjdirname" "$reviewoutput"
	fi
done
