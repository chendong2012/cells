/*******************************************************
  本机为客户端，本机地址：3:90
  服务器地址：1:81
  第一步： 连接
  第二步： 按了一个按键就发送一个时间请求（得到当时时间）
 ********************************************************/

#include <rfm73.h>
#include <comm.h>
#include <user_activity.h>
#include <u2.h>
#include <uff.h>
#include "public.h"

#include <CallMe.h>

#define LED_PIN 4

//COMM comm_uff(I_AM_CLIENT);
//COMM comm(I_AM_CLIENT);

u2 act;
uff act_uff;

//user_activity *act = new u2();
//user_activity *act_uff = new uff();

CallMe cmled(1000, callme_cb);
char g_debug[32];

static boolean callme_cb(void)
{
	static boolean ledlevel = HIGH;
	digitalWrite(TICK_LED, ledlevel);
	ledlevel = !ledlevel;
}

static void irq_func(void)
{
	RFM.tick();
}

static void setup_irq()
{
	attachInterrupt(0, irq_func, FALLING); //port 2
}

void setup()
{
	byte i;
	Serial.begin(9600);
	Serial.println("reset...ok!");
	memset(g_debug,0,32);
/*初始化comm的本地地址和远程地址*/

	comm.attach_id(I_AM_CLIENT);
	comm_uff.attach_id(I_AM_CLIENT);
	comm.set_local_addr(LOCAL_ADDR, LOCAL_PORT);
	comm.set_remote_addr(REMOTE_ADDR, REMOTE_PORT);

	comm_uff.set_local_addr(3, 15);
	//comm_uff.set_remote_addr(BRD_REMOTE_ADDR, BRD_REMOTE_PORT);
	comm_uff.set_remote_addr(1, 15);

/*work ok*/
	comm.attach_user_activity((user_activity *)&act);
	comm.attach_rfm(&RFM);

	comm_uff.attach_user_activity((user_activity *)&act_uff);
	comm_uff.attach_rfm(&RFM);

	RFM.Begin();
	RFM.onReceive(receiveEvent);
	setup_irq();

	/*这里面包括了等待连接的过程,要改进*/
	act.init_ok();
	act_uff.init_ok();

        pinMode(TICK_LED, OUTPUT);
	cmled.start();
}

/*all received datas will come here!!
and dispatch all comm objs*/
/*这是RF中断入口函数，当有数据的时候，
 * 就会触发这个函数调用，所以是中断上下文，
 * 不应处理太多的事情,根据之前的经验，太多
 * 的ＬＯＧ，发导致死机*/
#define DEBUG
void receiveEvent(void)
{
	char *p = (char *)RFM.getRcvBuffer();
	delay(10);
#ifdef DEBUG
//	sprintf(g_debug, "%d:%d->%d:%d >%s",p[0], p[1], p[2], p[3], &p[4]);
//	Serial.println(g_debug);
#endif
	comm.read((unsigned char *)p, RFM.getPacketLength());
	comm_uff.read((unsigned char *)p, RFM.getPacketLength());
}
