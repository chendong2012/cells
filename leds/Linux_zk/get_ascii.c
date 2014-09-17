#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
	FILE * fp;
	unsigned char i = 0;

	fp = fopen("ascii.txt","w");
	if(fp == 0)
	{
		perror("open");
		return -1;
	}
	for(i=0x20;i<0x7F;i++)
	{
		fputc(i,fp);
	}

	return 0;
}
