#include <stdio.h>
#include <stdlib.h>
#include "comm.h"
#include <pthread.h>
#include "u11.h"
#include "user_activity.h"
#include <errno.h>
#include <IReceive.h>
#include <string.h>
#include <unistd.h>
#include <ISend.h>


extern unsigned int get_count_8x16();
extern unsigned char *get_datas();
extern int cvt_main(char *argv);


static void cb_sent_backmsg(unsigned char *dat, unsigned char len);
user_activity *myu11=NULL;

/****************************************************************************/

extern IReceive irec_time_server;		/*用于远程想得到当前时间信息*/
ISend isender_fan(FAN_REMOTE_KEYWORD, cb_sent_backmsg);	/*fan 控制*/
ISend isender_leddisp(LEDDISP_FONT_KEYWORD, cb_sent_backmsg);	/*leddisp 控制*/
/****************************************************************************/




static void cb_sent_backmsg(unsigned char *dat, unsigned char len)
{
	printf("isender_fan:cb_sent_backmsg!!!\n");
}


u11::u11(void)
{
        myu11 = this;
}

static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;

static pthread_mutex_t g_mutex_net = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond_net = PTHREAD_COND_INITIALIZER;


/*主要用来接收到要处理的信息:
1,并做相关的动作
2,回复相关信息等等
*/
static void *thread_handle_rev_datas(void *ptr)
{
	unsigned char len;
	int ret;
	struct timeval now;
	struct timespec outtime;

	user_activity *p = (user_activity*)ptr;
	sleep(1);
        for(;;) {
		pthread_mutex_lock(&g_mutex);

		gettimeofday(&now, NULL);
		outtime.tv_sec = now.tv_sec + 5;
		outtime.tv_nsec = now.tv_usec * 1000;

		ret = pthread_cond_timedwait(&g_cond, &g_mutex, &outtime);
		if(ret == ETIMEDOUT) {
			pthread_mutex_unlock(&g_mutex);
			continue;
		}

/*========================add your code here=======================*/

                isender_fan.msg_handler(p->rev_buff, p->rev_len);
                irec_time_server.msg_handler(p->rev_buff, p->rev_len);

		isender_leddisp(p->rev_buff, p->rev_len);

/*========================add your code end =======================*/

                pthread_mutex_unlock(&g_mutex);
        }
}

/*主动发送*/
static void *thread_main(void *ptr)
{
	unsigned char ret;
	unsigned char addr,port;
	user_activity *p = (user_activity*)ptr;
	for(;;) {
		sleep(10);
	}
}

/*
 * 这是一个发送数据包的通用函数：
 * dat:表示具体要发送的数据
 * len:表示具体要发送的数据的内容
 *返回０表示异常
 *返回１表示成功
 *返回２表示失败
 * */
unsigned char u11::send_package(unsigned char *dat, unsigned char len, ISend *psender)
{
	unsigned char ret=0;
/*step 1*/
	psender->trigerSend((const char *)dat);
	for(;;) {
/*step 2*/
		ret = psender->isResultOk();
		if (ret == 1) {
			printf("send datas ok:");
/*step 3*/
			break;
		} else if (ret == 2) {
			printf("send datas fail\n");
			break;
		} else {
			usleep(100000);
		}
	}
	return ret;
}

/*初始化后流程等等*/
int u11::init_ok()
{
        int ret;
	m_init = 1;
        pthread_t id;
	m_sended = 1;
	isender_fan.setUserObj((user_activity *)this);
	isender_leddisp.setUserObj((user_activity *)this);

        ret = pthread_create(&id, NULL, thread_main, this);
        if(ret) {
		return 1;
        }

        ret = pthread_create(&id, NULL, thread_handle_rev_datas, this);
        if(ret) {
                printf("create pthread_handle_rev_datas error\n");
                return 3;
        }
	return 0;
}

/*这个是从comm 类调过来的，当有数据来时, 并且已确认就是给自己(1:81)的会来到这个类对像*/
/*所有收到的数据都会放到data, data[len]=='\0',len 表示整个包的长度*/
void u11::receive_listener(unsigned char *data, unsigned char len)
{
        memset(rev_buff, 0, 255);
	data[len] = 0;
	if (m_sended == 1) {
		memcpy(rev_buff, data, len);
		rev_len = len;
		pthread_cond_broadcast(&g_cond);
	}
}

