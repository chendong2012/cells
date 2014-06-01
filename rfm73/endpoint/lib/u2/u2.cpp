#include <stdio.h>
#include <stdlib.h>
#include "comm.h"
#include "u2.h"
#include <errno.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ISendCustom.h>
#include <IReceiveGS.h>
#include <user_activity.h>

ISendCustom isender("gettemp");
IReceiveGS irev("getstatus");
static u2 *myu2;

/*************************************************************************/
u2::u2(void)
{
	m_sended = 1;
	m_init = 0;
	memset(rev_buff, 0, 32);
	myu2 = this;
}

void u2::init_cmd_list()
{
	unsigned char i;

	isender.setUserObj((user_activity *)this);
	irev.setUserObj((user_activity *)this);

	Serial.println("connecting...");
        for (;;) {
                i = m_comm->connect();
                if (i==0) {
			delay(2000);
                } else {
                        Serial.println("connected ok!\n");
                        break;
                }
        }
	m_init = 1;
}

static void irq_func(void)
{
	detachInterrupt(1); //port 3
	delay(1000);
	if (digitalRead(3)==0) {
		delay(1000);
		if (digitalRead(3)==0) {
			Serial.println("int send");
			isender.trigerSend((unsigned char *)"gettime");
		}
	}
	attachInterrupt(1, irq_func, FALLING); //port 3
}

int u2::init_ok()
{
	init_cmd_list();
	pinMode(3, INPUT_PULLUP);
//	attachInterrupt(1, irq_func, FALLING); //port 3
}

void u2::receive_listener(unsigned char *data, unsigned char len)
{
	if (m_init == 1) {
		isender.msg_handler(data, len);
		irev.msg_handler(data, len);
	}
}

