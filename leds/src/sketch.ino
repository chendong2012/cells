#include "CallMe.h"
#include "public.h"
#include "Task.h"

#include "zkcvt.h"
#include "fb.h"
#include "hw.h"
#include "circle.h"
#include "circle_e.h"
#include "circle_f.h"
#include "circle_f_pic.h"

#include "a.h"


circle_e ce_disp;
CallMe cm_eeprom(2);

circle_f cf_disp;
CallMe cm_flash(2);


circle_f_pic cf_pic_disp;
CallMe cm_flash_pic(2);
#if 0
int aaa=0;
unsigned char fc=0;
unsigned char s8=0;
static boolean display_test_flash(void)
{
	unsigned char i;
	HW.hw_write_screen();
	if(aaa++>3) {
		FB.fb_shift_left_screen();
		s8++;
		if(s8>=8) {
			s8=0;
			zk_cvt.write_block_flash(32,0,8,16, (unsigned char *)(&hz_8x16[fc*16]));
			fc++;
			if(fc>=(sizeof(hz_8x16)>>4)) {
				fc = 0;
		}
		}
		aaa = 0;
	}
	
}
#endif

void init_serial(void)
{
	Serial.begin(115200);
	Serial.println("begin!");
}

void init_ce()
{
	ce_disp.set_paras(1, 512, 8);
	ce_disp.install_timer(&cm_eeprom);
	ce_disp.start();
}

void init_cf()
{
	cf_disp.set_paras(1, (unsigned char *)hz_flash, 14);
	cf_disp.install_timer(&cm_flash);
	cf_disp.start();
}

void init_cf_pic()
{
	cf_pic_disp.set_paras(1, (struct _rgb_8pixels *)hz, 2);
	cf_pic_disp.install_timer(&cm_flash);
	cf_pic_disp.start();
}

void setup()
{
	unsigned char i;
	struct pixel p[4];
	init_serial();
	init_gpio();

	zk_cvt.set_fb(&FB);
	zk_cvt.setfg(2);
	zk_cvt.setbg(0);

//	init_ce();
	init_cf();


	FB.fb_clear();
	HW.hw_write_screen();
}

static void init_gpio(void)
{
	DDRD = B11111100;

	pinMode(SHCP, OUTPUT);
	pinMode(OE, OUTPUT);
	pinMode(LOCK, OUTPUT);
	pinMode(ROW0, OUTPUT);
	pinMode(ROW1, OUTPUT);
	pinMode(ROW2, OUTPUT);
	pinMode(16, OUTPUT);
}
