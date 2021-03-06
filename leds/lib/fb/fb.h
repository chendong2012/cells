#ifndef __FB__
#define __FB__
#include "public.h"
class fb {
public:
	void fb_draw_pixel(unsigned char x, unsigned char y, struct pixel p);
	void fb_draw(unsigned char x, unsigned char y, unsigned char w, unsigned char h, struct pixel *p);
	void fb_draw_custom(unsigned char x, unsigned char y, unsigned char w, unsigned char h, struct _rgb_8pixels *p);
	void fb_draw_custom_from_flash(unsigned char x, unsigned char y, unsigned char w, unsigned char h, struct _rgb_8pixels *p);
	void fb_clear(void);
	void fb_uphalf_clear();
	void fb_downhalf_clear();

	void fb_shift_left_screen(void);
	void fb_shift_left(unsigned char line);
	struct _rgb_line fb1[H];
	struct _rgb_line *fb_line_addr(unsigned char line);

};
extern fb FB;
#endif
