#!/bin/bash
i=0
while ((i<14)); do
	./clewarecontrol -r eeprom -p $i -l 8
	let i++
done
