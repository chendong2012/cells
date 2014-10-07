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

#define R 2
#define G 3
#define B 4

#define R2 5
#define G2 6
#define B2 7

#define SHCP 13
#define OE 8
#define	LOCK 9

#define ROW0	10
#define ROW1	11	
#define	ROW2	12

#define H 16
#define W 32
#define DIV_W W/8


#define R_COLOR 0
#define G_COLOR 1
#define B_COLOR 2

struct _rgb_line { 
	unsigned char r[8];
	unsigned char g[8];
	unsigned char b[8];
};

struct _rgb_8pixels { 
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct pixel { 
	unsigned char rbit;
	unsigned char gbit;
	unsigned char bbit;
};

struct raw_8x16 {
	unsigned char dat[16];
};

struct _raw_8pixels
{
	unsigned char dat;
};

#endif
