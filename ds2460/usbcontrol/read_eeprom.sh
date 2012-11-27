#!/bin/bash
i=0
while ((i<14)); do
	./uwaycontrol -r eeprom -p $i -l 8
	let i++
done
