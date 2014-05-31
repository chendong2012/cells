#ifndef __IRECEIVE__H
#define __IRECEIVE__H
#include <Arduino.h>
#include <user_activity.h>

class IReceive
{
public:
        const char *item;
	unsigned char index;
	unsigned char ack_buf[28];
	unsigned char ack_len;

	//IReceive(const char *cmdstr,void (*cb)(unsigned char *dat, unsigned char len));
	IReceive(const char *cmdstr,void (*cb)(void));
	void (*_callback)(void);
	void setCmdStr(const char *cmdstr);
//	void msg_handler(unsigned char *dat, unsigned char len);
	void msg_handler(unsigned char *dat, unsigned char len, user_activity *a);
	boolean isNewPackage(unsigned char *dat);

	void saveAckBuf(unsigned char *buf, unsigned  char len);
	unsigned char *getAckBuf();
	unsigned char getAckBufLen();
	void clearAckBuf();
};
#endif
