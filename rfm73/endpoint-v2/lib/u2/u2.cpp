#include <stdio.h>
#include <stdlib.h>
#include <MsTimer2.h>

#include "comm.h"
#include "u2.h"
#include <errno.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <user_activity.h>
#include <ISend.h>
#include <IReceive.h>
#define LED_PIN 4
const char * PROGMEM send_cmds[] = { 
	"getsvrtm",
	"cmd4"
};
/*********************/
static ISend *isender;
static IReceive *irec;
/*********************/

const unsigned char PROGMEM send_cmds_count  = 2;

user_activity *myu2=NULL;

static void cb_getstatus(unsigned char *dat, unsigned char len);
/*************************************************************************/
u2::u2(void)
{
	m_sended = 1;
	m_init = 0;
	memset(rev_buff, 0, 32);
	myu2 = this;
	*isender = ISend(send_cmds[0]);
	*irec =  IReceive("getstatus", cb_getstatus);
}

void u2::init_cmd_list()
{
	unsigned char i;

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

	init_timer();
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
			isender->trigerSend(send_cmds[0]);

		}
	}
	attachInterrupt(1, irq_func, FALLING); //port 3
}

int u2::init_ok()
{
	init_cmd_list();
	pinMode(3, INPUT_PULLUP);
        pinMode(LED_PIN, OUTPUT);

//	attachInterrupt(1, irq_func, FALLING); //port 3
}

void u2::receive_listener(unsigned char *data, unsigned char len)
{
	if (m_init == 1) {
		isender->msg_handler(data, len);
		irec->msg_handler(data, len);


	}
}

static void timer_func(void)
{
	static boolean ledlevel = HIGH;
	static unsigned char div=0;
	MsTimer2::stop();
	div++;
	if (div%2==0) {
		isender->send_cb(isender);
	}

	if(div%50==0) {
		Serial.println("wdt over!");
		digitalWrite(LED_PIN, ledlevel);
		ledlevel = !ledlevel;
	}

	MsTimer2::start();
}

void u2::init_timer()
{
	MsTimer2::set(20, timer_func); //80mS period
	MsTimer2::start();
}


static void cb_getstatus(unsigned char *dat, unsigned char len)
{
	if(irec->isNewPackage(dat)) {
		myu2->m_comm->send("getstatus:ok", 12);	
		irec->saveAckBuf((unsigned char *)"getstatus:ok", 12);
	} else {
		myu2->m_comm->send("getstatus:ok", 12);	
	}
}
