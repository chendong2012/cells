#ifndef  __CIRCLE__
#define  __CIRCLE__
class circle {
public:
	void init(unsigned char speed, unsigned char position, unsigned char count);
	void start();
	void stop(void);
	boolean display_loop(void);
	static void do_update(void);
	static unsigned char is_need_shift(void);
	static void do_shift(void);
	static unsigned char is_need_append_datas(void);
	static void do_append_datas(void);

	unsigned char  _speed=0;
	int _pos_begin;
	int _count_datas;
	int _pos_end;
	int _pos_current;
};
#endif
