#include "circle.h"

void circle::set_paras(unsigned char speed)
{
	_speed = speed;
	_speed_count = 0;
	_shift = 0;
}

void circle::start(void)
{
	_cm->start();
}

void circle::stop(void)
{
	_cm->stop();
}

boolean circle::display_cloop(void *p)
{
	circle *_p;
	_p = (circle *)p;

	_p->do_update();

	if(!_p->is_need_shift())
		return true;
	_p->do_shift();

	if(!_p->is_need_append_datas())
		return true;
	_p->do_append_datas();
}

void circle::do_update(void)
{
	HW.hw_write_screen();
	_speed_count++;
}

unsigned char circle::is_need_shift(void)
{
	if((_speed_count%_speed)==0) {
		_speed_count = 0;
		return true;
	}
	return false;
}

void circle::do_shift(void)
{
	FB.fb_shift_left_screen();
	_shift++;
}

unsigned char circle::is_need_append_datas(void)
{

	if((_shift%FONT_WIDTH)==0) {
		_shift = 0;
		return true;
	}
	return false;
}
