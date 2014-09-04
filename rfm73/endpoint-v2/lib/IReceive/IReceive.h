#ifndef __IRECEIVE__H
#define __IRECEIVE__H
#include <Arduino.h>
#include <user_activity.h>
#include "public.h"

class IReceive
{
public:
        const char *item;
	unsigned char index;
	unsigned char ack_buf[PACKAGE_LEN];
	unsigned char ack_len;

	IReceive(const char *cmdstr,void (*cb)(unsigned char *dat, unsigned char len));
	void (*_cb)(unsigned char *dat, unsigned char len);

	void setCmdStr(const char *cmdstr);
	void msg_handler(unsigned char *dat, unsigned char len);
	boolean isNewPackage(unsigned char *dat);

	void initAckBuf();
	void saveAckBuf(unsigned char *buf, unsigned  char len);
	void saveUserBufToAckBuf(unsigned char *buf, unsigned  char len);

	unsigned char *getAckBuf();
	unsigned char getAckBufLen();
	unsigned char getCmdLen(void);
	unsigned char is_contain_userdata(unsigned char *dat, unsigned char len, unsigned char *var);
	unsigned char *getKeyword(void);
	unsigned char getKeywordLen(void);
	unsigned char compare_keyword(unsigned char *dat);
};
#endif
