#include "hw.h"
#include "fb.h"

extern struct _rgb_line fb1[H];
void hw::hw_write_pixel(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2)
{
	unsigned char datas=0;
	digitalWrite(SHCP, LOW);

	datas|=((r1&0x80)>>5);
	datas|=((g1&0x80)>>4);
	datas|=((b1&0x80)>>3);
	datas|=((r2&0x80)>>2);
	datas|=((g2&0x80)>>1);
	datas|=((b2&0x80)>>0);
	PORTD=datas;

	digitalWrite(SHCP, HIGH);
}


void hw::hw_write_8pixels(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2)
{
	unsigned char i;

	for(i=0; i<8; i++) {
		hw_write_pixel(r1,g1,b1,r2,g2,b2);
		r1<<=1;
		g1<<=1;
		b1<<=1;

		r2<<=1;
		g2<<=1;
		b2<<=1;
	}
}

void hw::hw_write_line(struct _rgb_line *rgb1, struct _rgb_line *rgb2)
{
	unsigned char i;
	for(i=0;i<W/8;i++) {
		hw_write_8pixels(rgb1->r[i],\
				rgb1->g[i],\
				rgb1->b[i],\
				rgb2->r[i],\
				rgb2->g[i],\
				rgb2->b[i]);
	}
}


void hw::hw_select_row(unsigned char line)
{
	digitalWrite(ROW0, line&0x01);
	digitalWrite(ROW1, (line&0x02)>>1);
	digitalWrite(ROW2, (line&0x04)>>2);
}

void hw::hw_lock_data(void)
{
	digitalWrite(LOCK, 1);
	digitalWrite(LOCK, 0);
}

void hw::hw_output_data(void)
{
	digitalWrite(OE, 0);
	delayMicroseconds(HW_DISP_DELAY);
	digitalWrite(OE, 1);
}

void hw::hw_write_screen(void)
{
	unsigned char line;
	for(line=0; line<8; line++) {
		hw_write_line(FB.fb_line_addr(line),FB.fb_line_addr(line+8));
		hw_select_row(line);
		hw_lock_data();
		hw_output_data();
	}
}
hw HW;
