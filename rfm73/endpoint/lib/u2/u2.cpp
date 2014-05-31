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
#include <IReceive.h>
#include <user_activity.h>

static void on_receive_getstatus(unsigned char *dat, unsigned char len);

ISendCustom isender("gettemp");
IReceive irev("getstatus", on_receive_getstatus);
static u2 *myu2;

/*************************************/
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
	Serial.println("u2::receive_listener");
	if (m_init == 1) {
		isender.msg_handler(data, len);
		irev.msg_handler(data, len);
	}
}

static void on_receive_getstatus(unsigned char *dat, unsigned char len)
{
		if (irev.isNewPackage(dat)) {
			irev.saveAckBuf((unsigned char *)"getstatus:ok", strlen("getstatus:ok"));
			myu2->m_comm->send("getstatus:ok", strlen("getstatus:ok"));
		} else {
			myu2->m_comm->send((const char *)irev.getAckBuf(), irev.getAckBufLen());
		}
}
