#include "circle.h"
static boolean display_cloop(void);
CallMe cloop(2, display_cloop);

void circle::init(unsigned char speed, int position, int count)
{
	_speed = speed;
	_pos_begin = position;
	_count = count*16;
	_pos_end = _pos_begin+_count;
	_pos_current = _pos_begin;
	_shift = 0;
}

void circle::start(void)
{
	cloop.start();
}

void circle::stop(void)
{
	
	cloop.stop();
}

static boolean display_cloop(void)
{
	cl.do_update();

	if(!cl.is_need_shift())
		return true;
	cl.do_shift();

	if(!cl.is_need_append_datas())
		return true;
	cl.do_append_datas();
}

void circle::do_update(void)
{
	HW.hw_write_screen();
}

unsigned char circle::is_need_shift(void)
{
	_speed++;
	if(_speed>8) {
		_speed = 0;
		return true;
	} else {
		return false;
	}
}

void circle::do_shift(void)
{
	FB.fb_shift_left_screen();
}

unsigned char circle::is_need_append_datas(void)
{
	_shift++;
	if(_shift>8) {
		_shift = 0;
		return true;
	} else {
		return false;
	}
}

void circle::do_append_datas(void)
{
	zk_cvt.write_block_eeprom(32,0,8,16, _pos_current);
	_pos_current+=16;

	if(_pos_current>=_pos_end) {
		_pos_current+=_pos_begin;
	}
}
circle cl;
