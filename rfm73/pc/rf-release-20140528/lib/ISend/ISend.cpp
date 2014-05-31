#include <ISend.h>
/*
   建立状态机
   １、发送前
   ２、正在发送中
   ３、发送完毕
   4、对方确认
   */
ISend::ISend(const char *cmdstr)
{
	item_len = strlen(cmdstr);
	item_len+=1;
	strcpy((char *)&item[1], (const char*)cmdstr);
	trys = 0;
}

void ISend::setCmdStr(unsigned char *cmdstr)
{
	item_len = strlen((const char*)cmdstr);
	item_len += 1;
	strcpy((char *)&item[1], (const char*)cmdstr);
}

unsigned char ISend::setindex()
{
	index++;
	item[1] = index;
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

void ISend::trigerSend(void)
{

        status = S_S;
	setindex();
        sendRfDatas();
        *cm = CallMe(70, &(ISend::send_cb), this);
}

void ISend::sendRfDatas()
{
        return;
}

void ISend::disableSend()
{
	cm->stop();
}

/*receive ack from remote:set finish flag*/
void ISend::onReceive(unsigned char *dat, unsigned char len)
{
	return;
}

void ISend::msg_handler(unsigned char *dat, unsigned char len)
{
	unsigned char ret;
        ret = strncmp(item, (const char *)&dat[4], strlen(item));
	if (ret == 0)
		if (getStatus() == S_S) {
			setStatus(S_A);
			onReceive(dat, len);
		}
}

boolean ISend::send_cb(ISend *me)
{
	if (me->status == S_S) {
		me->trys++;
		me->sendRfDatas();
		if (me->trys >= MAX_TRY) {
			me->trys = 0;
			me->setStatus(S_I);
			me->disableSend();
		}
	} else if (me->status == S_A){
		me->setStatus(S_I);
		me->disableSend();
		me->trys = 0;
	}
	return true;
}
