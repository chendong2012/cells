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
	printf("senddatas item,%d,%c%c%c%c,len: %d\n",\
		item[0], item[1], item[2], item[3],item[4],item_len);
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
