#include <stdio.h>
#include <stdlib.h>
#include "comm.h"
#include <pthread.h>
#include "u2.h"


#include <sys/time.h>
#include <errno.h>

u2::u2(void)
{
	printf("hello i am u2\n");
	m_sended = 1;
	m_init = 0;
	a = 5;
	memset(rev_buff, 0, 255);
}

/*作为服务器端
如有消息到达，
1、广播给系统
2、把消息传出去

*/
static int g_count = 0;
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;
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
/*主要用来接收到要处理的信息*/
static void *thread_handle_rev_datas(void *ptr)
{
	unsigned char saddr,sport,raddr,rport, id;	
	int ret;

	struct timeval now;
	struct timespec outtime;

	user_activity *p = (user_activity*)ptr;
	sleep(1);
        for(;;) {
		gettimeofday(&now, NULL);
		outtime.tv_sec = now.tv_sec + 5;
		outtime.tv_nsec = 0;

                pthread_mutex_lock(&g_mutex);

		ret = pthread_cond_timedwait(&g_cond, &g_mutex, &outtime);
#if 1
		if(ret == ETIMEDOUT) {
		//	if (p->m_comm->get_status() == CLIENT_STATUS_CONNECTING) {
				/*die server*/
				printf("u2 thread wait tmeout!\n");
				pthread_mutex_unlock(&g_mutex);
				continue;
		//	}
		}
#endif
		p->m_comm->get_local_addr(&saddr, &sport);
		p->m_comm->get_remote_addr(&raddr, &rport);

		if (p->rev_buff[4] == 'a') {
			printf("i am client(%d:%d) ,", saddr, sport);
			printf("get data from serser(%d:%d):%s\n",raddr,rport,&p->rev_buff[4]);
		}
                pthread_mutex_unlock(&g_mutex);
        }
}

/*主动发送*/
static void *thread_main(void *ptr)
{
	unsigned char i = 0;
	int ret;
	char buf[23];
        pthread_t id;
	user_activity *p = (user_activity*)ptr; 
#if 1
        sleep(1);

	for (;;) {
		i = p->m_comm->connect("1", 1);
		if (i==0) {
			printf("not connected\n");
			sleep(2);
		} else {
			printf("connected....\n");
			break;
		}
	}
#endif

#if 0
        ret = pthread_create(&id, NULL, thread1, p);
        if(ret) {
                printf("create pthread1 error\n");
        }
#endif

        ret = pthread_create(&id, NULL, thread_handle_rev_datas, p);
        if(ret) {
                printf("create pthread2 error\n");
        }
#if 1
	sleep(2);
	i = '0';
        for(;;) {
			sprintf(buf,"cetdat%c", i);
                        p->m_comm->send(buf, 7);
			i++;
			if (i > '9')i='0';
                        sleep(6);
        }
#endif

}

int u2::init_ok()
{
        int ret;
	m_init = 1;
        pthread_t id;
        ret = pthread_create(&id, NULL, thread_main, this);
        if(ret) {
                printf("create pthread_main error\n");
		return 1;
        }
	return 0;
}

void u2::receive_listener(unsigned char *data, unsigned char len)
{
	data[len] = 0;
	memset(rev_buff, 0, 255);
//	printf ("received:%s\n", data);
	if (m_sended == 1) {
		memcpy(rev_buff, data, len);
		pthread_cond_broadcast(&g_cond);
	}
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
