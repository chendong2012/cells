#include <stdio.h>
#include <stdlib.h>
#include "comm.h"
#include <pthread.h>
#include "u1.h"
#include "user_activity.h"
#include "ISend.h"
#include <sys/time.h>
#include <errno.h>

static int gettime(char *buf);
static void cb_msg(unsigned char *dat, unsigned char len)
{
	printf("u1 cb_msg\n");
}

ISend isender_brd("brd", cb_msg);
u1::u1(void)
{
	m_sended = 1;
	m_init = 0;
	memset(rev_buff, 0, 255);
	rev_len = 0;
}
/*作为服务器端
广播给系统
*/

static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;

static pthread_mutex_t g_mutex_net = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond_net = PTHREAD_COND_INITIALIZER;

/*主动发送*/
static void *thread_main(void *ptr)
{
	char buf[32];
	user_activity *p = (user_activity*)ptr;
	for(;;) {
		sleep(30);
		if (p->m_comm->get_status() == STATUS_LISTEN) {
			printf("comm no client!!!\n");
		}
		gettime(buf);
	}
}

/*初始化后流程等等*/
int u1::init_ok()
{
        int ret;
	m_init = 1;
        pthread_t id;
	isender_brd.setUserObj((user_activity *)this);
	isender_brd.setSendType(1);

        ret = pthread_create(&id, NULL, thread_main, this);
        if(ret) {
                printf("create pthread_main error\n");
		return 1;
        }
	return 0;
}

void u1::receive_listener(unsigned char *data, unsigned char len)
{
	printf("u1::receive_listener\n");
        memset(rev_buff, 0, 255);
	data[len] = 0;

	if (m_sended == 1) {
		memcpy(rev_buff, data, len);
		rev_len = len;
		pthread_cond_broadcast(&g_cond);
	}
}

static int gettime(char *buf)
{
	struct tm  *ptm;
	long   ts;
	int    y,m,d,h,n,s;
	ts = time(NULL);
	ptm = localtime(&ts);

	y = ptm->tm_year+1900;  //年
	m = ptm->tm_mon+1;      //月
	d = ptm->tm_mday;       //日
	h = ptm->tm_hour;       //时
	n = ptm->tm_min;        //分
	s = ptm->tm_sec;        //秒 
	sprintf(buf,"brd%04d,%02d,%02d,%02d,%02d,%02d",y,m,d,h,n,s);
	isender_brd.trigerSend(buf);
}
