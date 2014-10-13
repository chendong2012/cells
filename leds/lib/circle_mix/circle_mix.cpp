#include "circle_mix.h"
void circle_mix::set_paras(unsigned char speed, struct mix_table *t, unsigned char tlen)
{
	circle::set_paras(speed);
	_t = t;
	_tlen = tlen;

	_data_index=0;
	_item_index=0;
}

void circle_mix::install_timer(CallMe *cm)
{
	_cm = cm;
	set_timer_func();
	set_timer_para();
}

void circle_mix::set_timer_func()
{
	_cm->cb_func(circle::display_cloop);
}

void circle_mix::set_timer_para()
{
	_cm->set_cb_para((void *)this);
}

void circle_mix::handle_data(void)
{
	unsigned char *p = (unsigned char *)_t[_item_index].addr;
	struct _rgb_8pixels *pmem = (struct _rgb_8pixels *)_t[_item_index].addr;

	switch (_t[_item_index].flag) {
	case TABLE_MEMORY:
		FB.fb_draw_custom(W, 0, FONT_WIDTH, FONT_HEIGHT, (struct _rgb_8pixels *)&pmem[_data_index*16]);
		break;
	case TABLE_EEPROM:
		zk_cvt.write_block_eeprom(W, 0, FONT_WIDTH, FONT_HEIGHT,*(int *)&p+_data_index*16);
		break;
	case TABLE_FLASH:
		zk_cvt.write_block_flash(W, 0, FONT_WIDTH, FONT_HEIGHT,(unsigned char *)&p[_data_index*16]);
		break;
	default:
		break;
	}
}

void circle_mix::move_to_next_data(void)
{
	if(_data_index<_t[_item_index].count) {
		_data_index++;
	} else {
		_data_index=0;
	}
}

void circle_mix::handle_item(void)
{
	handle_data();
	move_to_next_data();
}

boolean circle_mix::is_item_handled(void)
{
	if(_data_index==0)
		return true;
	return false;
}

void circle_mix::move_to_next_item(void)
{
	_data_index=0;
	if(_item_index<_tlen) {
		_item_index++;
	} else {
		_item_index=0;
	}
}

void circle_mix::do_append_datas(void)
{
	handle_item();
	if(is_item_handled()) {
		move_to_next_item();
	}
}
