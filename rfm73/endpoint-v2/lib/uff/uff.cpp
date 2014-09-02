#include <stdio.h>
#include <stdlib.h>

#include "comm.h"
#include "uff.h"
#include <errno.h>
#include <user_activity.h>
#include <ISend.h>
#include <IReceive.h>
#include <CallMe.h>
#include <DelayRun.h>
#include <Task.h>
#include "public.h"

extern IReceive irec_brd;

static boolean connect_func(void);
user_activity *myuff=NULL;
static CallMe connect_task_brd(1500, connect_func);
uff::uff(void)
{
	m_init = 0;
	myuff = this;
}

int uff::init_ok()
{
	Serial.println("uff connecting...");
	connect_task_brd.start();
}

void uff::receive_listener(unsigned char *data, unsigned char len)
{
	sprintf(g_debug, "s4:%s", data);
	Serial.println((const char *)g_debug);
	if (m_init == 1) {
		irec_brd.msg_handler(data, len);
	}
}

static boolean connect_func(void)
{
	unsigned char i;
	if (myuff->m_init == 1)
		return false;

//	Serial.println("uff connecting...");
	if (1==myuff->m_comm->connect()) {
		Serial.println("uff connected ok!\n");

		connect_task_brd.stop();
		myuff->m_init = 1;
	} else {
	}
	return true;
}
