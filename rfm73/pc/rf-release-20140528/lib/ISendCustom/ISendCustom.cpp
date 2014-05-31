#include <ISendCustom.h>
#include <comm.h>

ISendCustom::ISendCustom(const char *cmdstr, user_activity *act) : ISend(cmdstr)
{
	_act = act;
}

void ISendCustom::onReceive(unsigned char *dat, unsigned char len)
{
	return;
}

void ISendCustom::sendRfDatas()
{
	_act->m_comm->send(item, item_len);
}
