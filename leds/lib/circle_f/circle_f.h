#ifndef _CIRCLE_F_H
#define _CIRCLE_F_H
#include "circle.h"

class circle_f : public circle {
public:
	void set_paras(unsigned char speed, unsigned char *flash_addr, int count);
	void install_timer(CallMe *cm);
	void set_timer_func();
	void set_timer_para();

	void do_append_datas(void);

	int _pos_begin;
	int _pos_end;
	int _pos_current;
	int _speed_count;
	unsigned char *_flash_addr;
	int _count;
};
#endif
