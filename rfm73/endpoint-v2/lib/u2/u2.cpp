#include <stdio.h>
#include <stdlib.h>

#include "comm.h"
#include "u2.h"
#include <errno.h>
/*
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
*/
#include <user_activity.h>
#include <ISend.h>
#include <IReceive.h>
#include <CallMe.h>
#include <DelayRun.h>
#include <Task.h>
#include "public.h"

#ifdef LED_REMOTE_CONTROL
extern IReceive irec_led;
#endif

#ifdef FAN_REMOTE_CONTROL
extern IReceive irec_fan;
#endif

extern ISend sendors;
extern CallMe cmrf;
extern const char * send_cmds[];

static boolean connect_func(void);
static boolean key_thread(void);

CallMe connect_task(500, connect_func);
CallMe key_task(500, key_thread);

const unsigned char PROGMEM send_cmds_count  = 2;

user_activity *myu2=NULL;

/*************************************************************************/
u2::u2(void)
{
	m_sended = 1;
	m_init = 0;
	memset(rev_buff, 0, 32);
	myu2 = this;
}

static void key_irq_func(void)
{
	detachInterrupt(GPIO3_KEY_IRQ);
	key_task.start();
}

int u2::init_ok()
{
	Serial.println("connecting...");
	connect_task.start();
}

/*comm 对像会调这个对像*/
void u2::receive_listener(unsigned char *data, unsigned char len)
{
	if (m_init == 1) {
		sendors.msg_handler(data, len);
#ifdef LED_REMOTE_CONTROL
		irec_led.msg_handler(data, len);
#endif
#ifdef FAN_REMOTE_CONTROL
		irec_fan.msg_handler(data, len);
#endif
	}
}

static boolean connect_func(void)
{
	unsigned char i;
	if (myu2->m_init == 1)
		return false;

	if (1==myu2->m_comm->connect()) {
		Serial.println("connected ok!\n");

		/*fan*/
		pinMode(FAN_SPEED_KEY, OUTPUT);
		pinMode(FAN_STOP_KEY, OUTPUT);
		digitalWrite(FAN_SPEED_KEY, LOW);
		digitalWrite(FAN_STOP_KEY, LOW);
		/*end*/

		/*led*/
		pinMode(LED_SWITCH_GPIO, OUTPUT);
		digitalWrite(LED_SWITCH_GPIO, LOW);
		/*end*/

		/*key handler*/
		pinMode(GPIO3_KEY, INPUT_PULLUP);
		attachInterrupt(GPIO3_KEY_IRQ, key_irq_func, FALLING);

		connect_task.stop();
		myu2->m_init = 1;
		cmrf.start();
	}
	return true;
}

static boolean key_thread(void)
{
	static char press_ok_flag=0;
	static char press_count=0;
	if (press_ok_flag == false) {
		if (digitalRead(GPIO3_KEY)==0) { //过一指定时间，有键按下
			press_count++;
			if (press_count>0) {//按下超过一定时间，认为有真正的键按下
				sendors.trigerSend(CMD_GET_SERVER_TIME);
				press_ok_flag = true;
			}
		} else {/*表示时间不到松开了*/
			attachInterrupt(GPIO3_KEY_IRQ, key_irq_func, FALLING);
		}

	} else {//等待松开
		if(digitalRead(GPIO3_KEY)==1) { //已松开了
			press_ok_flag = false;
			key_task.stop();
			attachInterrupt(GPIO3_KEY_IRQ, key_irq_func, FALLING);
		} else {
			//还按着，没有松开
		}
	}
}
