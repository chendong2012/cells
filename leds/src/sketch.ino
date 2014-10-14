#include "TimerTask.h"
#include "public.h"
#include "Task.h"

#include "zkcvt.h"
#include "fb.h"
#include "hw.h"
#include "circle.h"
#include "circle_e.h"
#include "circle_f.h"
#include "circle_f_pic.h"
#include "circle_mix.h"

#include "a.h"
#include "c.h"


circle_e ce_disp;
TimerTask cm_eeprom(2);

circle_f cf_disp;
TimerTask cm_flash(2);


circle_f_pic cf_pic_disp;
TimerTask cm_flash_pic(2);

circle_mix c_mix;

struct mix_table mt[2];


void init_serial(void)
{
	Serial.begin(115200);
	Serial.println("begin!");
}

void init_ce()
{
	ce_disp.set_paras(5, 512, 8);
	ce_disp.install_timer(&cm_eeprom);
	ce_disp.start();
}

void init_cf()
{
	cf_disp.set_paras(6, (unsigned char *)hz_flash, 14);
	cf_disp.install_timer(&cm_flash);
	cf_disp.start();
}

void init_cf_pic()
{
	cf_pic_disp.set_paras(9, (struct _rgb_8pixels *)hz, 2);
	cf_pic_disp.install_timer(&cm_flash);
	cf_pic_disp.start();
}

int eeprom_addr=512;
int pause = 20;
void init_c_mix(void)
{
	mt[0]={TABLE_EEPROM | TABLE_WITH_ZK, (void *)&eeprom_addr, 8};
	mt[1]={0xff, (void *)&pause, 1};
	mt[2]={TABLE_FLASH | TABLE_WITH_ZK, (void *)hz_flash, HZ_8x16_COUNT};
	c_mix.set_paras(6, mt, 3);
	c_mix.install_timer(&cm_flash);
	c_mix.start();
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
//	init_cf();

	init_c_mix();

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
