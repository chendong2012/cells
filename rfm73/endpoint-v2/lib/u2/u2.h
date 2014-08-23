#ifndef __U2_H__
#define __U2_H__
#include "user_activity.h"
class user_activity;
class u2:public user_activity {
public:
	u2();
	void receive_listener(unsigned char *data, unsigned char len);
	int init_ok();
};
#endif
