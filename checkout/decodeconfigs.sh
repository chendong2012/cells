#!/bin/bash
#items里面的内容表示android项目所在的行号
#current_http_line_index:表示当前项目起始行
#next_http_line_index:表示下一个项目起始行
#items[] 组成
#0	内容为项目开始行号
#1	内容为项目开始行号
#2	内容为项目开始行号
#3	内容为项目开始行号
swappath=${HOME}/.checkout
configfile=${HOME}/configs

if [ -d "$swappath" ]; then
	rm -rf "$swappath"
fi
mkdir "$swappath"

bb=(`cat $configfile | sed '/^$/d;/^#/d'`)
items=(`cat $configfile | sed '/^$/d;/^#/d' | sed -n '/android=/='`)
public_tail=$((${items[0]}-1))

cat $configfile | sed '/^$/d;/^#/d' | sed -n "1,${public_tail}p" >>  "$swappath"/public

item_index=${#items[*]}
current_context=(null null)
#计算项目数
prjnum=$item_index

echo =========================================
echo number of projects:$prjnum
for ((i=0;i<$item_index;i+=1)) {
	mkdir "$swappath"/"$i"
	echo =========================================
	((temp=$i+1)) #items索引加二，到下一个项目的items索引
	if [ $temp -ge $item_index ]; then #超出数组索引,已到最后一个项目,项目内容到文件最后
		next_http_line_index=${#bb[*]}
	else
		next_http_line_index=${items[$temp]}
		((next_http_line_index--))
	fi
		current_http_line_index=${items[$i]}
		((current_http_line_index--))
	ctxidx=0
	for ((j=$current_http_line_index;j<$next_http_line_index;j++)) {
		current_context[$ctxidx]="${bb[$j]}"
		((ctxidx++))
	}
	for ((m=0;m<${#current_context[*]};m++))	{
		echo ${current_context[$m]}
		echo ${current_context[$m]} >> "$swappath"/"$i"/"$i"
	}
}
echo =========================================
