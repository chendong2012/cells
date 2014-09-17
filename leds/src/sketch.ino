#include "public.h"
struct line {
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

struct pixel { 
        unsigned char r;
        unsigned char g;
        unsigned char b;
};

struct _rgb_line rgb_backup[H];

struct _rgb_line rgb_datas[H] = {
{0xff,0xff,0x03,0x00,
0x00,0x00,0xfc,0xff,
0x00,0x00,0x00,0x00},
{0xff,0xff,0x03,0x00,
0x00,0x00,0xfc,0xff,
0x00,0x00,0x00,0x00},
{0xff,0xff,0x03,0x00,
0x00,0x00,0xfc,0xff,
0x00,0x00,0x00,0x00},
{0xff,0xff,0x03,0x00,
0x00,0x00,0xfc,0xff,
0x00,0x00,0x00,0x00},
{0xff,0xff,0x03,0x00,
0x00,0x00,0xfc,0xff,
0x00,0x00,0x00,0x00},
{0xff,0xff,0x03,0x00,
0x00,0x00,0xfc,0xff,
0x00,0x00,0x00,0x00},
{0xff,0xff,0x03,0x00,
0x00,0x00,0xfc,0xff,
0x00,0x00,0x00,0x00},
{0xff,0xff,0x03,0x00,
0x00,0x00,0xfc,0xff,
0x00,0x00,0xfc,0xff},
{0xff,0xff,0x03,0x00,
0xff,0xff,0xff,0xff,
0x00,0x00,0xfc,0xff},
{0xff,0xff,0x03,0x00,
0xff,0xff,0xff,0xff,
0x00,0x00,0xfc,0xff},
{0xff,0xff,0x03,0x00,
0xff,0xff,0xff,0xff,
0x00,0x00,0xfc,0xff},
{0xff,0xff,0x03,0x00,
0xff,0xff,0xff,0xff,
0x00,0x00,0xfc,0xff},
{0xff,0xff,0x03,0x00,
0xff,0xff,0xff,0xff,
0x00,0x00,0xfc,0xff},
{0xff,0xff,0x03,0x00,
0xff,0xff,0xff,0xff,
0x00,0x00,0xfc,0xff},
{0xff,0xff,0x03,0x00,
0xff,0xff,0xff,0xff,
0x00,0x00,0xfc,0xff},
{0xff,0xff,0x03,0x00,
0xff,0xff,0xff,0xff,
0x00,0x00,0xfc,0xff}
};


static void shift_1bits(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2);
static void shift_8bits(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2);

void select_row(void);
void lock_data(void);
void init_gpio(void);
void init_rgb_datas(void);



static void shift_32bits(struct _rgb_line *rgb1,struct _rgb_line *rgb2);
static void update_32x16(void);
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
	Serial.print("pic1!");
	for(i=0;i<50;i++) {
		update_32x16();
	}
}

static void init_gpio(void)
{
	pinMode(R, OUTPUT);
	pinMode(G, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(R2, OUTPUT);
	pinMode(G2, OUTPUT);
	pinMode(B2, OUTPUT);
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

static void shift_1bits(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2)
{

        digitalWrite(SHCP, LOW);

        digitalWrite(R, r1&0x01);
        digitalWrite(G, g1&0x01);
        digitalWrite(B, b1&0x01);

        digitalWrite(R2, r2&0x01);
        digitalWrite(G2, g2&0x01);
        digitalWrite(B2, b2&0x01);

        digitalWrite(SHCP, HIGH);
}

static void shift_8bits(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2)
{
	unsigned char i;

	for(i=0; i<8; i++) {
		shift_1bits(r1,g1,b1,r2,g2,b2);
		r1>>=1;
		g1>>=1;
		b1>>=1;

		r2>>=1;
		g2>>=1;
		b2>>=1;
	}
}

static void select_row(unsigned char line)
{
        digitalWrite(ROW0, line&0x01);
        digitalWrite(ROW1, (line&0x02)>>1);
        digitalWrite(ROW2, (line&0x04)>>2);
}

static void shift_32bits(struct _rgb_line *rgb1, struct _rgb_line *rgb2)
{
	unsigned char i;
	for(i=0;i<4;i++) {
		shift_8bits(rgb1->r[i],\
			rgb1->g[i],\
			rgb1->b[i],\
			rgb2->r[i],\
			rgb2->g[i],\
			rgb2->b[i]);
	}
}

static void lock_data(void)
{
        digitalWrite(LOCK, 1);
        digitalWrite(LOCK, 0);
}

static void output_data(void)
{
        digitalWrite(OE, 0);
	delayMicroseconds(200);
        digitalWrite(OE, 1);
//	delayMicroseconds(500);
}

static struct _rgb_line rgb, rgb1;
static void update_32x16(void)
{
	unsigned char line;
	for(line=0; line<8; line++) {
		memcpy(rgb.r, (const void *)rgb_datas[line].r, 4);
		memcpy(rgb.g, (const void *)rgb_datas[line].g, 4);
		memcpy(rgb.b, (const void *)rgb_datas[line].b, 4);

		memcpy(rgb1.r, (const void *)rgb_datas[line+8].r, 4);
		memcpy(rgb1.g, (const void *)rgb_datas[line+8].g, 4);
		memcpy(rgb1.b, (const void *)rgb_datas[line+8].b, 4);
		shift_32bits(&rgb,&rgb1);
		select_row(line);
		lock_data();
		output_data();
	}
}

unsigned char byte_ofst(unsigned short pixel_index)
{
	return pixel_index/8;
}

static void setpixel(unsigned char x, unsigned char y, struct pixel p)
{
	unsigned char byteofst;
	unsigned char bitofst;
	unsigned char dat;

	if(x>(H-1) || y>(W-1)) return;
	byteofst = x/8;
	bitofst = x%8;

	dat = rgb_datas[y].r[byteofst];
	if (p.r==0)
		dat &=~(0x01<<bitofst);
	else
		dat |=(0x01<<bitofst);

	dat = rgb_datas[y].g[byteofst];
	if (p.g==0)
		dat &=~(0x01<<bitofst);
	else
		dat |=(0x01<<bitofst);

	dat = rgb_datas[y].b[byteofst];
	if (p.b==0)
		dat &=~(0x01<<bitofst);
	else
		dat |=(0x01<<bitofst);
}

static void draw_a_color_screen(struct pixel p)
{
	unsigned char x=0;
	unsigned char y=0;
	for (y=0;y<H;y++)
	for (x=0;x<W;x++)
		setpixel(x, y, p);
}

static void clear_screen(void)
{
	unsigned char i=0;
	for (i=0;i<H;i++) {
		memset(rgb_datas[i].r, 0x00, W/8);
		memset(rgb_datas[i].g, 0x00, W/8);
		memset(rgb_datas[i].b, 0x00, W/8);
	}
}

static void draw_a_line(unsigned char line, struct _rgb_line rgbline)
{
	memcpy(rgb_datas[line].r, rgbline.r, W/8);
	memcpy(rgb_datas[line].g, rgbline.g, W/8);
	memcpy(rgb_datas[line].b, rgbline.b, W/8);
}
