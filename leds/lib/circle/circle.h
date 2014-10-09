#ifndef  __CIRCLE__
#define  __CIRCLE__
#include "public.h"
#include "fb.h"
#include "CallMe.h"
#include "hw.h"
#include "zkcvt.h"
class circle {
public:
	void init(unsigned char speed, int position, int count);
	void start();
	void stop(void);
	boolean display_loop(void);
	void do_update(void);
	unsigned char is_need_shift(void);
	void do_shift(void);
	unsigned char is_need_append_datas(void);
	void do_append_datas(void);

	unsigned char _speed;
	int _pos_begin;
	int _count;
	int _pos_end;
	int _pos_current;
	unsigned char _shift;
};
extern circle cl;
#endif
