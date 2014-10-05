#ifndef __ZKC__H
#define __ZKC__H
#include "public.h"
struct _raw_8pixels
{
	unsigned char dat;
};

class zkcvt {
public:

	void setfg(unsigned char fg);
	void setbg(unsigned char bg);
	void convert_pixel(unsigned char raw_bit, struct pixel *p);
	void convert_8_pixels(const struct _raw_8pixels *raw, struct _rgb_8pixels *rgb);

	unsigned char _fg;
	unsigned char _bg;
};
#endif
