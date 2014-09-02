#ifndef __USER_ACTIVITY__
#define __USER_ACTIVITY__
class COMM;
class user_activity {
public:
	user_activity();
	virtual void receive_listener(unsigned char *data, unsigned char len);
	virtual int init_ok(void);
	unsigned char check_addr(unsigned char a1, unsigned char p1, unsigned char a2, unsigned char p2);
	virtual int send_net_package(unsigned char *buf, unsigned char *len)
	{
		return 0;
	}


	void set_comm(COMM *comm);
	COMM *m_comm;
	unsigned char rev_buff[32];
	unsigned char rev_len;
	int m_init;
};
#endif
