#include "zkcvt.h"

void zkcvt::set_fg(unsigned char fg)
{
	_fg = fg;
}

void zkcvt::set_bg(unsigned char bg)
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

void zkcvt::convert_8_pixels(const struct _raw_8pixels *raw, struct _rgb_8pixels *rgb, unsigned char fg, unsigned char bg)
{
	char i;
	struct pixel p;

/*must clear rgb*/
	for(i=7;i>=0;i--) {
		convert_pixel(raw->dat>>i, &p);
		rgb->r|=(p.r<<i);
		rgb->g|=(p.g<<i);
		rgb->b|=(p.b<<i);
	}
}
