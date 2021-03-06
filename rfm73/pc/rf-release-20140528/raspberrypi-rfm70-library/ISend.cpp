#include <ISend.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "comm.h"

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
	_type = 0;
	setStatus(S_I);
	setSendResult(RLT_INIT);
	clearAckData();
	setCmdStr(cmdstr);
	_keywordlen = strlen(cmdstr);
	_cb = cb;
	creat_send_thread();
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

		if (_type == 1) {
			setStatus(S_I);
			setSendResult(RLT_OK);
		}
                sendRfDatas();
                return 1;
        } else {
                return 0;
        }
}

void ISend::sendRfDatas()
{
	_act->m_comm->send(item, item_len);
//	printf("ISend::sendRfDatas\n");
        return;
}

void ISend::disableSend()
{
//	cm->stop();
}

/*receive ack from remote:set finish flag*/
void ISend::onReceive(unsigned char *dat, unsigned char len)
{
	if (_cb != NULL)
		_cb(dat, len);
	return;
}

unsigned char *ISend::getKeyword(void)
{
        return (unsigned char *)&item[1];
}

unsigned char ISend::getKeywordLen(void)
{
        return _keywordlen;
}

unsigned char ISend::compare_keyword(unsigned char *dat)
{
        unsigned char ret;
        unsigned char *d1 = getKeyword();
        unsigned char *d2 = &dat[4];
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
//        ret = strncmp((char *)&item[1], (const char *)&dat[4], item_len-1);
	ret = compare_keyword(dat);
	if (ret == 0) {
		if (getStatus() == S_S) {
			setStatus(S_A);
			/*返回结果,打出来*/
			/*处理所有接收回来的应答码*/

			clearAckData();
			strncpy((char *)strAck, (const char *)&dat[4+item_len], len-4-item_len);
			strAckLen = len -4-item_len;

			onReceive(dat, len);
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
void *ISend::send_cb(void *ptr)
{
	ISend *me = (ISend *)ptr;
	for (;;) {
		/*查看对像是否是处于发送状态，如是，就会发送数据出去*/
		if (me->status == S_S) {
			sleep(1);
			me->trys++;
			printf("send try:%d\n", me->trys);
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
		} else {
			sleep(1);
		//	usleep(10000);
		}
	}
}

unsigned char ISend::creat_send_thread()
{
        int ret;
        pthread_t id;

        ret = pthread_create(&id, NULL,ISend::send_cb, this);
        if(ret) {
		printf("creat_send_thread error\n");
		return 1;
        }
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

//new interface
unsigned char *ISend::getAckData(void)
{
	return strAck;
}

unsigned char ISend::getAckDataLen(void)
{
	return strAckLen;
}
unsigned char *ISend::getItemData(void)
{
	return (unsigned char *)&item[1];
}

unsigned char ISend::getItemDataLen(void)
{
	return item_len - 1;
}

void ISend::setUserObj(user_activity *a)
{
        _act = a;
}

/*表示发送完一次后，就结束，不会再去等远程的返回信息*/
void ISend::setSendType(unsigned char type)
{
	_type = type;
}

