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

static boolean timer_func(void);
static boolean connect_func(void);
static boolean key_thread(void);

CallMe cmrf(500, timer_func);
CallMe connect_task(500, connect_func);
CallMe key_task(500, key_thread);

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
	return true;
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
		pinMode(LED_SWITCH_KEY, OUTPUT);
		digitalWrite(LED_SWITCH_KEY, LOW);
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
			if (press_count>0) {//按下超过一定时间了，认为有真正的键按下
				isender.trigerSend(send_cmds[0]);
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
