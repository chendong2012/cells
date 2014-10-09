CallMe loop(2, display_loop);
void circle::init(unsigned char speed, unsigned char position, unsigned char count)
{
	_speed = speed;
	_pos_begin=position;
	_count_datas= count;
	_pos_end=_pos_begin+_count;
	_pos_current = _pos_begin;
}

void circle::start()
{
	loop.start();
}

void circle::stop(void)
{
	
	loop.stop();
}


boolean circle::display_loop(void)
{
	do_update();

	if(!is_need_shift())
		return true;
	do_shift();

	if(!is_need_append_datas())
		return true;
	do_append_datas();
}

static void circle::do_update(void)
{
	HW.hw_write_screen();
}

static unsigned char is_need_shift(void)
{
	_speed++;
	if(_speed>8) {
		_speed = 0;
		return true;
	} else {
		return false;
	}
}

static void circle::do_shift(void)
{
	FB.fb_shift_left_screen();
}

static unsigned char circle::is_need_append_datas(void)
{
	_shift++;
	if(_shift>8) {
		_shift = 0;
		return true;
	} else {
		return false;
	}
}

static void circle::do_append_datas(void)
{
	zk_cvt.write_block_eeprom(32,0,8,16, _pos_current);
	_pos_current+=16;

	if(_pos_current>=_position_end) {
		_pos_current+=_pos_begin;
	}
}
