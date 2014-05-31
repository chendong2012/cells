#include <stdio.h>
#include <stdlib.h>
#include "comm.h"
#include <pthread.h>
#include "u1.h"
#include "user_activity.h"
#include <sys/time.h>
#include <errno.h>
u1::u1(void)
{
	printf("hello i am u1\n");
	m_sended = 1;
	m_init = 0;
	a = 5;
	memset(rev_buff, 0, 255);
	rev_len = 0;
}

/*作为服务器端
如有消息到达，
1、广播给系统
2、把消息传出去
*/

static int g_count = 0;
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;


static pthread_mutex_t g_mutex_net = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond_net = PTHREAD_COND_INITIALIZER;

#if 0
static void *thread1(void *ptr)
{
	user_activity *p = (user_activity*)ptr; 
	sleep(1);
        for(;;) {
                pthread_mutex_lock(&g_mutex);
                pthread_cond_wait(&g_cond, &g_mutex);
                pthread_mutex_unlock(&g_mutex);
        }

}
#endif

/*主要用来接收到要处理的信息:

1,并做相关的动作
2,回复相关信息等等
*/
static void *thread_handle_rev_datas(void *ptr)
{
	unsigned char saddr,sport,raddr,rport, id;	
	unsigned char len;
	int ret;
	struct timeval now;
	struct timespec outtime;

	char buf[32];
	unsigned char i='0';

	user_activity *p = (user_activity*)ptr;
	sleep(1);
        for(;;) {
		pthread_mutex_lock(&g_mutex);

		gettimeofday(&now, NULL);
		outtime.tv_sec = now.tv_sec + 5;
		outtime.tv_nsec = now.tv_usec * 1000;

		ret = pthread_cond_timedwait(&g_cond, &g_mutex, &outtime);
		if(ret == ETIMEDOUT) {
		//	if (p->m_comm->get_status() == STATUS_LISTEN) {
				/*die client*/
		//		printf("u1 thread wait tmeout!\n");
				pthread_mutex_unlock(&g_mutex);
				continue;
		//	}
		}
		p->m_comm->get_local_addr(&saddr, &sport);
		p->m_comm->get_remote_addr(&raddr, &rport);
		
/*********************handler here********************/
		switch (p->rev_buff[4]) {
		case 'g':
			printf("i am s(%d:%d) ,", saddr, sport);
			printf("get data from c(%d:%d):%s\n",raddr,rport,&p->rev_buff[4]);
                        i++;
                        if (i > '9')i='0';
                        ret = sprintf(buf,"a12:0%c", i);
                        p->m_comm->send(buf, ret);
			break;

		case 'n': //receive data and send to net
			pthread_cond_broadcast(&g_cond_net);
			break;

		default:
			break;
		};
/*********************end********************/

                pthread_mutex_unlock(&g_mutex);
        }
}

/*主动发送*/
static void *thread_main(void *ptr)
{
	for(;;) {
		sleep(5);
	}
}

/*初始化后流程等等*/
int u1::init_ok()
{
        int ret;
	m_init = 1;
        pthread_t id;

        ret = pthread_create(&id, NULL, thread_main, this);
        if(ret) {
                printf("create pthread_main error\n");
		return 1;
        }
#if 0
        ret = pthread_create(&id, NULL, thread1, this);
        if(ret) {
                printf("create pthread1 error\n");
                return 2;
        }
#endif

        ret = pthread_create(&id, NULL, thread_handle_rev_datas, this);
        if(ret) {
                printf("create pthread_handle_rev_datas error\n");
                return 3;
        }
	return 0;
}

/*这个是从comm 类调过来的，当有数据来时, 并且已确认就是给自己的会来到这个类对像*/
/*所有收到的数据都会放到data, data[len]=='\0',len 表示整个包的长度*/
void u1::receive_listener(unsigned char *data, unsigned char len)
{
        memset(rev_buff, 0, 255);
	data[len] = 0;

	if (m_sended == 1) {
		memcpy(rev_buff, data, len);
		rev_len = len;
		pthread_cond_broadcast(&g_cond);
	}
}

/*对外接口*/
int u1::send_net_package(unsigned char *buf, unsigned char *len)
{
	unsigned char l;
	unsigned char addr;
	unsigned char port;
	unsigned char ret;

	struct timeval now;
	struct timespec outtime;

	unsigned char tempbuf[32];
	memset(tempbuf, 0x00, 32);
	memcpy(tempbuf, buf, *len);

/*首先判断是否为自己的地址，如果不是就直接返回０就完了*/
	ret = check_addr(tempbuf[0], tempbuf[1], tempbuf[2], tempbuf[3]);
	if (ret == 0) {
		return 0;
	}

	if (m_comm->get_status() == STATUS_LISTEN) {
		printf("comm no client!!!\n");
		return 0;
	}

	m_comm->send((const char *)(&tempbuf[4]), (*len-4));

	pthread_mutex_lock(&g_mutex_net);

	gettimeofday(&now, NULL);
	outtime.tv_sec = now.tv_sec + 5;
	outtime.tv_nsec = now.tv_usec * 1000;
	pthread_cond_timedwait(&g_cond_net, &g_mutex_net, &outtime);

	if(ret == ETIMEDOUT) {
		if (m_comm->get_status() == STATUS_LISTEN) {
			l = 7;
			printf("next to web ack:timeout\n");
			memcpy((void *)buf, "timeout", 7);
			*len = l;
		}
	} else {

		l = strlen((const char *)&rev_buff[4]);
		printf("next to web ack:%s\n",&rev_buff[4]);
		memcpy((void *)buf, (const void *)&rev_buff[4], l);
		*len = l;
	}

	pthread_mutex_unlock(&g_mutex_net);
	return 1;
}
#if 0
/********************建一个定时器**************************/
static void t_cb(union sigval si)
{
#if 0
        user_activity *this_act;
        rftimer *ptr = (rftimer *)si.sival_ptr;

        this_act=ptr->m_activity;
        printf("timer is over %d\n",this_act->test++);

#endif
}

void user_activity::ack_timeout(void)
{
}

void user_activity::setup_timer(void)
{
        m_rftimer = new rftimer(this);
        m_rftimer->init(t_cb);
        m_rftimer->begin((float)1, (float)(10));
}
#endif
