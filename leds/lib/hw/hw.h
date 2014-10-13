#ifndef __HW__
#define __HW__
#include "public.h"

#define HW_DISP_DELAY 300
class hw {
public:
void hw_write_pixel(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2);

void hw_write_8pixels(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2);

void hw_write_line(struct _rgb_line *rgb1, struct _rgb_line *rgb2);
void hw_select_row(unsigned char line);
void hw_lock_data(void);
void hw_output_data(void);
void hw_write_screen(void);
};
extern hw HW;
#endif
