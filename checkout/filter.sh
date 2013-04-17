#!/bin/bash
##############################
TAG="[filter.sh]"
save_prj_num=1
##############################

projectspath=`getitem.sh "$1/public" "projectspath"`
if [ -d "$projectspath" ]; then
	cd "$projectspath"
else
	echo "$TAG:no found directory to ops"
	exit
fi

#找到所有项
a=(`find -maxdepth 1 -type d |  cut -d"-" -f1,2 | sort | uniq | sed -n 's/^\.\///p'`)
#找到每个项的所有目录
i=${#a[*]}
echo "$TAG:number of projects:$i"
echo "$TAG:${a[*]}"
echo "$TAG:================="
for ((j=0;j<i;j++)) {
	error_item=(`find -maxdepth 1 -type d | grep "${a[$j]}" | sed -n 's/\.\///p' | sed -n '/error$/p'`)
	ok_item=(`find -maxdepth 1 -type d | grep "${a[$j]}" | sed -n 's/\.\///p' | sed -n '/ok$/p'`)
	ok_item_date=(`find -maxdepth 1 -type d | grep "${a[$j]}" | sed -n 's/\.\///p' | sed -n '/ok$/p' | cut -d"-" -f3 | cut -d"_" -f1,2 | sed 's/_//'`)
	#delete error directory
	if [ ${#error_item[*]} -gt 0 ]; then
		echo "$TAG:delete error directory"
		echo "$TAG:rm -rf ${error_item[*]}"
		rm -rf "${error_item[*]}" &
	fi

	#进行分析，处理是否要del
	if [ "${#ok_item_date[*]}" -lt 1 ]; then
		echo "$TAG:no found the project"
		continue
	fi 
	    sorted=($(printf '%s\n' "${ok_item_date[@]}"|sort))
		for ((s=0;s<"${#sorted[*]}"-"$save_prj_num";s++)) {
		tmp=`echo "${sorted[$s]}" | sed -n 's/\(.*\)\([0-9][0-9][0-9][0-9]$\)/\1_\2/p'`
			for((r=0;r<${#ok_item[*]};r++)) {
#				echo "echo "${ok_item[$r]}" | grep "$tmp""
				myrm=`echo "${ok_item[$r]}" | grep "$tmp"`
				if [ -d "$myrm" ]; then
					echo "$TAG:rm -rf "$myrm""
					rm -rf "$myrm" &
				fi
			}
		}
}
