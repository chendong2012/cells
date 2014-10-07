#include "CallMe.h"
#include "public.h"
#include "Task.h"

#include "zkcvt.h"
#include "fb.h"
#include "hw.h"


#include "a.h"
zkcvt zk_cvt;


typedef struct {
	byte pin : 6;
	byte mode : 1;
	byte state : 1;
} DigitalPin;


int aaa=0;
static boolean display_simple_test(void)
{
	unsigned char i;
	HW.hw_write_screen();
	if(aaa++>24) {
		for(i=0;i<16;i++) {
			FB.fb_shift_left(i);
			aaa = 0;
		}
	}
}

CallMe disp_write(2, display_simple_test);

void init_serial(void)
{
	Serial.begin(115200);
	Serial.println("begin!");
}
unsigned char zk_test[2]={
	0x55,
	0xff,
};
void setup()
{
	struct pixel p[4];
	init_serial();
	init_gpio();

	zk_cvt.set_fb(&FB);
	zk_cvt.setfg(4);
	zk_cvt.setbg(7);

	FB.fb_clear();
	HW.hw_write_screen();

	zk_cvt.write_pixel(20, 14, 1);
	zk_cvt.write_block(7,0,8,1, zk_test);
#if 0
	p[0].rbit=1; p[0].gbit=0; p[0].bbit=0;
	p[1].rbit=1; p[1].gbit=0; p[1].bbit=0;
	p[2].rbit=1; p[2].gbit=0; p[2].bbit=0;
	p[3].rbit=1; p[3].gbit=0; p[3].bbit=0;

	FB.fb_draw(20, 0, 4, 1, p);
	FB.fb_draw(20, 1, 4, 1, p);
	FB.fb_draw(20, 2, 4, 1, p);
	FB.fb_draw(20, 3, 4, 1, p);
	FB.fb_draw(20, 4, 4, 1, p);
	FB.fb_draw(20, 5, 4, 1, p);

	FB.fb_draw(20, 6, 4, 1, p);
	FB.fb_draw(20, 7, 4, 1, p);
	FB.fb_draw(20, 8, 4, 1, p);
	FB.fb_draw(20, 9, 4, 1, p);
	FB.fb_draw(20, 10, 4, 1, p);

	FB.fb_draw(20, 11, 4, 1, p);
	FB.fb_draw(20, 12, 4, 1, p);
	FB.fb_draw(20, 13, 4, 1, p);
	FB.fb_draw(20, 14, 4, 1, p);
	FB.fb_draw(20, 15, 4, 1, p);

	FB.fb_draw_custom(8, 0, 8, 16, &hz[16]);
	FB.fb_draw_custom(16, 0, 8, 16, hz);
#endif
	disp_write.start();
}

#if 0
void loop()
{

	//write_32x16();
	fb_shift_loop();
}
#endif
static void init_gpio(void)
{

#if 0
	pinMode(R, OUTPUT);
	pinMode(G, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(R2, OUTPUT);
	pinMode(G2, OUTPUT);
	pinMode(B2, OUTPUT);
#endif

	DDRD = B11111100;

	pinMode(SHCP, OUTPUT);
	pinMode(OE, OUTPUT);
	pinMode(LOCK, OUTPUT);
	pinMode(ROW0, OUTPUT);
	pinMode(ROW1, OUTPUT);
	pinMode(ROW2, OUTPUT);
	pinMode(16, OUTPUT);
}
