#include <IReceiveGS.h>
#include <comm.h>

IReceiveGS::IReceiveGS(const char *cmdstr) : IReceive(cmdstr)
{
}

void IReceiveGS::setUserObj(user_activity *a)
{
	_a = a;
}

/*接收到的数据，作进一步处理函数*/
void IReceiveGS::onReceive(unsigned char *dat, unsigned char len)
{
	if (isNewPackage(dat)) {
		Serial.println("new package");
		saveAckBuf((unsigned char *)"getstatus:ok", strlen("getstatus:ok"));
		_a->m_comm->send("getstatus:ok", 12);
	} else {
		Serial.println("old package");
		_a->m_comm->send((const char *)getAckBuf(), getAckBufLen());
	}
}
