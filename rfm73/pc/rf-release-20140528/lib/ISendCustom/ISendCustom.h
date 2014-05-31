#ifndef __ISENDCUSTOM__H
#define __ISENDCUSTOM__H
#include <Arduino.h>
#include <ISend.h>
#include <user_activity.h>

class ISendCustom : public ISend
{
public:
	ISendCustom(const char *cmdstr, user_activity *act);
	void onReceive(unsigned char *dat, unsigned char len);
	void sendRfDatas();
	user_activity *_act;
};
#endif
