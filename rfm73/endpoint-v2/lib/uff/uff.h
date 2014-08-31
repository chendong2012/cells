#ifndef __UFF_H__
#define __UFF_H__
#include "user_activity.h"
class user_activity;
class uff:public user_activity {
public:
	uff();
	void receive_listener(unsigned char *data, unsigned char len);
	int init_ok();
};
#endif
