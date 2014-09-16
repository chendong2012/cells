#ifndef __PUBLIC__
#define __PUBLIC__
#include <stdio.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <avr/pgmspace.h>
#include "pins_arduino.h"

#define R 4
#define G 5
#define B 6

#define SHCP 7
#define OE 8
#define	LOCK 9

#define ROW0	10
#define ROW1	11	
#define	ROW2	12

#define H 16
#define W 32
#define DIV_W W/8


#endif
