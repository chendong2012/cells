#include <stdio.h>
#include <stdlib.h>
#include "comm.h"
#include <pthread.h>
#include "u1.h"
#include "user_activity.h"
#include "ISend.h"
#include <sys/time.h>
#include <errno.h>
#include "public.h"

ISend isender_brd(BRD_SEND_KEYWORD, NULL);

static void broadcast_time(void);

u1::u1(void)
{
}

static void *thread_broadcast_time(void *ptr)
{
	char buf[32];
	user_activity *p = (user_activity*)ptr;
	for(;;) {
		sleep(5);
		if (p->m_comm->get_status() == STATUS_LISTEN) {
			printf("broadcast no clients!!!\n");
		}
		broadcast_time();
	}
}

/*初始化后流程等等*/
int u1::init_ok()
{
        int ret;
        pthread_t id;

	isender_brd.setUserObj((user_activity *)this);

/*表示发送完一次后，就结束，不会再去等远程的返回信息*/
	isender_brd.setSendType(1);

	memset(rev_buff, 0, 255);

        ret = pthread_create(&id, NULL, thread_broadcast_time, this);
        if(ret) {
                printf("create pthread_broadcast_time error\n");
		return 1;
        }
	return 0;
}

void u1::receive_listener(unsigned char *data, unsigned char len)
{
	;
}

static void fetch_server_time(char *buf)
{
	struct tm *ptm;
	long ts;
	int y,m,d,h,n,s;
	ts = time(NULL);
	ptm = localtime(&ts);

	y = ptm->tm_year+1900;  //年
	m = ptm->tm_mon+1;      //月
	d = ptm->tm_mday;       //日
	h = ptm->tm_hour;       //时
	n = ptm->tm_min;        //分
	s = ptm->tm_sec;        //秒 

	sprintf(buf,"brd%04d,%02d,%02d,%02d,%02d,%02d",y,m,d,h,n,s);
}

static void broadcast_time(void)
{
	char buf[32];
	memset(buf, 0, 32);
	fetch_server_time(buf);
	isender_brd.trigerSend(buf);
}
