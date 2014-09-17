#include <stdio.h>
#include <stdlib.h>
#include <iconv.h>
#include <string.h>
 
#define OUTLEN 1200
//#define DEBUG
//#define DISP_MODE1
#define DISP_MODE2
 
static char HZ_16x16[16][2];
int display(char *incode, int len);
int main(int argc, char **argv)
{
        char *string = argv[1];
        iconv_t cd;
        int inbuf_len = strlen(string);
        char outbuf[OUTLEN];
        char *pin = string;
        char *pout = &outbuf[0];  //用"pout=&outbuf" 会引发SIGSERV信号，导致段错误
        int outbuf_len = OUTLEN;
 
        memset(outbuf, 0, OUTLEN); //清空输出缓存，否则会有意外结果的

#ifdef DEBUG
        printf("Originial Data:\n"); //打印转换前的一些参数和信息，以进行比较
        printf("\tpin str: %s, outbuf str:%s\n", pin, outbuf);
        printf("\tinbuf_len=%d, outbuf_len=%d\n", inbuf_len, outbuf_len);
        printf("\tstrlen(outbuf)= %d\n", strlen(outbuf));
#endif
 
        cd = iconv_open("GB2312", "UTF8");
        if(cd == 0)
                return EXIT_FAILURE;
        int count = iconv(cd, &pin, &inbuf_len, &pout, &outbuf_len);
#ifdef DEBUG
        printf("iconv count : %d\n", count);//观察iconv返回值，理解不可逆转换含义
#endif
        iconv_close(cd);
 
#ifdef DEBUG
        printf("After Converted Data:\n"); //注意发生变化的变量
        printf("\tpin str: %s, gb2312 str:%s\n", pin, outbuf );
        printf("\tinbuf_len=%d, outbuf_len=%d\n", inbuf_len, outbuf_len);
        printf("\tstrlen(outbuf)= %d\n", strlen(outbuf));
#endif
 
        int i,j;
        for(i = 0; i < strlen(outbuf); i += 2)
        {
                display (outbuf+i, 2); //使用HZK16字库显示GB2312编码的中文点阵
        }
 
        return EXIT_SUCCESS;
}
 
void disp_mode1()
{
	unsigned char i,j,k;
        for(i = 0; i < 16; i++)  //显示点阵
        {
                for(j = 0; j < 2; j++)
                        for(k = 0; k < 8; k++)
                        {
                                if(HZ_16x16[i][j] & (0x80>>k))
                                        printf("#");
                                else
                                        printf("-");
                        }
                printf("\n");
        }
}

void disp_mode2()
{
	unsigned char i,j;
        for(i = 0; i < 16; i++) {
                for(j = 0; j < 2; j+=2)
                        printf("0x%02x,0x%02x", (unsigned char)HZ_16x16[i][j], (unsigned char)HZ_16x16[i][j+1]);
                printf("\n");
        }
}

int display(char *incode, int len)
{
        int i, j, k;
        FILE    *HZK=NULL;
        unsigned    char    qh,wh;
        unsigned    long    offset;
 
        qh=incode[0]-0xa0;//获得区码，中文编码从0xA1开始
        wh=incode[1]-0xa0;   //获得位码，中文编码从0xA1开始
        offset=(94*(qh-1)+(wh-1))*32; //得到汉字在HZK16字库中的字节偏移位置
#ifdef DEBUG
        printf("区码：%d,位码：%d\n",qh,wh);
#endif
 
        if((HZK=fopen("HZK16","rb")) ==NULL)
        {
           perror("Can't Open hzk16");
           return (EXIT_FAILURE);
        }
        fseek(HZK,offset,SEEK_SET);
        fread(HZ_16x16,32,1,HZK);//读取汉字的16*16点阵字模
        fclose(HZK);

#ifdef DISP_MODE1
	disp_mode1();
#endif
#ifdef DISP_MODE2
	disp_mode2();
#endif
 
        return EXIT_SUCCESS;
}

void hz_to_rgb(unsigned bg)
{
	unsigned char i,j;
	unsigned short temp;
	unsigned short r;
	unsigned short g;
	unsigned short b;
	r=0xffff;
	g=0xffff;
	b=0xffff;
	//12345678-12345678
	for(i=0;i<16;i++) {
		temp = (HZ_16x16[i]<<8)|HZ_16x16[i+1];
		r= temp;
		for(j=0;j<16;j++) {
			if(((temp<<j)&0x8000)==1) {
				r&=~(1<<(15-j)); //前景色
			} else {
				r|=(1<<(15-j));  //背景色
			}
		}
	}
}
