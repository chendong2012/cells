#ifndef __DISP__
#define __DISP__
class disp {
	void init(void)
	{
		_fb = 0;
		_gb = 0;
		_count = 0;
		_current = 0;
		int addr = 0;
	}

	void save_fb(unsigned char fb)
	{
		_fb = fb;
	}

	void save_bg(unsigned char bg)
	{
		_bg = bg;
	}

	void save_count(unsigned char count)
	{
		_count = count;
	}
	
	unsigned char get_count()
	{
		return _count;
	}

	void save_current(unsigned char current)
	{
		_current = current;
	}

	unsigned char get_current(void)
	{
		return	_current;
	}

	unsigned char add_current(void)
	{
		return	_current++;
	}

	unsigned char is_receive_ok()
	{
		if (_count == _current+1) {
			return 1;
		} else {
			return 0;
		}
	}

	void trigger_disp(void)
	{
		;
	}

	void stop_disp(void)
	{
	}

	unsigned char _fb;	
	unsigned char _gb;	
	unsigned char _count;
	unsigned char _current;
	int addr;
};
#endif
