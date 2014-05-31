#include <IReceive.h>

IReceive::IReceive(const char *cmdstr,void (*cb)())
{
	index = 0;
	_callback = cb;
	item = cmdstr;
	clearAckBuf();
}

void IReceive::setCmdStr(const char *cmdstr)
{
	index = 0;
	item = cmdstr;
	clearAckBuf();
}

void IReceive::msg_handler(unsigned char *dat, unsigned char len)
{
	unsigned char ret;
        ret = strncmp(item, (const char *)&dat[5], strlen(item));
	if (ret == 0) {
		_callback();
	}
}

boolean IReceive::isNewPackage(unsigned char *dat)
{
	if (index == dat[4]) {
		return false;
	} else {
		index = dat[4];
		return true;
	}
}

void IReceive::saveAckBuf(unsigned char *buf, unsigned  char len)
{
	memcpy(ack_buf, buf, len);
}

unsigned char *IReceive::getAckBuf()
{
	return ack_buf;
}

unsigned char IReceive::getAckBufLen()
{
	return ack_len;
}

void IReceive::clearAckBuf()
{
	memset(ack_buf,0, 28);
	ack_len = 0;
}
