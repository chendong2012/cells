#include <string.h>
#include "comm.h"
#include "rfm73.h"
#include "user_activity.h"
#include "public.h"
#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))
//unsigned char aa,bb,cc,dd;
//unsigned char status_bug;
COMM::COMM()
{
	
        memset(rcv_buff,0,PACKAGE_LEN);
	m_activity = NULL;
	m_rfm = NULL;
}

/*new: attach activity*/
void COMM::attach_user_activity(user_activity *activity)
{
	//m_activity = &act_uff;
	m_activity = activity;
	m_activity->set_comm(this);
}

/*new: attach activity*/
void COMM::attach_rfm(rfm73 *rfm)
{
	m_rfm = rfm;
}

unsigned char COMM::is_mydata(byte *p)
{
	/*
	if ((p[1] == 0xff) && (p[0] == dst_addr)) {
		return 1;
	}

	sprintf(g_debug, "%d-%d-%d-%d", p[0], p[1], p[2], p[3]);
	Serial.println((const char *)g_debug);
	*/

	if(is_accept_addrs(p)) {
		return 1;
	}
//	sprintf(g_debug, "c:%d-%d-%d-%d", p[0], p[1], p[2], p[3]);
//	Serial.println((const char *)g_debug);
//	sprintf(g_debug, "c:%d-%d-%d-%d", aa, bb, cc, dd);
//	Serial.println((const char *)g_debug);
	if ((p[0] == dst_addr) && (p[1] == dst_port) && \
	    (p[2] == src_addr) && (p[3] == src_port)) {
//	if ((p[0] == cc) && (p[1] == dd) && \
//	    (p[2] == aa) && (p[3] == bb)) {
		return 1;
	} else {

		return 0;
	}
}

void COMM::read(unsigned char *buf, unsigned char len)
{
	if (m_activity == NULL)
		return;
	if (m_rfm == NULL)
		return;

	memcpy((void *)rcv_buff, (const void *)buf, PACKAGE_LEN);

        /**********
        0,dst_addr;
        1,dst_port;
        2,src_addr;
        3,src_port
        ***********/
	if (!is_mydata(rcv_buff)) {
//	if (rcv_buff[2] != src_addr || (rcv_buff[3] != src_port)) {

		/*remote is xx.0xf and local is xx.0xf will pass*/
		if ((rcv_buff[1] == 0x0f) && (is_server == I_AM_CLIENT) && (src_port==0xf)) {
			;
		} else {
			return;	
		}
	}

/*server connect part*/
	if (is_server == I_AM_SERVER) {
		if (rcv_buff[4] == CONNECT) {
/*只要是来连接的，都让之连接就行*/
		//	if (status == STATUS_LISTEN) {
				set_remote_addr(rcv_buff[0], rcv_buff[1]);
				conect_ack(); //ack client
				status_bug = STATUS_CONNECTED;
#if 0 
				printf("connected:(%d:%d)<--server-client---> \
				(%d:%d)\n", \
				rcv_buff[2], \
				rcv_buff[3],\
				rcv_buff[0],\
				rcv_buff[1]);
#endif
				
				return;
		//	} else {
			//	printf("the server aleady connected!!!\n");
		//		;
		//	}

		} else {
			set_remote_addr(rcv_buff[0], rcv_buff[1]);
			if (status_bug == STATUS_LISTEN) {
#if 0
				printf("connected:(%d:%d)<--server-client---> \
				(%d:%d)\n", \
				rcv_buff[2], \
				rcv_buff[3],\
				rcv_buff[0],\
				rcv_buff[1]);
#endif
			}
			status_bug = STATUS_CONNECTED;
		}
	}

/*client connect part*/
	if (is_server == I_AM_CLIENT) {
		if (status_bug == CLIENT_STATUS_CONNECTING) {
			if (rcv_buff[4] == CONNECT_ACK) {
				status_bug = CLIENT_STATUS_CONNECTED;
				
//	sprintf(g_debug, "s9:%d", status_bug);
//	Serial.println((const char *)g_debug);
#if 0
				printf("connected:(%d:%d)<--server-client--->(%d:%d)\n", rcv_buff[0], rcv_buff[1],rcv_buff[2], rcv_buff[3]);
#endif
			} else {
				return;
			}
			return;
		}
	}
	m_activity->receive_listener(rcv_buff, len); //交给对像去接收
	//sprintf(g_debug, "s3:%s", rcv_buff);
	//Serial.println((const char *)g_debug);
}

static unsigned char sendbuf[32];
void COMM::send(const char *buf, unsigned char len)
{
	unsigned char ret;
	memset(sendbuf, 0, 32);
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
int COMM::connect()
{
        unsigned char sendbuf[5];

        sendbuf[0] = src_addr;
        sendbuf[1] = src_port;
        sendbuf[2] = dst_addr;
        sendbuf[3] = dst_port;
        sendbuf[4] = '1';
	m_rfm->send(sendbuf, 5);
	delay(10);

	if (status_bug == CLIENT_STATUS_CONNECTED) {
	//sprintf(g_debug, "s2:%d", status_bug);
	//Serial.println((const char *)g_debug);
		return 1; //connect ok
	} else {
	//sprintf(g_debug, "s1:%d", status_bug);
	//Serial.println((const char *)g_debug);
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
		//sprintf(g_debug, "cug:%d-%d-%d-%d", dst_addr, dst_port, src_addr, src_port);
		//Serial.println((const char *)g_debug);
}

void COMM::reset_listen_status(void)
{
	status_bug = STATUS_LISTEN;
}

void COMM::reset_client_status(void)
{
//	status = CLIENT_STATUS_CONNECTING;
}

unsigned char COMM::get_status(void)
{
	return status_bug;
}

void COMM::attach_id(unsigned char id)
{
	is_server = id;
	if (is_server == I_AM_SERVER) {
		status_bug = STATUS_LISTEN;
	} else {
		status_bug = CLIENT_STATUS_CONNECTING;
	}
}

unsigned char COMM::is_accept_addrs(unsigned char *paddr)
{
	if ((paddr[0] == _accept_addr) && (paddr[1] == _accept_port)) {
		return 1;
	}
}

unsigned char COMM::set_accept_addrs(unsigned char addr, unsigned char port)
{
	_accept_addr = addr;
	_accept_port = port;
}

COMM comm;
COMM comm_uff;
