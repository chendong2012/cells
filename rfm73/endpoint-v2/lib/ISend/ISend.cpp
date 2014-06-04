#include <ISend.h>
#include <comm.h>
#include <user_activity.h>
/*
#define putstring(x) SerialPrint_P(PSTR(x))
void SerialPrint_P(PGM_P str) {
	  for (uint8_t c; (c = pgm_read_byte(str)); str++) Serial.write(c);
}
//putstring("This is a nice long string that takes no memory");
*/
extern user_activity *myu2;
/*
   建立状态机
   １、发送前
   ２、正在发送中
   ３、发送完毕
   4、对方确认
   */
/*
使用方法：
１、构造对像
	isend("dddd");
	trigerSend("dddd");
２、设置要发送的命令字符串
３、在远程返回结果的地方加入自己的代码

*/
ISend::ISend(const char *cmdstr)
{
	trys = 0;
	setStatus(S_I);
	setSendResult(RLT_INIT);
	clearAckData();
        setCmdStr(cmdstr);

	creat_send_thread();
}

void ISend::setCmdStr(const char *cmdstr)
{
	item_len = strlen((const char*)cmdstr);
	item_len += 1;
	strcpy((char *)&item[1], (const char*)cmdstr);
}

unsigned char ISend::setindex()
{
	index++;
	item[0] = index;
	return index;
}

void ISend::setStatus(unsigned char s)
{
	this->status = s;
}

unsigned char ISend::getStatus()
{
	return this->status;
}

unsigned char ISend::getSendResult()
{
	return send_result;
}

unsigned char ISend::setSendResult(unsigned char result)
{
	send_result = result;
}

unsigned char ISend::trigerSend(const char *s)
{
        if (getStatus() == S_I) {
		clearAckData();
                setCmdStr(s);
		setStatus(S_S);

                setindex();
                setSendResult(RLT_INIT);
	//	Serial.println(s);
                sendRfDatas();
                return 1;
        } else {
                return 0;
        }
}

void ISend::sendRfDatas()
{
	if (myu2 != NULL) {
		myu2->m_comm->send((const char *)item, item_len);
	} else 
		Serial.println("myu2 is null");
        return;
}

void ISend::disableSend()
{
//	cm->stop();
}

void ISend::msg_handler(unsigned char *dat, unsigned char len)
{
	unsigned char ret;
        ret = strncmp((char *)&item[1], (const char *)&dat[4], item_len-1);
	if (ret == 0)
		if (getStatus() == S_S) {
			setStatus(S_A);
			/*返回结果,打出来*/
			/*处理所有接收回来的应答码*/
			Serial.println("rec ack");
		}
}

/*70mS run a time*/
boolean ISend::send_cb(ISend *me)
{
		if (me->status == S_S) {
			me->trys++;
			me->sendRfDatas();
			if (me->trys >= MAX_TRY) {
				me->trys = 0;
				me->setStatus(S_I);
				me->disableSend();
				me->setSendResult(RLT_FAIL);
			}
		} else if (me->status == S_A) {
			me->setStatus(S_I);
			me->disableSend();
			me->trys = 0;
			me->setSendResult(RLT_OK);
		}
		return true;
}

unsigned char ISend::creat_send_thread()
{
	return 1;
}

unsigned char ISend::isResultOk(void)
{
	return getSendResult();
}

unsigned char ISend::clearAckData()
{
	memset(strAck, 0, PACKAGE_LEN);
	strAckLen = 0;
}
