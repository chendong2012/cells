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
#include <user_activity.h>
#include <ISend.h>
#include <IReceive.h>
#include <CallMe.h>
#include <DelayRun.h>
#include <Task.h>
//#define LED_FUNC
#define FAN_FUNC

#ifdef FAN_FUNC
#define FAN_SPEED_KEY 5
#define FAN_STOP_KEY 6
#endif

#define LED_SWITCH_KEY 7

static boolean timer_func(void);
CallMe cmrf(500, timer_func);

#ifdef LED_FUNC
static void cb_led(unsigned char *dat, unsigned char len);
IReceive irec("led", cb_led);
#endif

#ifdef FAN_FUNC
static void cb_fan(unsigned char *dat, unsigned char len);
IReceive irec_fan("fan", cb_fan);
#endif

static boolean press_speed_key(Task* task);
static boolean release_speed_key(Task* task);

static boolean press_off_key(Task* task);
static boolean release_off_key(Task* task);

/*风扇调速的任务*/
DelayRun speed_release_task(500, release_speed_key);
DelayRun speed_press_task(10, press_speed_key, &speed_release_task);

/*风扇关机的任务*/
DelayRun off_release_task(500, release_off_key);
DelayRun off_press_task(10, press_off_key, &off_release_task);

boolean press_speed_key(Task* task)
{
	digitalWrite(FAN_SPEED_KEY, HIGH);
	return true;
}

boolean release_speed_key(Task* task)
{
	digitalWrite(FAN_SPEED_KEY, LOW);
	return false;
}

///
boolean press_off_key(Task* task)
{
	digitalWrite(FAN_STOP_KEY, HIGH);
	return true;
}

boolean release_off_key(Task* task)
{
	digitalWrite(FAN_STOP_KEY, LOW);
	return false;
}


/*主动发送的命令集合*/
const char * send_cmds[] = { 
	"getsvrtm",/*获取服务器的时间*/
	"cmd4"
};
/*********************/
ISend isender(send_cmds[0]);
/*********************/

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

	cmrf.start();
	m_init = 1;
}

static void irq_func(void)
{
	detachInterrupt(1); //port 3
	delay(1000);
	if (digitalRead(3)==0) {
		delay(1000);
		if (digitalRead(3)==0) {
			isender.trigerSend(send_cmds[0]);
		}
	}
	attachInterrupt(1, irq_func, FALLING); //port 3
}

int u2::init_ok()
{
	init_cmd_list();
	pinMode(3, INPUT_PULLUP);

        pinMode(FAN_SPEED_KEY, OUTPUT);
        pinMode(FAN_STOP_KEY, OUTPUT);
	digitalWrite(FAN_SPEED_KEY, LOW);
	digitalWrite(FAN_STOP_KEY, LOW);

        pinMode(LED_SWITCH_KEY, OUTPUT);
	digitalWrite(LED_SWITCH_KEY, LOW);

	attachInterrupt(1, irq_func, FALLING); //port 3
}

void u2::receive_listener(unsigned char *data, unsigned char len)
{
	if (m_init == 1) {
		isender.msg_handler(data, len);
#ifdef LED_FUNC
		irec.msg_handler(data, len);
#endif
#ifdef FAN_FUNC
		irec_fan.msg_handler(data, len);
#endif

	}
}

static boolean timer_func(void)
{
	isender.send_cb(&isender);
}

#ifdef LED_FUNC
/*
 *接收到从远程过来的ＬＥＤ命令，然后根据变量判断，是开灯还是关灯，
  并执行相关动作。
**/
static void cb_led(unsigned char *dat, unsigned char len)
{
	if(irec.isNewPackage(dat)) {
		if(irec.cmpAction(dat, len , (unsigned char *)"on")) {
			digitalWrite(LED_SWITCH_KEY, HIGH);

		} else if (irec.cmpAction(dat, len , (unsigned char *)"off")) {
			digitalWrite(LED_SWITCH_KEY, LOW);

		} else {
				
		}

		myu2->m_comm->send("getstatus:ok", 12);	
		irec.saveAckBuf((unsigned char *)"getstatus:ok", 12);
	} else {
		myu2->m_comm->send((const char *)irec.getAckBuf(), irec.getAckBufLen());	
	}
}
#endif

#ifdef FAN_FUNC
/*
 *接收到从远程过来的风扇控制命令，然后根据变量判断，是开灯还是关灯，
  并执行相关动作。
**/
static void cb_fan(unsigned char *dat, unsigned char len)
{
	if(irec_fan.isNewPackage(dat)) {
		if(irec_fan.cmpAction(dat, len , (unsigned char *)"speed")) {
			speed_press_task.startDelayed();

			myu2->m_comm->send("fanspeed:ok", 11);
			irec_fan.saveAckBuf((unsigned char *)"fanspeed:ok", 11);

		} else if (irec_fan.cmpAction(dat, len , (unsigned char *)"off")) {
			off_press_task.startDelayed();

			myu2->m_comm->send("fanoff:ok", 9);
			irec_fan.saveAckBuf((unsigned char *)"fanoff:ok", 9);

		} else {
				
		}

	} else {
		myu2->m_comm->send((const char *)irec_fan.getAckBuf(), irec_fan.getAckBufLen());	
	}
}
#endif
