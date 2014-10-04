#ifndef __ZKC__H
#define __ZKC__H
struct _raw_8pixels
{
	unsigned char dat;
}

class zkcvt {
public:

	void set_fg(unsigned char fg);
	void set_bg(unsigned char bg);
	void convert_pixel(unsigned char raw_bit, struct pixel *p);
	void convert_8_pixels(const struct _raw_8pixels *raw, struct _rgb_8pixels *rgb);

	unsigned char _fg;
	unsigned char _bg;
};
#endif
