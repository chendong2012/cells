#ifndef __CIRCLE_MIX__
#define __CIRCLE_MIX__
#include "circle.h"

class circle_mix : public circle {
public:
	void set_paras(unsigned char speed, struct mix_table *t, unsigned char count);
	void install_timer(TimerTask *cm);
	void set_timer_func();
	void set_timer_para();

	void handle_data(void);
	void move_to_next_data(void);
	void handle_item(void);
	boolean is_item_handled(void);
	void move_to_next_item(void);

	void do_append_datas(void);

private:
	struct mix_table *_t;
	unsigned char _tlen;
	unsigned char _data_index;
	unsigned char _item_index;
};
#endif
