#ifndef __IRECEIVE__H
#define __IRECEIVE__H
#include <user_activity.h>

class IReceive
{
public:
        const char *item;
	unsigned char index;
	unsigned char ack_buf[32];
	unsigned char ack_len;

	IReceive(const char *cmdstr,void (*cb)(unsigned char *dat, unsigned char len));
	void (*_cb)(unsigned char *dat, unsigned char len);

	void setCmdStr(const char *cmdstr);
	void msg_handler(unsigned char *dat, unsigned char len);
	boolean isNewPackage(unsigned char *dat);

	void clearAckBuf();
	void saveAckBuf(unsigned char *buf, unsigned  char len);

	unsigned char *getAckBuf();
	unsigned char getAckBufLen();
};
#endif
