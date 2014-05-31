#ifndef __IRECEIVE__H
#define __IRECEIVE__H
#include <Arduino.h>

/*命令字节最多为10bytes*/
#define CMD_MAX_LEN 10

/*应答包字节最多为28bytes*/
#define CMDACK_MAX_LEN 28
class IReceive
{
public:
        char item[CMD_MAX_LEN];
	unsigned char index;
	unsigned char ack_buf[CMDACK_MAX_LEN];
	unsigned char ack_len;

	IReceive(const char *cmdstr);
	virtual void onReceive();
	void setCmdStr(unsigned char *cmdstr);
	void msg_handler(unsigned char *dat, unsigned char len);
	boolean isNewPackage(unsigned char dat);

};
#endif
