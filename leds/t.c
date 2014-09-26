#include <stdio.h>
#include <stdlib.h>
#include <iconv.h>
#include <string.h>
 
#define OUTLEN 1200
//#define DEBUG
#define DISP_MODE1
//#define DISP_MODE2
#define DISP_MODE3
 


struct _rgb_8points {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct _rgb_8points hz_8points[32];

static unsigned short _r[16];
static unsigned short _g[16];
static unsigned short _b[16];

void set_bg(unsigned char type);
void set_fg(unsigned char line, unsigned char type, unsigned short focus);
void hz_to_rgb(unsigned char fg, unsigned bg);
void print_result(void);
void print_result1(void);
void disp_mode1(void);
void disp_mode2(void);

void print_result_8x16(struct _rgb_8points *flash_addr);
void set_fg_8points(struct _rgb_8points *flash_addr, unsigned char fg, unsigned short focus);
void set_rgb_8points_value(struct _rgb_8points *rgb8points, unsigned char r, unsigned char g, unsigned b);
void set_bg_8points(struct _rgb_8points *flash_addr, unsigned char bg);
void hz_to_rgb_8x16(unsigned char *datas_8x16, struct _rgb_8points *flash_addr, unsigned char fg, unsigned bg);
void print_head(void);
void print_tail(void);
void hzk_map_array(unsigned char fg_l, unsigned char bg_l, unsigned char fg_r, unsigned char bg_r);

static unsigned char hz_left_8x16[16];
static unsigned char hz_right_8x16[16];
static unsigned char HZ_16x16[16][2];
int display(char *incode, int len);
int main(int argc, char **argv)
{
        char *string = argv[1];
        iconv_t cd;
        size_t inbuf_len = strlen(string);
        char outbuf[OUTLEN];
        char *pin = string;
        char *pout = &outbuf[0];  //用"pout=&outbuf" 会引发SIGSERV信号，导致段错误
        size_t outbuf_len = OUTLEN;
 
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

	printf("#define HZ_LEN	%d\n", strlen(outbuf));
	print_head();
        for(i = 0; i < strlen(outbuf); i += 2)
        {
                display(outbuf+i, 2); //使用HZK16字库显示GB2312编码的中文点阵
        }
	print_tail();
 
        return EXIT_SUCCESS;
}

void disp_mode0(void)
{
	unsigned char i,j,k;
	/*打出第一个8x16*/
		printf("{");
	for(i = 0; i < 15; i++) {
		printf("0x%02x,",HZ_16x16[i][0]);
	}
	printf("0x%02x",HZ_16x16[i][0]);
	printf("},\n");

	/*打出第二个8x16*/
	printf("{");
	for(i = 0; i < 15; i++) {
		printf("0x%02x,",HZ_16x16[i][1]);
	}
	printf("0x%02x",HZ_16x16[i][1]);
	printf("},\n");
}

void disp_mode1(void)
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

void disp_mode2(void)
{
	hz_to_rgb(4, 7);
	/*
	unsigned char i,j;
        for(i = 0; i < 16; i++) {
                for(j = 0; j < 2; j+=2)
                        printf("0x%02x,0x%02x", (unsigned char)HZ_16x16[i][j], (unsigned char)HZ_16x16[i][j+1]);
                printf("\n");
        }
	*/
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
//	disp_mode0();
#ifdef DISP_MODE1
	printf("/*\n");
	disp_mode1();
	printf("*/\n");
#endif
#ifdef DISP_MODE2
	disp_mode2();
#endif

#ifdef DISP_MODE3
	hzk_map_array(4, 7, 4, 7);
#endif

        return EXIT_SUCCESS;
}

void hz_div_8x16_array()
{
	unsigned char i;
	for(i=0;i<16;i++) {
		hz_left_8x16[i] = HZ_16x16[i][0];
		hz_right_8x16[i] = HZ_16x16[i][1];
	}
}

void hz_to_rgb_8x16(unsigned char *datas_8x16, struct _rgb_8points *flash_addr, unsigned char fg, unsigned bg)
{
	unsigned char i,j;
	unsigned char temp=0;
	for(i=0;i<16;i++) {
		set_bg_8points(&flash_addr[i], bg);
	}

	for(i=0;i<16;i++) {
		temp=0;
		temp = datas_8x16[i];
		for(j=0;j<8;j++) {
			if(((temp<<j)&0x80)==0x80)
				set_fg_8points(&flash_addr[i], fg, (7-j));
		}
	}

	print_result_8x16(flash_addr);
//	print_result_8x16(&hz_8points[16]);

//	print_result();
//	print_result1();
}

void hzk_map_array(unsigned char fg_l, unsigned char bg_l, unsigned char fg_r, unsigned char bg_r)
{
	hz_div_8x16_array();
	hz_to_rgb_8x16(hz_left_8x16, &hz_8points[0], fg_l, bg_l);
	hz_to_rgb_8x16(hz_right_8x16, &hz_8points[16], fg_r, bg_r);
}



void hz_to_rgb(unsigned char fg, unsigned bg)
{
	unsigned char i,j;
	unsigned short temp=0;
	unsigned short temp1=0;
	set_bg(bg);
	for(i=0;i<16;i++) {
		temp=0;
		temp = HZ_16x16[i][0]; //([0]=0x12, [1]=0x34) ==> 0x1234 
		temp<<=8;
		
		temp1 = (unsigned short)HZ_16x16[i][1]; 
		temp1=temp1&0x00ff;
		temp|=temp1;
		for(j=0;j<16;j++) {
			if(((temp<<j)&0x8000)==0x8000)
				set_fg(i, fg, (15-j));
		}
	}

//	print_result();
	print_result1();
}

void print_result(void)
{
	unsigned char i,j;
        printf("static struct _rgb_line rgb_datas[H] = {\n");
	for(i=0;i<16;i++) {
		printf("{0x%02x,0x%02x,0x%02x,0x%02x,\n",_r[i]>>8,_r[i]&0x00ff, _r[i]>>8,_r[i]&0x00ff);
		printf("0x%02x,0x%02x,0x%02x,0x%02x,\n",_g[i]>>8,_g[i]&0x00ff, _g[i]>>8,_g[i]&0x00ff);
		printf("0x%02x,0x%02x,0x%02x,0x%02x},\n",_b[i]>>8,_b[i]&0x00ff, _b[i]>>8,_b[i]&0x00ff);
	}
        printf("};\n");
}

void print_result1(void)
{
	unsigned char i,j;
	/*第一个8x16*/
     //   printf("static struct _rgb_half_line rgb_datas[H] = {\n");
	for(i=0;i<16;i++) {
		printf("{0x%02x,0x%02x,0x%02x},\n",_r[i]>>8, _g[i]>>8, _b[i]>>8);
	}

	/*第二个8x16*/
	for(i=0;i<16;i++) {
		printf("{0x%02x,0x%02x,0x%02x},\n", _r[i]&0x00ff, _g[i]&0x00ff, _b[i]&0x00ff);
	}
      //  printf("};\n");
}


void print_head(void)
{

        printf("const struct _rgb_8points PROGMEM hz[]={/*8x16*/\n");
}

void print_tail(void)
{
	printf("};\n");
}


void print_result_8x16(struct _rgb_8points *flash_addr)
{
	unsigned char i,j;
	for(i=0;i<16;i++) {
		printf("{0x%02x,0x%02x,0x%02x},\n",flash_addr[i].r, flash_addr[i].g, flash_addr[i].b);
	}
}

void set_fg_8points(struct _rgb_8points *flash_addr, unsigned char fg, unsigned short focus)
{
	switch (fg) {
	case 0://000
		flash_addr->r&=~(1<<focus);
		flash_addr->g&=~(1<<focus);
		flash_addr->b&=~(1<<focus);
		break;
	case 1://001

		flash_addr->r&=~(1<<focus);
		flash_addr->g&=~(1<<focus);
		flash_addr->b|=(1<<focus);
		break;
	case 2://010
		flash_addr->r&=~(1<<focus);
		flash_addr->g|=(1<<focus);
		flash_addr->b&=~(1<<focus);
		break;
	case 3://011
		flash_addr->r&=~(1<<focus);
		flash_addr->g|=(1<<focus);
		flash_addr->b|=(1<<focus);
		break;
	case 4://100
		flash_addr->r|=(1<<focus);
		flash_addr->g&=~(1<<focus);
		flash_addr->b&=~(1<<focus);
		break;
	case 5://101
		flash_addr->r|=(1<<focus);
		flash_addr->g&=~(1<<focus);
		flash_addr->b|=(1<<focus);
		break;
	case 6://110
		flash_addr->r|=(1<<focus);
		flash_addr->g|=(1<<focus);
		flash_addr->b&=~(1<<focus);
		break;
	case 7://111
		flash_addr->r|=(1<<focus);
		flash_addr->g|=(1<<focus);
		flash_addr->b|=(1<<focus);
		break;
	}
}

void set_fg(unsigned char line, unsigned char type, unsigned short focus)
{
	switch (type) {
	case 0://000
		_r[line]&=~(1<<focus);
		_g[line]&=~(1<<focus);
		_b[line]&=~(1<<focus);
		break;
	case 1://001
		_r[line]&=~(1<<focus);
		_g[line]&=~(1<<focus);
		_b[line]|=(1<<focus);
		break;
	case 2://010
		_r[line]&=~(1<<focus);
		_g[line]|=(1<<focus);
		_b[line]&=~(1<<focus);
		break;
	case 3://011
		_r[line]&=~(1<<focus);
		_g[line]|=(1<<focus);
		_b[line]|=(1<<focus);
		break;
	case 4://100
		_r[line]|=(1<<focus);
		_g[line]&=~(1<<focus);
		_b[line]&=~(1<<focus);
		break;
	case 5://101
		_r[line]|=(1<<focus);
		_g[line]&=~(1<<focus);
		_b[line]|=(1<<focus);
		break;
	case 6://110
		_r[line]|=(1<<focus);
		_g[line]|=(1<<focus);
		_b[line]&=~(1<<focus);
		break;
	case 7://111
		_r[line]|=(1<<focus);
		_g[line]|=(1<<focus);
		_b[line]|=(1<<focus);
		break;
	}
}

void set_rgb_8points_value(struct _rgb_8points *rgb8points, unsigned char r, unsigned char g, unsigned b)
{
	rgb8points->r=r;
	rgb8points->g=g;
	rgb8points->b=b;
}

void set_bg_8points(struct _rgb_8points *flash_addr, unsigned char bg)
{
		switch (bg) {
		case 0://000
			set_rgb_8points_value(flash_addr, 0x00, 0x00, 0x00);
			break;
		case 1://001
			set_rgb_8points_value(flash_addr, 0x00, 0x00, 0xff);
			break;
		case 2://010
			set_rgb_8points_value(flash_addr, 0x00, 0xff, 0x00);
			break;
		case 3://011
			set_rgb_8points_value(flash_addr, 0x00, 0xff, 0xff);
			break;
		case 4://100
			set_rgb_8points_value(flash_addr, 0xff, 0x00, 0x00);
			break;
		case 5://101
			set_rgb_8points_value(flash_addr, 0xff, 0x00, 0xff);
			break;
		case 6://110
			set_rgb_8points_value(flash_addr, 0xff, 0xff, 0x00);
			break;
		case 7://111
			set_rgb_8points_value(flash_addr, 0xff, 0xff, 0xff);
			break;
		default:
			break;
		}
}

void set_bg(unsigned char type)
{
	unsigned char line;
	for(line=0;line<16;line++) {
		switch (type) {
		case 0://000
			_r[line]=0x0000; _g[line]=0x0000; _b[line]=0x0000;
			break;
		case 1://001
			_r[line]=0x0000; _g[line]=0x0000; _b[line]=0xffff;
			break;
		case 2://010
			_r[line]=0x0000; _g[line]=0xffff; _b[line]=0x0000;
			break;
		case 3://011
			_r[line]=0x0000; _g[line]=0xffff; _b[line]=0xffff;
			break;
		case 4://100
			_r[line]=0xffff; _g[line]=0x0000; _b[line]=0x0000;
			break;
		case 5://101
			_r[line]=0xffff; _g[line]=0x0000; _b[line]=0xffff;
			break;
		case 6://110
			_r[line]=0xffff; _g[line]=0xffff; _b[line]=0x0000;
			break;
		case 7://111
			_r[line]=0xffff; _g[line]=0xffff; _b[line]=0xffff;
			break;
		default:
			break;
		}
	}
}
