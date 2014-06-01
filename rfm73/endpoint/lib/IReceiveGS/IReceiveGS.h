#ifndef __IRECEIVEGS__H
#define __IRECEIVEGS__H
#include <Arduino.h>
#include <IReceive.h>
#include <user_activity.h>

class IReceiveGS : public IReceive
{
public:
	IReceiveGS(const char *cmdstr);
	void onReceive(unsigned char *dat, unsigned char len);
	user_activity *_a;
	void setUserObj(user_activity *a);
};
#endif
