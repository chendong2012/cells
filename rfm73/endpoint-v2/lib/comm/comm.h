#ifndef _COMM_H_
#define _COMM_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <rfm73.h>

#define CONNECT '1'
#define CONNECT_ACK '2'
#define COMMNICATE '3'

#define STATUS_LISTEN 0xf0
#define STATUS_CONNECTED 0xf1
#define STATUS_COMM_TIMEOUT 0xf2


#define CLIENT_STATUS_CONNECTING 0Xe0
#define CLIENT_STATUS_CONNECTED 0Xe1
#define CLIENT_STATUS_NOT_CONNECT 0Xe2

#define PACKAGE_LEN	32

class user_activity;
class rfm73;
class COMM {
        public:
	COMM();
	void attach_id(unsigned char id);
        void send(const char *buf, unsigned char len);
	int connect();
        void create(void);
        void read(unsigned char *buf, unsigned char len); 
        void set_remote_addr(unsigned char addr, unsigned char port);
        void set_local_addr(unsigned char addr, unsigned char port);
	void conect_ack(void);
	unsigned char get_status(void);
	void reset_listen_status(void);
	unsigned char is_mydata(byte *p);
	void reset_client_status(void);
	/*new*/
	void attach_user_activity(user_activity *activity);
	void attach_rfm(rfm73 *rfm);


	unsigned char getid(void)
	{
		return is_server;
	}

        void get_local_addr(unsigned char *addr, unsigned char *port)
	{
		*addr = src_addr;
		*port = src_port;
	}
        void get_remote_addr(unsigned char *addr, unsigned char *port)
	{
		*addr = dst_addr;
		*port = dst_port;
	}
/*这是一个发送的注册函数*/
	static void (*sendcb)(unsigned char *buf, unsigned char len);
	unsigned char is_accept_addrs(unsigned char *paddr);
	unsigned char set_accept_addrs(unsigned char addr, unsigned char port);

public:
        unsigned char dst_addr;
        unsigned char dst_port;
        unsigned char src_addr;
        unsigned char src_port;
        unsigned char is_server; /*0 server, 1 client*/
        volatile unsigned char status_bug;	/*0 is free 1 connected*/
        unsigned char rcv_buff[32];
	user_activity *m_activity;
	rfm73 *m_rfm;
	unsigned char _accept_addr;
	unsigned char _accept_port;
};


extern COMM comm;
extern COMM comm_uff;
#endif
