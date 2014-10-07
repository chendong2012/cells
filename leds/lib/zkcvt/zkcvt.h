#ifndef __ZKC__H
#define __ZKC__H
#include "public.h"


class zkcvt {
public:

	void setfg(unsigned char fg);
	void setbg(unsigned char bg);
	void convert_pixel(unsigned char raw_bit, struct pixel *p);
	void convert_8_pixels(const unsigned char zk_raw, struct _rgb_8pixels *rgb);

	unsigned char _fg;
	unsigned char _bg;

	

};
#endif
