#include "public.h"

struct line {
        unsigned char f0;
        unsigned char f1;
        unsigned char f2;
        unsigned char f3;
};
struct rgb_line {
        unsigned char r[4];
        unsigned char g[4];
        unsigned char b[4];
};
static struct rgb_line rgb_datas[H] = {
/*1line*/
{0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00},
/*2 line*/
{0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00},
/*3 line blue*/
{0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff},
/*4 line*/
{0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff},
/*5 line*/
{0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff},
/*6 line*/
{0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00},
/*7 line*/
{0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00},
/*8 line*/
{0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff},

/*9 line*/
{0x0f,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0x0f},

/*10 line*/
{0xf0,0xf0,0xf0,0xf0,
0xf0,0xf0,0xf0,0xf0,
0xf0,0xf0,0xf0,0xf0},

/*11 line*/
{0xff,0x00,0xf0,0x00,
0xff,0x00,0xf0,0x00,
0xff,0x00,0xf0,0x00},

/*12 line*/
{0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55},

/*13 line*/
{0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55},

/*14 line*/
{0x55,0x55,0xAA,0xAA,
0x55,0x55,0xAA,0xAA,
0x55,0x55,0xAA,0xAA},

/*15 line*/
{0xAA,0xAA,0x55,0x55,
0xAA,0xAA,0x55,0x55,
0xAA,0xAA,0x55,0x55},
/*16 line*/
{0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00}
};

void shift_1bits(unsigned char r, unsigned char g, unsigned char b);
void shift_8bits(unsigned char r, unsigned char g, unsigned char b);

static void update_32x16_1(void);
static void update_32x16_2(void);
static void update_32x16_3(void);
static void update_32x16_4(void);

void select_row(void);
void lock_data(void);
void update_32x16(void);
void init_gpio(void);
void init_rgb_datas(void);

typedef struct
{ 
  unsigned char bit0:1; 
  unsigned char bit1:1; 
  unsigned char bit2:1; 
  unsigned char bit3:1; 
  unsigned char bit4:1; 
  unsigned char bit5:1; 
  unsigned char bit6:1; 
  unsigned char bit7:1; 
} _io_reg;


typedef struct {
  byte pin : 6;
  byte mode : 1;
  byte state : 1;
} DigitalPin;

void init_serial(void)
{
	Serial.begin(115200);
	Serial.print("begin!");
}
void setup()
{
	init_serial();
	init_gpio();
	init_rgb_datas();
}
void loop()
{
	unsigned short i=0;
	for(i=0;i<500;i++) {
		update_32x16_1();
	}

	for(i=0;i<500;i++) {
		update_32x16_2();
	}

	for(i=0;i<500;i++) {
		update_32x16_3();
	}

	for(i=0;i<500;i++) {
		update_32x16_4();
	}
}

static void init_gpio(void)
{
	pinMode(R, OUTPUT);
	pinMode(G, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(SHCP, OUTPUT);
	pinMode(OE, OUTPUT);
	pinMode(LOCK, OUTPUT);
	pinMode(ROW0, OUTPUT);
	pinMode(ROW1, OUTPUT);
	pinMode(ROW2, OUTPUT);
}

static void init_rgb_datas(void)
{
	unsigned char i;
}
static void shift_1bits(unsigned char r, unsigned char g, unsigned char b)
{
        digitalWrite(SHCP, LOW);

        digitalWrite(R, r&0x01);
        digitalWrite(G, g&0x01);
        digitalWrite(B, b&0x01);


        digitalWrite(SHCP, HIGH);
}

static void shift_8bits(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char k;
	for(k=0; k<8; k++) {
		shift_1bits(r,g,b);
		r>>=1;
		g>>=1;
		b>>=1;
	}
}

static void select_row(unsigned char line)
{
        digitalWrite(ROW0, line&0x01);
        digitalWrite(ROW1, line&0x02);
        digitalWrite(ROW2, line&0x04);
}

static void shift_32bits(unsigned char *r, unsigned char *g, unsigned char *b)
{
	unsigned char i;
	for(i=0;i<4;i++)
		shift_8bits(r[i], g[i], b[i]);
}

static void shift_16bits(unsigned char *r, unsigned char *g, unsigned char *b)
{
	unsigned char i;
	for(i=0;i<2;i++)
		shift_8bits(r[i], g[i], b[i]);
}

static void lock_data(void)
{
        digitalWrite(LOCK, 1);
        digitalWrite(LOCK, 0);
}

static void output_data(void)
{
        digitalWrite(OE, 0);
	delayMicroseconds(500);
        digitalWrite(OE, 1);
//	delayMicroseconds(500);
}

static void update_32x16_1(void)
{
	unsigned char line;
	for(line=0; line<H; line++) {
		shift_32bits(rgb_datas[line].r, rgb_datas[line].g, rgb_datas[line].b);
		select_row(line);
		lock_data();
		output_data();
	}
}

static void update_32x16_2(void)
{
	unsigned char line;
	for(line=0; line<H; line++) {
		shift_32bits(rgb_datas[15-line].r, rgb_datas[15-line].g, rgb_datas[15-line].b);
		select_row(line);
		lock_data();
		output_data();
	}
}

static void update_32x16_3(void)
{
	unsigned char line;
	for(line=0; line<H; line++) {
		shift_32bits(rgb_datas[line].g, rgb_datas[line].r, rgb_datas[line].b);
		select_row(line);
		lock_data();
		output_data();
	}
}

static void update_32x16_4(void)
{
	unsigned char line;
	for(line=0; line<H; line++) {
		shift_32bits(rgb_datas[line].b, rgb_datas[line].g, rgb_datas[line].r);
		select_row(line);
		lock_data();
		output_data();
	}
}
