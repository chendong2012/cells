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
#include <EEPROM.h>

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
#define LINE_BYTES 8

#define DIV_W W/8


#define R_COLOR 0
#define G_COLOR 1
#define B_COLOR 2

#define FONT_WIDTH 	8
#define FONT_HEIGHT	16

#define TABLE_MEMORY	0
#define TABLE_EEPROM	1
#define TABLE_FLASH	2

struct mix_table {
	unsigned char flag;
	void *addr;
	unsigned char count;
};

struct _rgb_line { 
	unsigned char r[LINE_BYTES];
	unsigned char g[LINE_BYTES];
	unsigned char b[LINE_BYTES];
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
