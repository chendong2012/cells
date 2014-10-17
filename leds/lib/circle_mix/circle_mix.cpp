#include "circle_mix.h"
void circle_mix::set_paras(unsigned char speed, struct mix_table *t, unsigned char tlen)
{
	circle::set_paras(speed);
	_t = t;
	_tlen = tlen;

	_data_index=0;
	_item_index=0;
}

void circle_mix::install_timer(TimerTask *cm)
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
	int offset = _data_index*16;

	unsigned char *p;
	int eedat = *(int *)(_t[_item_index].addr);
	struct _rgb_8pixels *prgb;

	unsigned char pos = _t[_item_index].flag>>4;
	unsigned char zr = _t[_item_index].flag&0x0f;

	p = (unsigned char *)(_t[_item_index].addr);
	p += offset;

	prgb = (struct _rgb_8pixels *)_t[_item_index].addr;
	prgb += offset;

	switch (pos) {
	case TABLE_MEMORY:
		if (zr==TABLE_WITH_RAW) {
			FB.fb_draw_custom(W, 0, FONT_WIDTH, FONT_HEIGHT, prgb);

		} else if(zr==TABLE_WITH_ZK) {
			zk_cvt.write_block(W, 0, FONT_WIDTH, FONT_HEIGHT, (const unsigned char *)p);
		}
		break;

	case TABLE_EEPROM:
		zk_cvt.write_block_eeprom(W, 0, FONT_WIDTH, FONT_HEIGHT, eedat+offset);
		break;

	case TABLE_FLASH:
		if (zr==TABLE_WITH_RAW) {
			FB.fb_draw_custom_from_flash(W, 0, FONT_WIDTH, FONT_HEIGHT, prgb);

		} else if(zr==TABLE_WITH_ZK) {
			zk_cvt.write_block_flash(W, 0, FONT_WIDTH, FONT_HEIGHT, p);

		}
		break;

	default:
		break;
	}
}

void circle_mix::move_to_next_data(void)
{
	if(_data_index<_t[_item_index].count-1) {
		_data_index++;
	} else {
		_data_index=0;
	}
}

void circle_mix::handle_pause_item(void)
{
	int eedat = *(int *)(_t[_item_index].addr);
	if(_t[_item_index].flag==0xff) {
		circle::set_pause(eedat);
		_item_index++;
	}
}

void circle_mix::handle_item(void)
{
	handle_pause_item();
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
	if(_item_index<_tlen-1) {
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