/*对外接口:
 * buf:本地地址(addr:port)，远程地址(addr:port) 发送数据
 *以设置风扇为例：
 *<1.81->3.90-fanspeed> 	(这是服务器过来)
 *1.81->3.90-fanoff	(这是服务器过来的格式)
 *http://mountain.eicp.net:19999/test.php?id=1.81->3.90-ledoff'
 * */
int u11::send_net_package(unsigned char *buf, unsigned char *len)
{
	unsigned char l;
	unsigned char addr;
	unsigned char port;
	unsigned char ret;
	int a, b, c, d;

	char *p_cvt;
	unsigned char *p_cvt_datas;
	unsigned char *cvt_datas_len;
	unsigned char out_datas[1024];

	unsigned char tempbuf[32];
	unsigned char ctxbuf[32];
	memset(tempbuf, 0x00, 32);
	memset(ctxbuf, 0x00, 32);
	memcpy(tempbuf, buf, *len);

/*判断
 *用户输入的从网址过来的数据进行分析：
 *１、发送方的地址，以及接收方的地址是否符合。包括，对像的地址以及这个对像所连接的远程地址
 */
	ret = check_addr_ex((const char *)tempbuf);
	if (ret == 0) {
		printf("check_addr_ex fail!!!\n");
		return 0;
	}

	if (m_comm->get_status() == STATUS_LISTEN) {
		printf("comm no client!!!\n");
		return 0;
	}

	sscanf((const char *)tempbuf, "%d.%d->%d.%d-%s", &a, &b, &c, &d, ctxbuf);

/*这里要做出选择吗*/
	if(strncmp(FAN_REMOTE_KEYWORD, ctxbuf, strlen(FAN_REMOTE_KEYWORD))==0) {
		ret = send_package((unsigned char *)ctxbuf, strlen((const char *)ctxbuf), &isender_fan);


	} else if (strncmp(LEDDISP_FONT_KEYWORD, ctxbuf, strlen(LEDDISP_FONT_KEYWORD))==0) {
/*0、预处理*/
		p_cvt = (char *)(ctxbuf+1);
		cvt_main(p_cvt);
		p_cvt_datas = get_datas();
		cvt_datas_len = get_count_8x16();
/*1、发送头信息*/
/*
 *命令：
 *１、<ra rp la lp><index><f><b><fg><bg><addr><count>
 *      f：指的是关键字
 *      b:指的是类型：开始，帧的起始码
 *      fg:表示前景色
 *      bg:表示背景色
 *      addr:地址
 *      count:数据总长度,表示多少个8x16
 * */
	out_datas[0] = 'f';
	out_datas[1] = 'b';

	out_datas[2] = '4';
	out_datas[3] = '0';

	out_datas[4] = '0';
	out_datas[5] = cvt_datas_len;

	ret = send_package((unsigned char *)out_datas, 6, &isender_leddisp);
/*2、发送数据*/
/*
 *命令：
 *2、<ra rp la lp><index><f><d><contents>
 *      f：指的是关键字
 *      d:指的是类型：数据，帧内容
 *      contents:数据16 固定字节
 * */

	for(i=0;i<cvt_datas_len;i+=16) {
		out_datas[0] = 'f';
		out_datas[1] = 'd';
		memcpy(&out_datas[2], p_cvt_data[i],16);
		ret = send_package((unsigned char *)out_datas, 18, &isender_leddisp);
		
	}

/*3、发送结束*/
/*
 *命令：
 *3、<ra rp la lp><index><f><e>
 *      f：指的是关键字
 *      e:指的是类型：帧结束
 * */
	out_datas[0] = 'f';
	out_datas[1] = 'e';
	ret = send_package((unsigned char *)out_datas, 2, &isender_leddisp);

	}

	if (ret == 1) {
		l = strlen((const char *)&rev_buff[4]);
		printf("send_net_package:ack ok\n");
		memcpy((void *)buf, (const void *)&rev_buff[4], l);
		*len = l;
	} else if (ret == 2) {
		printf("send_net_package:(ack fail)\n");
		return 0;
	} else {
		printf("send_net_package:(ack exception)\n");
		return 0;
	}
	return 1;
}
