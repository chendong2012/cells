#ifndef __ZKC__H
#define __ZKC__H
#include "public.h"
#include "fb.h"


class zkcvt {
public:

	void set_fb(fb *fb_buf)
	{
		_fb_buf = fb_buf;
	}
	void setfg(unsigned char fg);
	void setbg(unsigned char bg);
	void write_pixel(unsigned char x, unsigned char y, unsigned char pixel);
	void write_block(unsigned char x, unsigned char y, unsigned char w, unsigned char h, const unsigned char *zk_raw);
	void write_block_flash(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char *zk_raw);
	void write_block_eeprom(unsigned char x, unsigned char y, unsigned char w, unsigned char h, int position);

	unsigned char _fg;
	unsigned char _bg;
	fb *_fb_buf;
};

extern zkcvt zk_cvt;
#endif
