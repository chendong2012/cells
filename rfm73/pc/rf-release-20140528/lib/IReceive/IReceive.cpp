#include <IReceive.h>

IReceive::IReceive(const char *cmdstr)
{
	strcpy((char *)item, (const char*)cmdstr);
}

void IReceive::setCmdStr(unsigned char *cmdstr)
{
	strcpy((char *)item, (const char*)cmdstr);
}

void IReceive::onReceive()
{
	return;
}

/*外部调用*/
void IReceive::msg_handler(unsigned char *dat, unsigned char len)
{
	unsigned char ret;
        ret = strncmp(item, (const char *)&dat[4], strlen(item));
	if (ret == 0) {
			if (isNewPackage(dat[5]))
				onReceive();
			/*send ack datas*/
	}
}

boolean IReceive::isNewPackage(unsigned char dat)
{
	if (index == dat) {
		return false;
	} else {
		return true;
	}
}
