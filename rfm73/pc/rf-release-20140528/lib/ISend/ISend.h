#ifndef __ISEND__H
#define __ISEND__H
#include <CallMe.h>
#include <Arduino.h>
#define MAX_TRY 10
#define CMD_MAX_LEN 10
enum {
	S_I,
	S_S,
	S_A,
	S_N
};

class ISend
{
public:
        char item[CMD_MAX_LEN];
	unsigned char item_len;
	unsigned char index;
	unsigned char status;
	CallMe *cm;
	unsigned char trys;


	ISend(const char *cmdstr);
	virtual void onReceive(unsigned char *dat, unsigned char len);
	virtual void sendRfDatas();
	void setStatus(unsigned char s);
	void setCmdStr(unsigned char *cmdstr);
	void msg_handler(unsigned char *dat, unsigned char len);

	unsigned char getStatus();
	void trigerSend(void);
	void disableSend(void);
	static boolean send_cb(ISend *me);
	unsigned char setindex();

};
#endif

