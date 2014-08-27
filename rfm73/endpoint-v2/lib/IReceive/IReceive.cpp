#include <IReceive.h>
#include <comm.h>

/*
 *针对item的组成
 *１、关键字，用于匹配（比如led）
 *２、当接收回来的字符串:
 (本地地址：２个字节）－（远程地址：２个字节）－（包索引号１个字节）－（匹配字最多１６个字节）－（变量n个字节）)
 ，首先要看看收到字符串是否有led,然后
 接收回来的字符串led + 变量部分
 变量部分：on:表示让led打开
 	　off:表示让led关闭
 * */
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

unsigned char IReceive::getCmdLen(void)
{
	return strlen(item);
}

/*
 *dat表示所有从远程接收回来字节的集合
 *len表示上面字节长度
 *var表示动作字符串
 *返回的结果：如果为１表示，接收回来的动作和指定的动作是一样的
 * */
unsigned char IReceive::cmpAction(unsigned char *dat, unsigned char len, unsigned char *var)
{
	unsigned char varlen = len - 5 - getCmdLen();
	if(memcmp((const void *)var, (const void *)&dat[5+getCmdLen()], varlen) == 0) {
		return 1; 
	}
	return 0;
}

void IReceive::msg_handler(unsigned char *dat, unsigned char len)
{
	unsigned char ret;
        ret = strncmp(item, (const char *)&dat[5], strlen(item));
	if (ret == 0) {
		if (_cb != NULL) {
			_cb(dat, len);
		}
	} else {
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
