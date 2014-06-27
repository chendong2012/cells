#ifndef __U11_H__
#define __U11_H__

#include "user_activity.h"
class user_activity;

class u11:public user_activity {
public:
	u11();
	void receive_listener(unsigned char *data, unsigned char len);
	int init_ok();
	int send_net_package(unsigned char *buf, unsigned char *len);
	unsigned char send_package(unsigned char *dat, unsigned char len, ISendCustom *psender);
};
#endif
