#include <ISendCustom.h>
#include <comm.h>

ISendCustom::ISendCustom(const char *cmdstr) : ISend(cmdstr)
{
	_act = NULL;
}

void ISendCustom::onReceive(unsigned char *dat, unsigned char len)
{
	return;
}

void ISendCustom::sendRfDatas()
{
	_act->m_comm->send(item, item_len);
}
void ISendCustom::setUserObj(user_activity *a)
{
        _act = a;
}

unsigned char ISendCustom::sendRF(unsigned char *s)
{
	unsigned char ret;
	ret = trigerSend(s);
	return ret;
}

