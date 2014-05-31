#ifndef __U1_H__
#define __U1_H__

#include "user_activity.h"
class user_activity;

class u1:public user_activity {
public:
	u1();
	void receive_listener(unsigned char *data, unsigned char len);
	int init_ok();
	int send_net_package(unsigned char *buf, unsigned char *len);
};
#endif
