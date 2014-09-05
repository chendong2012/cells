#include <IReceive.h>
#include <comm.h>
#include "public.h"

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
	initAckBuf();
	_cb = cb;
}

void IReceive::setCmdStr(const char *cmdstr)
{
	index = 0;
	item = cmdstr;
	initAckBuf();
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
 * 
 * */
unsigned char IReceive::is_contain_userdata(unsigned char *dat, unsigned char len, unsigned char *var)
{

	unsigned char *usr_datas;
	unsigned char usr_len;
	unsigned char var_len;
	usr_datas = Package::get_user_datas(INDEX_EXIST, dat, getKeywordLen());
	usr_len = Package::get_user_datas_len(INDEX_EXIST, len, getKeywordLen());
	var_len = strlen((const char *)var);
	if (var_len > usr_len) return 0;

	if(memcmp((const void *)var, (const void *)usr_datas, var_len) == 0)
		return 1; 
	return 0;
}

/*remote sendor:[1][81][3][90][i][keyword][userdata]
 *local receive:[1][81][3][90][i][keyword][userdata]
 *
 * */
unsigned char *IReceive::getKeyword(void)
{
	return (unsigned char *)item;
}

unsigned char IReceive::getKeywordLen(void)
{
	return strlen(item);
}

unsigned char IReceive::compare_keyword(unsigned char *dat)
{
	unsigned char ret;
	unsigned char *d1 = getKeyword();
	unsigned char *d2 = Package::get_pkg_datas(INDEX_EXIST, dat);
	unsigned char len = getKeywordLen();
	
        ret = strncmp((const char *)d1, (const char *)d2, len);
	if (ret == 0) 
		return 0;
	return 1;
}

void IReceive::msg_handler(unsigned char *dat, unsigned char len)
{
	unsigned char ret;
	ret = compare_keyword(dat);
	if (ret == 0) {
		if (_cb != NULL) {
			_cb(dat, len);
		}
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

void IReceive::initAckBuf()
{
	memset(ack_buf, 0, PACKAGE_LEN);
	memcpy(ack_buf, item, getKeywordLen());
	ack_len = getKeywordLen();
}

void IReceive::saveUserBufToAckBuf(unsigned char *buf, unsigned  char len)
{
	unsigned char l;
	unsigned char user_offset = getKeywordLen(); 
	l = PACKAGE_LEN - user_offset;
	memset((void *)&ack_buf[user_offset], 0x00, l);
	memcpy((void *)&ack_buf[user_offset], (const void *)buf, len);
	ack_len = user_offset + len;
}

void IReceive::saveAckBuf(unsigned char *buf, unsigned  char len)
{
	memset(ack_buf, 0, PACKAGE_LEN);
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
