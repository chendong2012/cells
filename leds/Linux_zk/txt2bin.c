/*
 * txt2bin.cpp
 *
 *  Created on: Oct 20, 2012
 *      Author: limosky
 */

#include <stdio.h>
#include <stdlib.h>
/*
 *将一个ascii字符转换为十六进制数
 */
unsigned char c2x(char ch)
{
	unsigned char temp=0;
	if(ch>=0x30 && ch<=0x39)
		temp = ch-0x30;
	else if(ch>=0x41 && ch<=0x46)
		temp = 0x0a+(ch-0x41);
	else if(ch>=0x61 && ch<=0x66)
		temp = 0x0a+(ch-0x61);
	else
		temp =0xff;
	return temp;
}
//将ascii_zk.txt转换为二进制文件
int main(void)
{
	char buffer[5];
	unsigned char ch=0;

	int i=0;

	FILE *frp=0;
	FILE *fwp=0;

	for(i=0; i<5; i++)
		buffer[i] = 0;

	frp=fopen("ascii_zk.txt","r");	//字模提取软件得到的文本文件
	fwp=fopen("ascii_zk","w");

	while(buffer[4] != 0x2e) //全部数据以句号结尾
	{
		for(i=0; i<5; i++)
			buffer[i]=fgetc(frp);
		ch = c2x(buffer[2]);
		ch = ch*16;
		ch = ch+c2x(buffer[3]);

		fputc(ch,fwp);

	}

	fclose(frp);
	fclose(fwp);

	return 0;
}
