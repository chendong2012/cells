#ifndef _CIRCLE_E_H
#define _CIRCLE_E_H
#include "circle.h"

class circle_e : public circle {
public:
	void set_paras(unsigned char speed, int position, int count);
	void install_timer(TimerTask *cm);
	void set_timer_func();
	void set_timer_para();

	void do_append_datas(void);

	int _pos_begin;
	int _pos_end;
	int _pos_current;
	int _speed_count;

	int _count;
};
#endif
