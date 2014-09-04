#include <ISend.h>
#include <comm.h>
#include <user_activity.h>
#include "public.h"
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
如何使用此对像：
１、构造对像
	isend("dddd");
2、发送数据：
	trigerSend("dddd"); //这个dddd表示要发送出去的完整内容。注意不是关键字
	如果不管结果的话，到此就ＯＫ了
３、如果要看最后发送的情况，就要设置一个for语句：
*step 1
       psender->trigerSend(dat);
       for(;;) {
*step 2
               ret = psender->isResultOk();
               if (ret == 1) {
                       printf("send datas ok:");
*step 3
                        break;
                } else if (ret == 2) {
                        printf("send datas fail\n");
                        break;
                } else {
                        usleep(100000);
                }
       }

２、设置要发送的命令字符串
３、在远程返回结果的地方加入自己的代码

*/
ISend::ISend(const char *cmdstr, void (*cb)(unsigned char *dat, unsigned char len))
{
	trys = 0;
	setStatus(S_I);
	setSendResult(RLT_INIT);
	clearAckData();
   setCmdStr(cmdstr);
	creat_send_thread();
	_cb = cb;
}

/*
 *把要发送的字符串关键字，存到item数组里
 *item[0],存放的是发送重试次数,也是包的识别码 
 *item[1]后，存的是具体要发送出去的数据了。
 * */
void ISend::setCmdStr(const char *cmdstr)
{
	memset(item, 0x00, PACKAGE_LEN);
	item_len = strlen((const char*)cmdstr);
	item_len += 1;
	strcpy((char *)&item[1], (const char*)cmdstr);
}

/*每次trigerSend 一次
*index 就会加1,每个index 对应一次发送，
*因为每次发送，要想到多发几次才会成功的情况，
*但是index是不变的。
*index的对应一次完整发送，有可能失败或是成功
* */
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

unsigned char ISend::compare_keyword(unsigned char *dat)
{
	unsigned char ret;
	unsigned char *d1 = getKeyword();
	unsigned char *d2 = Package::get_pkg_datas(INDEX_NONE, dat);
	unsigned char len = getKeywordLen();
	
        ret = strncmp((const char *)d1, (const char *)d2, len);
	if (ret == 0) 
		return 0;
	else 
		return 1;
}

void ISend::msg_handler(unsigned char *dat, unsigned char len)
{
	unsigned char ret;
	unsigned char l;
	unsigned char *p;

	ret = compare_keyword(dat);
	if (ret == 0) {
		if (getStatus() == S_S) {
			setStatus(S_A);
			clearAckData();

			l = Package::get_user_datas_len(INDEX_NONE, len, getKeywordLen());
			p = Package::get_user_datas(INDEX_NONE, dat, getKeywordLen());

			storeAckData(p, l);
			if (_cb != NULL)
				_cb(dat, len);
		}
	}
}

/*这是一个核心函数
 *在全局初始化后，就会一直每隔一定时间运行一次
 *当对像处于发送状态后，定时检查，是否还处于发送
 *状态，如是的话，就再次发送：
 这样做的目的，多次发送，如果收不到远程的应答，就会再次
 发送，如果重复次数多于MAX_TRY就会把状态恢复为S_I,
 变为没有初始状态了
 * 70mS run a time*/
boolean ISend::send_cb(ISend *me)
{
	/*查看对像是否是处于发送状态，如是，就会发送数据出去*/
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

unsigned char ISend::storeAckData(unsigned char *dat, unsigned char len)
{
	memcpy(strAck, dat, len);
	strAckLen = len;
}

//new interface
unsigned char *ISend::getAckData(void)
{
	return strAck;
}

unsigned char ISend::getAckDataLen(void)
{
	return strAckLen;
}

unsigned char *ISend::getKeyword(void)
{
	return (unsigned char *)&item[1];
}

unsigned char ISend::getKeywordLen(void)
{
	return item_len - 1;
}

unsigned char ISend::getItemLen(void)
{
	return item_len;
}
