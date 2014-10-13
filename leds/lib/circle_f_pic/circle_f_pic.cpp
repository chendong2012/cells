#include "circle_f_pic.h"
void circle_f_pic::set_paras(unsigned char speed, struct _rgb_8pixels *flash_addr, int count)
{
	circle::set_paras(speed);
	_pos_begin = 0;
	_pos_current = 0;
	_count = count*FONT_HEIGHT;
	_pos_end = _pos_begin+_count;
	_flash_addr = flash_addr;
}

void circle_f_pic::install_timer(CallMe *cm)
{
	_cm = cm;
	set_timer_func();
	set_timer_para();
}

void circle_f_pic::set_timer_func()
{
	_cm->cb_func(circle::display_cloop);
}

void circle_f_pic::set_timer_para()
{
	_cm->set_cb_para((void *)this);
}

void circle_f_pic::do_append_datas(void)
{
	FB.fb_draw_custom_from_flash(32,0,FONT_WIDTH, FONT_HEIGHT, (struct _rgb_8pixels *)&_flash_addr[_pos_current]);
	_pos_current+=FONT_HEIGHT;

	if(_pos_current>=_pos_end)
		_pos_current=_pos_begin;
}
