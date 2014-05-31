#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

#include <pthread.h>
#include <sys/time.h>

#include "comm.h"
#include "rfm70.h"
#include "user_activity.h"
#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))
/******************************************************************
连接阶段：
1、服务器端：
	初始化是设定好是服务器端，接收到客户端的连接消息（‘１’），设置对像状态位为１，表示连接了

2、客户端，初始化要设定好是客户端，首先主动发送连接消息（‘１’），
	当收到服务器端发回来的应答消息后，设置对像状态为１，表示连接了 (的规定的时间内)

3、到此实始化阶段结束。

例子说明：
客户端：
	发送:本机地址，本机端口，对方地址，对方端口，'1' (等待在指定的时间内是否收到)
服务器：
	收到:对方地址，对方端口，本机地址，本机端口，'1' 
			<会显示到屏幕:收到客户端的请求消息XXXXXX>
		
	发送:本机地址，本机端口，对方地址，对方端口，'2'
客户端：
	收到:对方地址，对方端口，本机地址，本机端口，'2' <会显示到屏幕>
			<会显示到屏幕:收到服务器的应答消息XXXXXX>

到此结束
*******************************************************************/

/******************************************************************
通信阶段
１、服务器或是客户端都可以主动发：
		１、发送
		２、等待结果(可选)
		３、作为反应（可选，可以回复，也可以不回）
	扩展功能的实现：
		命令列表可以从外来的文本文件读回来
		对应不同的命令可以返回不同的内容
*******************************************************************/

/******************************************************************
这个类实现了什么功能：

１、可以用来发送数据到指定的地址
２、当有数据到达时会自动调用read接口，然后可以对数据进行分析，
　　最后要不要发回去，由用户自由决定
*******************************************************************/

/******************************************************************
 *运行方法：
 *如果是服务器端：设置好通信的端口，然后就是等中断，如果有中断来了
 *，把收到的数据取出来分析：１、是否是客户端来连接的消息：
 格式： 第一字节：发送方的地址，
 　　　 第二字节：发送方的端口号，
        第三字节：接收方的地址
        第四字节：接收方的端口号
        第五字节：是命令字
                如果是连接消息，那么就返回应答就可以了send("ack\n");
 *
 * ****************************************************************/

COMM::COMM(unsigned char id, RFM70 *rfm, user_activity *activity)
{
        printf("comm is running!!!\n");
        is_server = id;
	if (is_server == I_AM_SERVER) {
		status = STATUS_LISTEN;
	} else {
		status = CLIENT_STATUS_CONNECTING;
	}
        memset(rcv_buff,0,PACKAGE_LEN);
	m_activity = activity;
	m_rfm = rfm;
	watchdog_count = 0;
	m_activity->set_comm(this);

	m_activity->init_ok();
	init_watchdog();
#if 0
	if (is_server == I_AM_CLIENT) {
		m_activity->init_ok();
	}
#endif
}

void COMM::read(unsigned char *buf, unsigned char len)
{
	memcpy((void *)rcv_buff, (const void *)buf, PACKAGE_LEN);
#if 0
    //    printf("len 0x%02x:", len);
        printf("%s", rcv_buff);
        printf("\n");
#endif

        /**********
        0,dst_addr;
        1,dst_port;
        2,src_addr;
        3,src_port
        ***********/
	if (rcv_buff[2] != src_addr || (rcv_buff[3] != src_port)) {
		return;	
	}

/*server connect part*/
	if (is_server == I_AM_SERVER) {
		if (rcv_buff[4] == CONNECT) {
/*只要是来连接的，都让之连接就行*/
		//	if (status == STATUS_LISTEN) {
				set_remote_addr(rcv_buff[0], rcv_buff[1]);
				conect_ack(); //ack client
				status = STATUS_CONNECTED;
				printf("connected:(%d:%d)<--server-client---> \
				(%d:%d)\n", \
				rcv_buff[2], \
				rcv_buff[3],\
				rcv_buff[0],\
				rcv_buff[1]);
				
				return;
		//	} else {
			//	printf("the server aleady connected!!!\n");
		//		;
		//	}

		} else {
			set_remote_addr(rcv_buff[0], rcv_buff[1]);
			if (status == STATUS_LISTEN) {
				printf("connected:(%d:%d)<--server-client---> \
				(%d:%d)\n", \
				rcv_buff[2], \
				rcv_buff[3],\
				rcv_buff[0],\
				rcv_buff[1]);
			}
			status = STATUS_CONNECTED;
		}
	}

/*client connect part*/
	if (is_server == I_AM_CLIENT) {
		if (status == CLIENT_STATUS_CONNECTING) {
			if (rcv_buff[4] == CONNECT_ACK) {
				status = CLIENT_STATUS_CONNECTED;
				printf("connected:(%d:%d)<--server-client--->(%d:%d)\n", rcv_buff[0], rcv_buff[1],rcv_buff[2], rcv_buff[3]);
			} else {
				return;
			}
			return;
		}
	}
/*[0],[1] 表示服务器，[2],[3]表示客户端*/
/*src_addr:本地的地址*/
/*dst_addr:本地的地址*/
	m_activity->receive_listener(rcv_buff, len); //交给对像去接收
}

