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
static CallMe connect_task_brd(500, connect_func);
uff::uff(void)
{
	m_init = 0;
	myuff = this;
}

int uff::init_ok()
{
	Serial.println("connecting...");
	connect_task_brd.start();
}

void uff::receive_listener(unsigned char *data, unsigned char len)
{
	if (m_init == 1) {
		irec_brd.msg_handler(data, len);
	}
}

static boolean connect_func(void)
{
	unsigned char i;
	if (myuff->m_init == 1)
		return false;

	if (1==myuff->m_comm->connect()) {
		Serial.println("uff connected ok!\n");

		connect_task_brd.stop();
		myuff->m_init = 1;
	}
	return true;
}
