#include "public.h"
#include <IReceive.h>
#include <user_activity.h>
#include "comm.h"
#include <DelayRun.h>
#include <Task.h>
#include "public.h"
extern user_activity *myu2;

static void cb_fan(unsigned char *dat, unsigned char len);
IReceive irec_fan(FAN_REMOTE_KEYWORD, cb_fan);

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

static boolean press_speed_key(Task* task)
{
	digitalWrite(FAN_SPEED_KEY, HIGH);
	return true;
}

static boolean release_speed_key(Task* task)
{
	digitalWrite(FAN_SPEED_KEY, LOW);
	return false;
}

///
static boolean press_off_key(Task* task)
{
	digitalWrite(FAN_STOP_KEY, HIGH);
	return true;
}

static boolean release_off_key(Task* task)
{
	digitalWrite(FAN_STOP_KEY, LOW);
	return false;
}

/*
 *接收到从远程过来的风扇控制命令，然后根据变量判断，是调速还是关机，
  并执行相关动作。
**/
static void cb_fan(unsigned char *dat, unsigned char len)
{
	unsigned char l;
	unsigned char *ack_data;
	if(irec_fan.isNewPackage(dat)) {
		if(irec_fan.is_contain_userdata(dat, len, (unsigned char *)FAN_REMOTE_SPEED)) {

			speed_press_task.startDelayed();

			l = strlen(FAN_REMOTE_ACK_SPEED_OK);
			irec_fan.saveUserBufToAckBuf((unsigned char *)FAN_REMOTE_ACK_SPEED_OK, l);

			l = irec_fan.getAckBufLen();
			ack_data = irec_fan.getAckBuf();
			myu2->m_comm->send((const char *)ack_data, l);
			//myu2->m_comm->send("fanspeed:ok", 11);

			dispinfo((const char *)Package::get_pkg_datas(INDEX_EXIST, dat));

		} else if (irec_fan.is_contain_userdata(dat, len , (unsigned char *)FAN_REMOTE_OFF)) {

			off_press_task.startDelayed();

			l = strlen(FAN_REMOTE_ACK_OFF_OK);
			irec_fan.saveUserBufToAckBuf((unsigned char *)FAN_REMOTE_ACK_OFF_OK, l);

			l = irec_fan.getAckBufLen();
			ack_data = irec_fan.getAckBuf();
			myu2->m_comm->send((const char *)ack_data, l);
			//myu2->m_comm->send("fanoff:ok", 9);

			dispinfo((const char *)Package::get_pkg_datas(INDEX_EXIST, dat));

		} else {
				
		}
	} else {
		l = irec_fan.getAckBufLen();
		ack_data = irec_fan.getAckBuf();
		//myu2->m_comm->send("fanspeed:ok", 11);
		myu2->m_comm->send((const char *)ack_data, l);	
	}
}
