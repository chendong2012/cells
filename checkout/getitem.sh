#!/bin/bash
#形参$1 表示完整的路径名，$2表示对应项的名字，返回对应项的内容
ctx=(`cat "$1" | sed -n "s/^$2=//p" | cut -d, -f1`)
echo "${ctx[0]}"
