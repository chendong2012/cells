#include <stdio.h>
#include <stdlib.h>
#include "user_activity.h"
#include "comm.h"
#include <pthread.h>

/*作为服务器端
如有消息到达，
1、广播给系统
2、把消息传出去

*/
#if 0
static int g_count = 0;
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;

void *thread1(void *ptr)
{
	user_activity *p = (user_activity*)ptr; 
sleep(1);
        for(;;) {
                pthread_mutex_lock(&g_mutex);
                pthread_cond_wait(&g_cond, &g_mutex);
      //          printf("thread2: %s\n",p->rev_buff);
		
                pthread_mutex_unlock(&g_mutex);
        }
}
/*主要用来接收到要处理的信息*/
void *thread2(void *ptr)
{
        static int ii=0;

	user_activity *p = (user_activity*)ptr;
	sleep(1);
        for(;;) {
                pthread_mutex_lock(&g_mutex);
                pthread_cond_wait(&g_cond, &g_mutex);

		if (p->rev_buff[4] == 's') {
			printf("receive%d:%s\n",ii++, &p->rev_buff[4]);
			p->m_comm->send("a12:00", 6);
		}
                pthread_mutex_unlock(&g_mutex);
        }
}

/*主动发送*/
void *thread_main(void *ptr)
{
	for(;;) {
		sleep(5);
	}
}
#endif
user_activity::user_activity()
{
#if 0
	m_sended = 1;
	m_init = 0;
	a = 5;
	memset(rev_buff, 0, 255);
#endif
}

int user_activity::init_ok()
{
#if 0
        int ret;
	m_init = 1;
        pthread_t id;

        ret = pthread_create(&id, NULL, thread_main, this);
        if(ret) {
                printf("create pthread_main error\n");
		return 1;
        }

        ret = pthread_create(&id, NULL, thread1, this);
        if(ret) {
                printf("create pthread1 error\n");
                return 2;
        }

        ret = pthread_create(&id, NULL, thread2, this);
        if(ret) {
                printf("create pthread2 error\n");
                return 3;
        }
#endif
	return 0;
}

void user_activity::receive_listener(unsigned char *data, unsigned char len)
{
#if 0
	data[len] = 0;
//	printf ("received:%s\n", data);
	if (m_sended == 1) {
		memcpy(rev_buff, data, len);
		pthread_cond_broadcast(&g_cond);
	}
#endif
}

void user_activity::set_comm(COMM *comm)
{
	m_comm=comm;
}

void user_activity::ack_timeout(void)
{
}
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


void user_activity::setup_timer(void)
{
        m_rftimer = new rftimer(this);
        m_rftimer->init(t_cb);
        m_rftimer->begin((float)1, (float)(10));
}


/*a1, p1,表示本地地址，a2,p2表示远程地址*/
unsigned char user_activity::check_addr(unsigned char a1, unsigned char p1, unsigned char a2, unsigned char p2)
{
	unsigned char addr;
	unsigned char port;
	m_comm->get_local_addr(&addr, &port);

	if ((a1 != addr) || (p1 != port))
		return 0;

	m_comm->get_remote_addr(&addr, &port);

	if ((a2 != addr) || (p2 != port))
		return 0;
	return 1;
}

/*返回０表示地址不是本地的(这个对像),
 * 输入字符串：1.81->3.90-具体内容...
 *
 * */
unsigned char user_activity::check_addr_ex(const char *s)
{
	int a, b, c, d;
	sscanf(s, "%d.%d->%d.%d-", &a, &b, &c, &d);
	printf("a=%d b=%d c=%d d=%d", a, b , c ,d);
	return check_addr((unsigned char)a, (unsigned char)b, (unsigned char)c, (unsigned char)d);
}

/*
 * 输入字符串：1.81->3.90-具体内容...
 *返回实际的控制命令字符串(具体内容)，输入的是原始字符串
 * */
char *user_activity::get_send_package_ctx(const char *s)
{
	int a, b, c, d;
	char *ctx;
	sscanf(s, "%d.%d->%d.%d-%s", &a, &b, &c, &d, ctx);
	return ctx;
}

/**********************************************/
