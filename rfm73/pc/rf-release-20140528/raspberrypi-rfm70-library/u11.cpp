#include <stdio.h>
#include <stdlib.h>
#include "comm.h"
#include <pthread.h>
#include "u11.h"
#include "user_activity.h"
#include <sys/time.h>
#include <errno.h>
#include <ISendCustom.h>
#include <IReceive.h>
#include <string.h>
#include <unistd.h>

user_activity *myu2=NULL;

ISendCustom isender("getstatus");
IReceive irev("gettemp");

IReceive irec("getsvrtm", cb_get_server_timer);

u11::u11(void)
{
	printf("hello i am u11\n");
        myu2 = this;


}

/*作为服务器端
如有消息到达，
1、广播给系统
2、把消息传出去
*/

static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;


/**/
static pthread_mutex_t g_mutex_net = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond_net = PTHREAD_COND_INITIALIZER;

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
	return sprintf(buf,"%02d",s);
//	return sprintf(buf,"%02d:%02d",h,n);
}

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
				pthread_mutex_unlock(&g_mutex);
				continue;
		}
		p->m_comm->get_local_addr(&saddr, &sport);
		p->m_comm->get_remote_addr(&raddr, &rport);
/*add your code here*/
                isender.msg_handler(p->rev_buff, p->rev_len);
                irev.msg_handler(p->rev_buff, p->rev_len);
/*add your code end*/
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

		p->m_comm->get_remote_addr(&addr,&port);
		if (addr == 0) {
			continue;
		}
/*step 1*/
		isender.trigerSend((unsigned char *)"getstatus");
		for(;;) {
/*step 2*/
			ret = isender.isResultOk();
			if (ret == 1) {
				printf("send datas ok:");
/*step 3*/
//				printf("%s\n",isender.getReceiveData());
				break;
			} else if (ret == 2) {
				printf("send datas fail\n");
				break;
			} else {
				usleep(100000);
			}
		}
	}
}

/*初始化后流程等等*/
int u11::init_ok()
{
        int ret;
	m_init = 1;
        pthread_t id;
	m_sended = 1;
	isender.setUserObj((user_activity *)this);

        ret = pthread_create(&id, NULL, thread_main, this);
        if(ret) {
                printf("create pthread_main error\n");
		return 1;
        }

        ret = pthread_create(&id, NULL, thread_handle_rev_datas, this);
        if(ret) {
                printf("create pthread_handle_rev_datas error\n");
                return 3;
        }
	return 0;
}

/*这个是从comm 类调过来的，当有数据来时, 并且已确认就是给自己的会来到这个类对像*/
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

/*对外接口*/
int u11::send_net_package(unsigned char *buf, unsigned char *len)
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

/*收到客户端要获取服务器端的时间，进行应答,看看是否是第一次发过来的同一条命令，
如是的话，执行动作并返回
如是同一条命令，但是第二次以上发送的，就把老结果返回去*/
static void cb_get_server_timer(unsigned char *dat, unsigned char len)
{
	/*第５个字节开始才是内容*/
        if(irec.isNewPackage(dat)) {
                myu2->m_comm->send("getsvrtm:12:00", 14);
                irec.saveAckBuf((unsigned char *)"getsvrtm:12:00", 14);
        } else {
                myu2->m_comm->send((const char *)irec.getAckBuf(), irec.getAckBufLen());
        }
}

