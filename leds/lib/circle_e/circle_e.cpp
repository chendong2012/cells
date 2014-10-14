#include "circle_e.h"
void circle_e::set_paras(unsigned char speed, int position, int count)
{
	circle::set_paras(speed);
	_pos_begin = position;

	_pos_current = _pos_begin;

	_count = count*FONT_HEIGHT;
	_pos_end = _pos_begin+_count;
}

void circle_e::install_timer(TimerTask *cm)
{
	_cm = cm;
	set_timer_func();
	set_timer_para();
}

void circle_e::set_timer_func()
{
	_cm->cb_func(circle::display_cloop);
}

void circle_e::set_timer_para()
{
	_cm->set_cb_para((void *)this);
}

void circle_e::do_append_datas(void)
{
	zk_cvt.write_block_eeprom(32, 0, FONT_WIDTH, FONT_HEIGHT,_pos_current);
	_pos_current+=FONT_HEIGHT;

	if(_pos_current>=_pos_end)
		_pos_current=_pos_begin;
}
