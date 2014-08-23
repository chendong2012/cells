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

#include <CallMe.h>

#define LED_PIN 4
/*************************/
#define LOCAL_ADDR      3
#define LOCAL_PORT      90
#define REMOTE_ADDR     1 
#define REMOTE_PORT     81
/*************************/
#define DEBUG

#define TICK_LED 6

user_activity *act = new u2();
COMM comm(0);
CallMe cmled(1000, callme_cb);
char buf[32];

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

/*初始化comm的本地地址和远程地址*/
	comm.set_local_addr(LOCAL_ADDR, LOCAL_PORT);
	comm.set_remote_addr(REMOTE_ADDR, REMOTE_PORT);

/*work ok*/
	comm.attach_user_activity(act);
	comm.attach_rfm(&RFM);

	RFM.Begin();
	RFM.onReceive(receiveEvent);
	setup_irq();

	/*这里面包括了等待连接的过程,要改进*/
	act->init_ok();


        pinMode(TICK_LED, OUTPUT);
	cmled.start();
}

/*all received datas will come here!!
and dispatch all comm objs*/
/*这是RF中断入口函数，当有数据的时候，
 * 就会触发这个函数调用，所以是中断上下文，
 * 不应处理太多的事情,根据之前的经验，太多
 * 的ＬＯＧ，发导致死机*/
void receiveEvent(void)
{
	char *p = (char *)RFM.getRcvBuffer();
#ifdef DEBUG
	sprintf(buf, "%d:%d->%d:%d >%s",p[0], p[1], p[2], p[3], &p[4]);
	Serial.println(buf);
#endif
	comm.read((unsigned char *)p, RFM.getPacketLength());
}
