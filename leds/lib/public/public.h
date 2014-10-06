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
#if 0
#define R 4
#define G 5
#define B 6

#define R2 13
#define G2 14
#define B2 15
#endif

#if 1
#define R 2
#define G 3
#define B 4

#define R2 5
#define G2 6
#define B2 7
#endif

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








struct line123 {
	unsigned char f0;
	unsigned char f1;
	unsigned char f2;
	unsigned char f3;
};

struct _rgb_line { 
	unsigned char r[8];
	unsigned char g[8];
	unsigned char b[8];
};

struct _rgb_double_line { 
	unsigned char r[8];
	unsigned char g[8];
	unsigned char b[8];
};


struct _rgb_half_line { 
	unsigned char r[2];
	unsigned char g[2];
	unsigned char b[2];
};

struct _rgb_8points { 
	unsigned char r;
	unsigned char g;
	unsigned char b;
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
	unsigned char datas[16];
};
#endif
