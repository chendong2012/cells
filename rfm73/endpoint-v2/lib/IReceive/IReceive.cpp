#include <IReceive.h>
#include <comm.h>

IReceive::IReceive(const char *cmdstr, void (*cb)(unsigned char *dat, unsigned char len))
{
	index = 0;
	item = cmdstr;
	clearAckBuf();
	_cb = cb;
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
		if (_cb != NULL)
			_cb(dat, len);
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

void IReceive::clearAckBuf()
{
	memset(ack_buf,0, 32);
	ack_len = 0;
}

void IReceive::saveAckBuf(unsigned char *buf, unsigned  char len)
{
	memcpy(ack_buf, buf, len);
	ack_len = len;
}

unsigned char *IReceive::getAckBuf()
{
	return ack_buf;
}

unsigned char IReceive::getAckBufLen()
{
	return ack_len;
}
