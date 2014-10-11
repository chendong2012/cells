#include "fb.h"
/*64 x 16
 *screen display order-->struct _rgb_line
 *        [0]  [1]  [2]  [3]  [4]  [5]  [6]  [7]
 *line1.r:0x10 0x22 0x44 0x21 0x54 0x65 0x77 0x55
 *line1.g:0x10 0x22 0x44 0x21 0x54 0x65 0x77 0x55
 *line1.b:0x10 0x22 0x44 0x21 0x54 0x65 0x77 0x55
 *line2:
 *
 *line16
 * */
void fb::fb_draw_pixel(unsigned char x, unsigned char y, struct pixel p)
{
	unsigned char byteofst;
	unsigned char bitofst;

	byteofst = x/8;
	bitofst = (7-x%8);

	fb1[y].r[byteofst] &= ~bitofst;
	fb1[y].r[byteofst] |= p.rbit<<bitofst;

	fb1[y].g[byteofst] &= ~bitofst;
	fb1[y].g[byteofst] |= p.gbit<<bitofst;

	fb1[y].b[byteofst] &= ~bitofst;
	fb1[y].b[byteofst] |= p.bbit<<bitofst;
}

void fb::fb_draw(unsigned char x, unsigned char y, unsigned char w, unsigned char h, struct pixel *p)
{
	unsigned char i;
	unsigned char j;
	for (i=0;i<h;i++) {
		for(j=0;j<w;j++) {
			fb_draw_pixel(x+j, y+i, *p++);
		}
	}
}

void fb::fb_draw_custom(unsigned char x, unsigned char y, unsigned char w, unsigned char h, struct _rgb_8pixels *p)
{
	unsigned char i;
	unsigned char byteofst = x/8;
#if 0
	if ((y+h)>H) {
		return;
	}
#endif

	for(i=0;i<h;i++) {
		fb1[y+i].r[byteofst] = p[i].r;
		fb1[y+i].g[byteofst] = p[i].g;
		fb1[y+i].b[byteofst] = p[i].b;
	}
}

void fb::fb_draw_custom_from_flash(unsigned char x, unsigned char y, unsigned char w, unsigned char h, struct _rgb_8pixels *p)
{
	unsigned char i;
	unsigned char byteofst = x/8;
	struct _rgb_8pixels temp_pix; 

	for(i=0;i<h;i++) {
		temp_pix.r = pgm_read_byte(&p[i].r);
		temp_pix.g = pgm_read_byte(&p[i].g);
		temp_pix.b = pgm_read_byte(&p[i].b);

		fb1[y+i].r[byteofst] = temp_pix.r;
		fb1[y+i].g[byteofst] = temp_pix.g;
		fb1[y+i].b[byteofst] = temp_pix.b;
	}
}

void fb::fb_clear()
{
	memset((void *)fb1, 0x0, (W/8)*H*3*2);
}

void fb::fb_uphalf_clear()
{
	unsigned char x;
	unsigned char y;
	struct pixel p={0,0,0};

	for(y=0;y<H;y++) {
		for(x=0;x<W;x++)
		fb_draw_pixel(x, y, p);
	}
}

void fb::fb_downhalf_clear()
{
	unsigned char x;
	unsigned char y;
	struct pixel p={0,0,0};

	for(y=0;y<H;y++) {
		for(x=W;x<2*W;x++)
		fb_draw_pixel(x, y, p);
	}
}

void fb::fb_shift_left(unsigned char line)
{
	unsigned char i;
	unsigned char len=(W*2)/8;

	for(i=0;i<len-1;i++) {

		fb1[line].r[i]<<=1;
		fb1[line].r[i] |= (fb1[line].r[i+1]>>7);

		fb1[line].g[i]<<=1;
		fb1[line].g[i] |= (fb1[line].g[i+1]>>7);

		fb1[line].b[i]<<=1;
		fb1[line].b[i] |= (fb1[line].b[i+1]>>7);
	}
	fb1[line].r[i]<<=1;
	fb1[line].g[i]<<=1;
	fb1[line].b[i]<<=1;
}

void fb::fb_shift_left_screen(void)
{
	unsigned char i;
	for(i=0;i<H;i++)
		fb_shift_left(i);
}

struct _rgb_line *fb::fb_line_addr(unsigned char line)
{
	return &fb1[line];
}

// preinstantiate object
fb FB;
