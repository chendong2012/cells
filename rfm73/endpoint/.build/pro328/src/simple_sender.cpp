#include <Arduino.h>
static void irq_func(void);
static void setup_irq();
void setup();
void receiveEvent(void);
#line 1 "src/simple_sender.ino"
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
#include <SoftTimer.h>

/*************************/
#define LOCAL_ADDR      3
#define LOCAL_PORT      90
#define REMOTE_ADDR     1 
#define REMOTE_PORT     81
/*************************/

/*************command list*****************

1>:
gettime<-------->gettime<11:0[0~9]>
2>:
playmp3<-------->playmp3<ok>


******************************************/
user_activity *act = new u2();
COMM comm(0, &RFM, act);

char buf[32];

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

	comm.set_local_addr(LOCAL_ADDR, LOCAL_PORT);
	comm.set_remote_addr(REMOTE_ADDR, REMOTE_PORT);

	RFM.Begin();
	RFM.onReceive(receiveEvent);

	setup_irq();
	act->init_ok();
}

/*all received datas will come here!!
and dispatch all comm objs*/
void receiveEvent(void)
{
	char *p = (char *)RFM.getRcvBuffer();
#if 1
	sprintf(buf, "%d:%d->%d:%d >%s",p[0], p[1], p[2], p[3], &p[4]);
	Serial.println(buf);
#endif
	comm.read((unsigned char *)p, RFM.getPacketLength());
}
