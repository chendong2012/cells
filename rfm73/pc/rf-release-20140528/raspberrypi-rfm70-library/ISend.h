#ifndef __ISEND__H
#define __ISEND__H
#define MAX_TRY 10
#define CMD_MAX_LEN 16
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
        char item[CMD_MAX_LEN];
	unsigned char item_len;
	unsigned char index;
	unsigned char status;
	unsigned char trys;
	unsigned char send_result;
	unsigned char strRec[32];
	unsigned char strRecLen;


	ISend(const char *cmdstr);
	virtual void onReceive(unsigned char *dat, unsigned char len);
	virtual void sendRfDatas();
	void setStatus(unsigned char s);
	void setCmdStr(unsigned char *cmdstr);
	void msg_handler(unsigned char *dat, unsigned char len);
	unsigned char setSendResult(unsigned char result);
	unsigned char getSendResult(void);

	unsigned char getStatus();
	unsigned char trigerSend(unsigned char *s);
	void disableSend(void);
	static void *send_cb(void *ptr);
	unsigned char setindex();
	unsigned char creat_send_thread();
	unsigned char *getReceiveData();
	unsigned char getReceiveDataLen();
	unsigned char clearReceiveData();
	unsigned char isResultOk(void);

};
#endif
