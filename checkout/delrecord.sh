#!/bin/bash
#$1 表示要操作的目录
#$2表示要生成的记录文件所在的目录
find $1 --maxdepth 0 -atime +1 >> $2/delrecord
find $1 --maxdepth 0  -atime +1 | xargs rm -rf
