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
//	IReceive(const char *cmdstr,void (*cb)(void));
	IReceive(const char *cmdstr);
	void (*_callback)(void);
	void setCmdStr(const char *cmdstr);
	void msg_handler(unsigned char *dat, unsigned char len);
	boolean isNewPackage(unsigned char *dat);
	virtual void onReceive(unsigned char *dat, unsigned char len);

	void saveAckBuf(unsigned char *buf, unsigned  char len);
	unsigned char *getAckBuf();
	unsigned char getAckBufLen();
	void clearAckBuf();
};
#endif
