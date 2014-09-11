#include "public.h"
static unsigned char r[4];
static unsigned char g[4];
static unsigned char b[4];

void shift_1bits(unsigned char r, unsigned char g, unsigned char b);
void shift_8bits(unsigned char r, unsigned char g, unsigned char b);
void shift_32bits(unsigned char *r, unsigned char *g, unsigned char *b);
void select_row(void);
void lock_data(void);
void update_32x16(void);
void init_gpio(void);
void init_rgb_datas(void);

void setup()
{
	init_gpio();
	init_rgb_datas();
}
void loop()
{
	update_32x16();
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
	for(i=0;i<4;i++) {
		r[i] = 0x55;
		g[i] = 0x65;
		b[i] = 0x78;
	}
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

static void lock_data(void)
{
        digitalWrite(LOCK, 1);
        digitalWrite(LOCK, 0);
}

static void output_data(void)
{
        digitalWrite(OE, 0);
	delay(1);
        digitalWrite(OE, 1);
	delay(1);
}

static void update_32x16(void)
{
	unsigned char line;
	for(line=0; line<SCREEN_HEIGHT; line++) {
		shift_32bits(r,g,b);
		select_row(line);
		lock_data();
		output_data();
	}
}
