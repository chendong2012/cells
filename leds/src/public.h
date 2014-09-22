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

#define R2 13
#define G2 14
#define B2 15


#define SHCP 7
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








struct line123 {
	unsigned char f0;
	unsigned char f1;
	unsigned char f2;
	unsigned char f3;
};

struct _rgb_line { 
	unsigned char r[4];
	unsigned char g[4];
	unsigned char b[4];
};

struct _rgb_half_line { 
	unsigned char r[2];
	unsigned char g[2];
	unsigned char b[2];
};

struct _rgb_quarter_line { 
	unsigned char r;
	unsigned char g;
	unsigned char b;
};
struct pixel { 
	unsigned char r;
	unsigned char g;
	unsigned char b;
};





#endif
