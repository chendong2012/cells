#include <IReceive.h>
#include <string.h>

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

/*只是比较接收部分所指定的字符串,收到的数据可能比这个多，
 * 那么多的部分就是变量了
 * 比如:
 *ledon
 * led 是固定部分
 * on　就是变量
 * 现在比较的只是led,
 * 就是看主动发送从第６个字节开始是否有led字样,
 * 如有表示是符合条件的*/
void IReceive::msg_handler(unsigned char *dat, unsigned char len)
{
	unsigned char ret;
        ret = strncmp(item, (const char *)&dat[5], strlen(item));
	if (ret == 0) {
		if (_cb != NULL) {
			_cb(dat, len);
			printf("ack request info");
		}
	}
}

bool IReceive::isNewPackage(unsigned char *dat)
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
