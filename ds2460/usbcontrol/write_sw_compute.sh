#!/bin/bash
datas=(
		1 2 3 4 5 6 7 8	#inputdata 0~7
		1 2 3 4 5 6 7 8	#inputdata 8~15
		1 2 3 4 5 6 7 8 #inputdata 16~23
		1 2 3 4 5 6 7 8	#inputdata 24~31
		1 2 3 4 5 6 7 8	#inputdata 32~39
		1 2 3 4 5 6 7 8	#inputdata 40~47
		1 2 3 4 5 6 7 8	#inputdata 48~55
		1 2 3 4 5 6 7 8	#inputdata 56~63
		1 2 3 4 5 6 7 8	#secrect
	   )
./clewarecontrol -w sw_compute -l 72 "${datas[@]}"
