#include <stdio.h> 
#include <string.h>  
#include <iconv.h>
#include <stdlib.h>

//代码转换:从一种编码转为另一种编码 
int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
        iconv_t cd;
        int rc;
        char **pin = &inbuf;
        char **pout = &outbuf;

        cd = iconv_open(to_charset,from_charset);
        if (cd==0) return -1;
        memset(outbuf,0,outlen);
        if (iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
        iconv_close(cd);
        return 0;
}
//UNICODE码转为GB2312码 
int u2g(char *inbuf,int inlen,char *outbuf,int outlen)
{
        return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}
//GB2312码转为UNICODE码 
int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
        return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}


int main() 
{	
	unsigned char incode[3]="东"; // 要读出的汉字，GB编码  
	int offset;
	printf("%x	%x	%x	%x\n",incode[0],incode[1],incode[2],incode[3]);
	printf("%d	%d	%d	%d\n",incode[0],incode[1],incode[2],incode[3]);
	unsigned char qh = 0, wh = 0;  
	char mat[16][2] = {0};  
	FILE *HZK = 0;  
	int i,j,k;  
	char *utf="车";
	printf("%d	%d	%d\n",*utf,*(utf+1),*(utf+2));
	char gb[20];
	printf("size of utf:%d\n",strlen(utf));
	u2g(utf,strlen(utf),gb,20);

	//每个汉字，占两个字节, 取其区位号  
	//qh = 0xb7- 0xa0;	//获得区码  
	//wh = 0xbf- 0xa0;	//获得位码  
	wh = incode[0] - 0xa0;	//获得区码  
	qh = incode[1] - 0xa0;	//获得位码  
	printf("%x	%x\n",qh,wh);
	offset = (94*(qh-1)+(wh-1))*32; //得到偏移位置 
	if((HZK=fopen("HZK16", "rb")) == NULL)  
	{  
		printf("Can't Open HZK16\n");  
		getchar(); 
		return 0; 
	}  
	fseek(HZK, offset, SEEK_SET);  
	fread(mat, 32, 1, HZK); 
	fclose(HZK); 

	//显示 
	for(i=0; i<16; i++) 
	{	
		for(j=0; j<2; j++) 
		{ 
			for(k=0; k<8; k++) 
			{ 
				if(mat[i][j] & (0x80>>k))  
				{//测试为1的位则显示 
					printf("%c",'#'); 
				} 
				else 
				{ 
					printf("%c",'-'); 
				}	
			}	
		} 
		printf("\n"); 
	} 
	getchar(); 
	return 1; 
}
