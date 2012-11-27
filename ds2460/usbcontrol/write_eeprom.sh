#!/bin/bash
i=0
array=(
	'0x99 2 30 4 5 6 7 8'	#page0
	'0x56 2 3 4 5 6 7 8'	#page1
	'1 2 3 4 5 6 7 8'	#page2
	'1 2 3 4 5 6 7 8'	#page3
	'1 2 3 4 5 6 7 8'	#page4
	'1 2 3 4 5 6 7 8'	#page5
	'1 2 3 4 5 6 7 8'	#page6
	'1 2 3 4 5 6 7 8'	#page7
	'1 2 3 4 5 6 7 8'	#page8
	'1 2 3 4 5 6 7 8'	#page9
	'1 2 3 4 5 6 7 8'	#page10
	'1 2 3 4 5 6 7 8'	#page11
	'1 2 3 4 5 6 7 8'	#page12
	'0x11 0x22 0x33 0x44  0x55  0x66  0x77  0x88'	#page13
	)
for mm in "${array[@]}"
do
	./uwaycontrol -w eeprom -p $i -l 8 $mm
	let i++
done
