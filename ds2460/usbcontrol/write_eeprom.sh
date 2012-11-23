#!/bin/bash
i=0
w_p0=(0 1 2 3 4 5 6 7 8 9)
w_p1=(0 1 2 3 4 5 6 7 8 9)
w_p2=(0 1 2 3 4 5 6 7 8 9)
w_p3=(0 1 2 3 4 5 6 7 8 9)
w_p4=(0 1 2 3 4 5 6 7 8 9)
w_p5=(0 1 2 3 4 5 6 7 8 9)
w_p6=(0 1 2 3 4 5 6 7 8 9)
while ((i<11)); do
	./clewarecontrol -w eeprom -p $i -l 8 ${w_p0[*]}
		let i++
done
