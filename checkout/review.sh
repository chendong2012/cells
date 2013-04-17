#!/bin/bash
#into mtkprojects directory
mtkprojects=/media/disk5/chend/mtkprojects
cd "$mtkprojects"
echo "$mtkprojects"
prj_info=".prj_info"
allreview=(`find -maxdepth 1 -type d | sed -n '/review$/p' | sed 's/\.\///'`)
for prjdirname in "${allreview[@]}"; do
	echo $prjdirname
	cd "$prjdirname"
	
	prjpath=`getitem.sh "$prj_info" "prjpath"`
	project=`getitem.sh "$prj_info" "project"`
	reviewoutput=`getitem.sh "$prj_info" "reviewoutput"`

	cd "$prjpath"
	echo current $PWD
	rm -rf release
	./mk  "$project" n
	echo  output is $reviewoutput

	if [[ "$reviewoutput" != "" ]]; then
		reviewoutput.sh "$PWD" "$prjdirname" "$reviewoutput"
	fi
	cd "$mtkprojects"
done
