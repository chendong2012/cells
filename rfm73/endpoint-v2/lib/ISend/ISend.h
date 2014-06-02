#ifndef __ISEND__H
#define __ISEND__H
#include <Arduino.h>
#define MAX_TRY 10
#define PACKAGE_LEN 32
enum {
	S_I,
	S_S,
	S_A,
	S_N
};
enum {
	RLT_INIT,
	RLT_OK,
	RLT_FAIL
};


class ISend
{
public:
        char item[PACKAGE_LEN];
	unsigned char item_len;
	unsigned char index;
	unsigned char status;
	unsigned char trys;
	unsigned char send_result;
	unsigned char strAck[PACKAGE_LEN];
	unsigned char strAckLen;

	ISend(const char *cmdstr);
	void setStatus(unsigned char s);
	void setCmdStr(const char *cmdstr);
	void msg_handler(unsigned char *dat, unsigned char len);/*消息处理*/
	unsigned char setSendResult(unsigned char result);
	unsigned char getSendResult(void);

	unsigned char getStatus();
	unsigned char trigerSend(const char *s);
	void disableSend(void);
	static boolean send_cb(ISend *me);
	unsigned char setindex();
	unsigned char creat_send_thread();
	unsigned char isResultOk(void);
        void  sendRfDatas();/*发送数据*/
	unsigned char clearAckData();
};
#endif
