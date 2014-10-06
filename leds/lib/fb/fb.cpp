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

	fb[y].r[byteofst] &= ~bitofst;
	fb[y].r[byteofst] |= p.rbit<<bitofst;

	fb[y].g[byteofst] &= ~bitofst;
	fb[y].g[byteofst] |= p.gbit<<bitofst;

	fb[y].b[byteofst] &= ~bitofst;
	fb[y].b[byteofst] |= p.bbit<<bitofst;
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

	if ((y+h)>H) {
		return;
	}

	for(i=0;i<h;i++) {
		fb[y+i].r[byteofst] = p[i].r;
		fb[y+i].g[byteofst] = p[i].g;
		fb[y+i].b[byteofst] = p[i].b;
	}
}

void fb::fb_clear(void)
{
	memset((void *)fb, 0x00, (W/8)*H*3*2);
	memset((void *)fb_bak, 0x00, (W/8)*H*3*2);
}


void fb::fb_shift_left(unsigned char line)
{
	unsigned char len=W*2;
	unsigned char i;
	for(i=0;i<len;i+=2) {
		fb[line].r[i]<<=1;
		fb[line].r[i] |= (fb[line].r[i+1]>>7);

		fb[line].g[i]<<=1;
		fb[line].g[i] |= (fb[line].g[i+1]>>7);

		fb[line].b[i]<<=1;
		fb[line].b[i] |= (fb[line].b[i+1]>>7);
	}
}

void fb::fb_shift_left_screen(void)
{
	unsigned char i;
	for(i=0;i<H;i++)
		fb_shift_left(i);
}
// preinstantiate object

fb FB;
