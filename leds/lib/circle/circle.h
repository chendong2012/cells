#ifndef  __CIRCLE__
#define  __CIRCLE__
#include "public.h"
#include "fb.h"
#include "CallMe.h"
#include "hw.h"
#include "zkcvt.h"
class circle {
public:
	void set_paras(unsigned char speed);
	void start();
	void stop(void);
	static boolean display_cloop(void *p);
	void do_update(void);
	unsigned char is_need_shift(void);
	void do_shift(void);
	unsigned char is_need_append_datas(void);
	virtual void do_append_datas(void);

	unsigned char _speed;
	unsigned char _speed_count;
	unsigned char _shift;
	CallMe *_cm;
};
//extern circle cl;
#endif
