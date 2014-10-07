#include "zkcvt.h"

void zkcvt::setfg(unsigned char fg)
{
	_fg = fg;
}

void zkcvt::setbg(unsigned char bg)
{
	_bg = bg;
}

void zkcvt::convert_pixel(unsigned char raw_bit, struct pixel *p)
{
	unsigned char color;
	if (raw_bit&0x01)
		color = _fg;
	else 
		color = _bg;

	p->rbit = color&0x01;
	p->gbit = (color&0x02)>>1;
	p->bbit = (color&0x04)>>2;
}

/*
 *		/---r---\
 *	zk_raw/-----g-----_rgb_8pixels
 *		\---b---/
 * */
void zkcvt::convert_8_pixels(const unsigned char zk_raw, struct _rgb_8pixels *rgb)
{
	char i;
	struct pixel p;
	rgb->r=0;
	rgb->g=0;
	rgb->b=0;

/*must clear rgb*/
	for(i=7;i>=0;i--) {
		convert_pixel(zk_raw>>i, &p);
		rgb->r|=(p.rbit<<i);
		rgb->g|=(p.gbit<<i);
		rgb->b|=(p.bbit<<i);
	}
}
