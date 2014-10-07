#include "zkcvt.h"

void zkcvt::setfg(unsigned char fg)
{
	_fg = fg;
}

void zkcvt::setbg(unsigned char bg)
{
	_bg = bg;
}

void zkcvt::write_pixel(unsigned char x, unsigned char y, unsigned char pixel)
{
	unsigned char color;
	struct pixel p;
	if (pixel&0x01)
		color = _fg;
	else 
		color = _bg;

	p.rbit = color&0x01;
	p.gbit = (color&0x02)>>1;
	p.bbit = (color&0x04)>>2;
	_fb_buf->fb_draw_pixel(x, y, p);
}

void zkcvt::write_block(unsigned char x, unsigned char y, unsigned char w, unsigned char h, const unsigned char *zk_raw)
{
	unsigned char i,j;
	for(i=0; i<h; i++) {

		/*write 8 pixels*/
		for(j=0; j<w; j++) {
			write_pixel(x+j, y+i, zk_raw[i]>>(7-j));
		}
	}
}
