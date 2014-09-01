#include "rfm70.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include "comm.h"
#include "user_activity.h"
#include "rfthread.h"
#include "u1.h"
#include "u11.h"
#include "u2.h"

#ifdef INTERRUPT_FUNC
RFM70 rfm70(10, 0, 6, RFM77_DEFAULT_SPI_CLOCK_DIV);
#else
RFM70 rfm70(10, 0, -1, RFM77_DEFAULT_SPI_CLOCK_DIV);
#endif

user_activity *users[10];

/*setup a client*/
//user_activity *myclient = new u2();
//COMM comm_client(0, &rfm70, myclient);

/*setup a server*/
user_activity *act = new u1();
COMM comm(SERVER_ID, &rfm70, act);

/*setup a second server*/
user_activity *act1 = new u11();
COMM comm1(SERVER_ID, &rfm70, act1);


rfthread *rfthrd;
/********************************************************************************/
/*
接收中断入口
*/
void irq_func(void)
{
	rfm70.tick();
}

/*所有收到的数据都会放到rcvbuf, rcvbuf[len]=='\0',len 表示整个包的长度*/
void receiveEvent(void *rcvbuf, uint8_t len) {
        uint8_t i=0;
        uint8_t *buf=(uint8_t *)rcvbuf;
#if 0
        printf("len 0x%02x:", len);
#if 0
        for (i=0; i< len; i++) {
                printf("0x%02x ", buf[i]);
        }
#endif
        printf("%s", buf);
        printf("\n");
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
		/*收到数据给到对像分析*/
	comm.read((unsigned char *)rcvbuf, len);
	comm1.read((unsigned char *)rcvbuf, len);
//	comm_client.read((unsigned char *)rcvbuf, len);
	//comm2.read((unsigned char *)rcvbuf, len);
	//comm3.read((unsigned char *)rcvbuf, len);
	//and so on
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
}
/********************************************************************************/

void setup_server()
{
        comm.set_local_addr(1, 0xff);
        comm1.set_local_addr(1, 81);
//	act->init_ok();
}

void setup_client()
{
//        comm_client.set_local_addr(1, 90);
//        comm_client.set_remote_addr(2, 80);
//	myclient->init_ok();
}

void setup()
{
	rfm70.begin();
	rfm70.onReceive(receiveEvent);
	rfm70.confIRQ(6, 0, 1, 0);
	rfm70.Register_IRQ(irq_func);


	setup_server();
//	setup_client();

//	users[0] = myclient;
	users[0] = act1;
	rfthrd = new rfthread(users, 1);
	rfthrd->init_ok();
}


int main(void)
{
	setup();
	for (;;) {
		usleep(20000);
	}
}

#if 0
void func(void)
{
	time_t now;   //实例化time_t结构
	struct tm *timenow;    //实例化tm结构指针
	time(&now);   //time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
	timenow = localtime(&now);//localtime函数把从time取得的时间now换算成你电脑中的时间(>就是你设置的地区)
	printf("Local time is %s\n", asctime(timenow)); //上句中asctime函数把时间转换成字符，>通过printf()函数输出
}
#endif
