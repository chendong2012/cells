#!/bin/bash
#找到所有项
a=(`find -maxdepth 1 -type d |  cut -d"-" -f1,2 | sort | uniq | sed -n 's/^\.\///p'`)
#找到每个项的所有目录
i=${#a[*]}
echo ${a[*]}
echo all directory:$i
echo =================
for ((j=0;j<i;j++)) {
	error_item=(`find -maxdepth 1 -type d | grep "${a[$j]}" | sed -n 's/\.\///p' | grep error`)
	ok_item=(`find -maxdepth 1 -type d | grep "${a[$j]}" | sed -n 's/\.\///p' | grep ok  `)
	ok_item_date=(`find -maxdepth 1 -type d | grep "${a[$j]}" | sed -n 's/\.\///p' | grep ok  | cut -d"-" -f3 | sed 's/_//'`)
	#delete error directory
	if [ ${#error_item[*]} -gt 0 ]; then
		echo "delete error directory"			
		echo "rm -rf ${error_item[*]}"
	fi

	#进行分析，处理是否要del
	if [ ${#ok_item_date[*]} -gt 1 ]; then
	    sorted=($(printf '%s\n' "${ok_item_date[@]}"|sort))
		for ((s=0;s<2;s++)) {
		tmp=`echo "${sorted[$s]}" | sed -n 's/\(.*\)\([0-9][0-9][0-9][0-9]$\)/\1_\2/p'`
			for((r=0;r<${#ok_item[*]};r++)) {
#				echo "echo "${ok_item[$r]}" | grep "$tmp""
				myrm=`echo "${ok_item[$r]}" | grep "$tmp"`
				if [ -d "$myrm" ]; then
					echo "rm -rf "$myrm""
					\rm -rf "$myrm"
				fi 
			}
		}
	fi
}
