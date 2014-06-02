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

/*************command list*****************

1>:
gettime<-------->gettime<11:0[0~9]>
2>:
playmp3<-------->playmp3<ok>


******************************************/
user_activity *act = new u2();
COMM comm(0, &RFM, act);
CallMe cmled(1000, callme_cb);
char buf[32];

static boolean callme_cb(void)
{
	static boolean ledlevel = HIGH;
	digitalWrite(4, ledlevel);
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

	comm.set_local_addr(LOCAL_ADDR, LOCAL_PORT);
	comm.set_remote_addr(REMOTE_ADDR, REMOTE_PORT);

	RFM.Begin();

	RFM.onReceive(receiveEvent);

	setup_irq();
	act->init_ok();

        pinMode(4, OUTPUT);
	cmled.start();
//	act->init_cmd_list();
        //delay(1000);
#if 0
	Serial.println("connecting...");
        for (;;) {
                i = comm.connect();
                if (i==0) {
			delay(2000);
                } else {
                        Serial.println("connected ok!\n");
                        break;
                }
        }
#endif
}
/*
void loop()
{

//	Serial.println("loop!");
//	comm.send(cmd_list[PLAYMP3].item, cmd_list[PLAYMP3].len);	
//	delay(1000);
}
*/
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
