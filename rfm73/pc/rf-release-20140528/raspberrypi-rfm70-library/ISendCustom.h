#ifndef __ISENDCUSTOM__H
#define __ISENDCUSTOM__H
#include <ISend.h>
#include <user_activity.h>

class ISendCustom : public ISend
{
public:
	ISendCustom(const char *cmdstr);
	void onReceive(unsigned char *dat, unsigned char len);
	void sendRfDatas();
	void setUserObj(user_activity *a);
	user_activity *_act;
	unsigned char sendRF(unsigned char *s);
};
#endif
