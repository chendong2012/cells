#ifndef __HW__
#define __HW__
#include "public.h"
class hw {
public:
void hw_shift_1bits(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2);

void hw_shift_8bits(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2);

void hw_shift_32bits(struct _rgb_line *rgb1, struct _rgb_line *rgb2);
};
#endif


