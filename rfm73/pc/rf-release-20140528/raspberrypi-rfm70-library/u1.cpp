#include <stdio.h>
#include <stdlib.h>
#include "comm.h"
#include <pthread.h>
#include "u1.h"
#include "user_activity.h"
#include <sys/time.h>
#include <errno.h>

static unsigned char send_brd(unsigned char *dat, unsigned char len, ISend *psender);
ISend isender("brd", cb_sent_backmsg);
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
	user_activity *p = (user_activity*)ptr;
	for(;;) {
		sleep(5);
        isender.trigerSend("brd1200");
	}
}

/*初始化后流程等等*/
int u1::init_ok()
{
        int ret;
	m_init = 1;
        pthread_t id;
	isender.setUserObj((user_activity *)this);

	isender.setSendType(1);

        ret = pthread_create(&id, NULL, thread_main, this);
        if(ret) {
                printf("create pthread_main error\n");
		return 1;
        }
	return 0;
}

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

static unsigned char send_brd(unsigned char *dat, unsigned char len, ISend *psender)
{
        psender->trigerSend((const char *)dat);
}