void COMM::send(const char *buf, unsigned char len)
{
	unsigned char ret;
        unsigned char sendbuf[32];
        sendbuf[0] = src_addr;
        sendbuf[1] = src_port;
        sendbuf[2] = dst_addr;
        sendbuf[3] = dst_port;

        memcpy((void *)&sendbuf[4], (const void *)buf, len);
	len = len+4;
/*增加发送超时功能,如果超过一定时间没有响应表示，

1\如果是服务器：就处在侦听状态中
2\如果是客户端，就处在连接中

*/
	return m_rfm->send(sendbuf, len);
}

/*only for client*/
int COMM::connect(const char *buf, unsigned char len)
{
        unsigned char sendbuf[32];

        sendbuf[0] = src_addr;
        sendbuf[1] = src_port;
        sendbuf[2] = dst_addr;
        sendbuf[3] = dst_port;

        memcpy((void *)&sendbuf[4], (const void *)buf, len);
	len = len+4;
	m_rfm->send(sendbuf, len);
	usleep(10000);

	if (status == CLIENT_STATUS_CONNECTED) {
		return 1; //connect ok
	} else {
		return 0; //connect fail
	}
}

/*only for server:connect ack client*/
void COMM::conect_ack(void)
{
	send("2", 1); //connect ack client
}

void COMM::set_local_addr(unsigned char addr, unsigned char port)
{
        src_addr = addr;
        src_port = port;
}

void COMM::set_remote_addr(unsigned char addr, unsigned char port)
{
        dst_addr = addr;
        dst_port = port;
}

void COMM::reset_listen_status(void)
{
	watchdog_count = 0;
	status = STATUS_LISTEN;
}

void COMM::reset_client_status(void)
{
	watchdog_count = 0;
	status = CLIENT_STATUS_CONNECTING;
}

unsigned char COMM::get_status(void)
{
	return status;
}

/*=============================================================*/
static void *thread_watchdog(void *ptr)
{
#if 0
	static int timeout = 0;
	COMM *p = (COMM *)ptr;
	unsigned char temp_status;
#endif
}

void COMM::init_watchdog(void)
{
#if 0
	int ret;
        pthread_t id;
        ret = pthread_create(&id, NULL, thread_watchdog, this);
        if(ret) {
                printf("create thread_watchdog error\n");
        }
#endif
}
#if 0
static void *thread_watchdog_client(void *ptr)
{
	COMM *p = (COMM *)ptr;
	unsigned char temp_status;
	int ret;
	for(;;) {
		sleep(60);
		temp_status = p->get_status();
		if (temp_status == CLIENT_STATUS_CONNECTED) {
			if (p->watchdog_count > 2) {
				p->reset_client_status();
				printf("thread watchdog client timeout\n");
				continue;
			}
			p->send("watchdog", 8);
			p->watchdog_count++;
		} else if (temp_status == CLIENT_STATUS_CONNECTING){
			printf("thread watchdog client send connect msg\n");
			p->connect("1", 1);
		}
	}
}

void COMM::init_watchdog_client(void)
{
	int ret;
        pthread_t id;
        ret = pthread_create(&id, NULL, thread_watchdog_client, this);
        if(ret) {
                printf("create thread_watchdog_client error\n");
        }
}
#endif
