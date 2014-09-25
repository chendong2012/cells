#include "public.h"
#include "a.h"
#include <CallMe.h>
#include <Task.h>

/*有三大块显存：
 * 1\rgb_datas 32x16
 * 2\rgb_backup 32x16
 * 3\hz_mem     64x16
 * 4\hz
 * */

static void clear_framebuffer(void);

static void fill_8x16(unsigned char offset_8x16_fb, unsigned char count_8x16_zk);
static void fill_32x16(unsigned char offset_8x16_fb, unsigned char count_8x16_zk);
static void fill_16x16(unsigned char offset_8x16_fb, unsigned char count_8x16_zk);


static void fill_8x16_from_flash(unsigned char offset_8x16_fb, unsigned char offset_flash);

static void fb64x16_shift_left_abit(void);
inline void fb64x1_shift_left_abit(unsigned char *l, unsigned char *l2);

void read_flash_to_hz_mem(unsigned char begin,unsigned char count);
void fb_shift_init(void);
void fb_shift_loop(void);

struct _rgb_8points hz_mem[64];

static struct _rgb_line rgb_backup[H];
static struct _rgb_line rgb_datas[H];

static void hw_shift_1bits(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2);

static void hw_shift_8bits(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2);

void hw_select_row(void);
void hw_lock_data(void);
void init_gpio(void);



static void hw_shift_32bits(struct _rgb_line *rgb1,struct _rgb_line *rgb2);
static void update_32x16(void);
typedef struct {
	byte pin : 6;
	byte mode : 1;
	byte state : 1;
} DigitalPin;

static unsigned char flash_update=0;
static unsigned char disp_speed=0;
static unsigned char flash_offset=0;
static unsigned char flash_length=HZ_LEN; //how many 8x16
static boolean display(void)
{
	unsigned char j;	
	update_32x16();
	speed++;
	if(disp_speed%3==0) {
		disp_speed=0;
	} else {
		return 1;
	}

	fb64x16_shift_left_abit();
	update++;
	if (update%32==0) {
		update = 0;
		if(flash_offset>=flash_length) {
			flash_offset = 0;
		}
		for(j=4;j<8;j++)
			fill_8x16_from_flash(j, flash_offset++);
	}
}

CallMe disp_update(200, display);


void init_serial(void)
{
	Serial.begin(115200);
	Serial.print("begin!");
}

/*
 * begin 是以8x16为单位的，begin 是偏移值:0,1,2
 *0:表示从hz开始，１表示从8x16开始，类推
 *count 要读几块16*8数据
 * */
void read_flash_to_hz_mem(unsigned char begin,unsigned char count)
{
	unsigned char i;
	unsigned char offset = begin*16;

	for (i=0;i<count*16;i++) {
		hz_mem[i].r = pgm_read_byte(&hz[i+offset].r);
		hz_mem[i].g = pgm_read_byte(&hz[i+offset].g);
		hz_mem[i].b = pgm_read_byte(&hz[i+offset].b);
	}
}
void fb_shift_init(void)
{
	unsigned char i;
	unsigned char j;
#if 0
	read_flash_to_hz_mem(4,4);
	fill_32x16(0, 0);
#endif

#if 1
	flash_offset = 0;
	if(flash_length>=8) {
		for(j=0; j<8; j++)
			fill_8x16_from_flash(j, flash_offset++);
	} else {
		for(j=0; j<flash_length; j++)
			fill_8x16_from_flash(j, flash_offset++);
	}
#endif
}

void fb_shift_loop(void)
{
	unsigned char i=0;
	unsigned char j=0;

	for(i=0;i<W;i++) {

		update_32x16();
		update_32x16();
		update_32x16();

		fb64x16_shift_left_abit();
	}
#if 1
	if(flash_offset>=flash_length) {
		flash_offset = 0;
	}

	for(j=4;j<8;j++)
		fill_8x16_from_flash(j, flash_offset++);
#endif
}

void setup()
{
	init_serial();
	init_gpio();
	//clear_framebuffer();
	fb_shift_init();
	disp_update.start();
}
#if 0
void loop()
{

	//update_32x16();
	fb_shift_loop();
}
#endif
static void init_gpio(void)
{
#if 0
	pinMode(R, OUTPUT);
	pinMode(G, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(R2, OUTPUT);
	pinMode(G2, OUTPUT);
	pinMode(B2, OUTPUT);
#endif

	DDRD = B11111100;

	pinMode(SHCP, OUTPUT);
	pinMode(OE, OUTPUT);
	pinMode(LOCK, OUTPUT);
	pinMode(ROW0, OUTPUT);
	pinMode(ROW1, OUTPUT);
	pinMode(ROW2, OUTPUT);
	pinMode(16, OUTPUT);


}

static void hw_shift_1bits(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2)
{

#if 1
	unsigned char datas=0;
	digitalWrite(SHCP, LOW);
	datas|=((r1&0x80)>>5);
	datas|=((g1&0x80)>>4);
	datas|=((b1&0x80)>>3);
	datas|=((r2&0x80)>>2);
	datas|=((g2&0x80)>>1);
	datas|=((b2&0x80)>>0);
	
	PORTD=datas;

#endif
#if 0
	digitalWrite(R, r1>>7);
	digitalWrite(G, g1>>7);
	digitalWrite(B, b1>>7);

	digitalWrite(R2, r2>>7);
	digitalWrite(G2, g2>>7);
	digitalWrite(B2, b2>>7);
#endif
#if 0
	digitalWrite(16, r1>>7);
	digitalWrite(16, g1>>7);
	digitalWrite(16, b1>>7);


	digitalWrite(16, r2>>7);
	digitalWrite(16, g2>>7);
	digitalWrite(16, b2>>7);

	digitalWrite(R, r1&0x01);
	digitalWrite(G, g1&0x01);
	digitalWrite(B, b1&0x01);

	digitalWrite(R2, r2&0x01);
	digitalWrite(G2, g2&0x01);
	digitalWrite(B2, b2&0x01);
#endif

	digitalWrite(SHCP, HIGH);
}

static void hw_shift_8bits(unsigned char r1,\
		unsigned char g1,\
		unsigned char b1,\
		unsigned char r2,\
		unsigned char g2,\
		unsigned char b2)
{
	unsigned char i;

	for(i=0; i<8; i++) {
		hw_shift_1bits(r1,g1,b1,r2,g2,b2);
		r1<<=1;
		g1<<=1;
		b1<<=1;

		r2<<=1;
		g2<<=1;
		b2<<=1;
#if 0
		r1>>=1;
		g1>>=1;
		b1>>=1;

		r2>>=1;
		g2>>=1;
		b2>>=1;
#endif
	}
}

static void hw_select_row(unsigned char line)
{
	digitalWrite(ROW0, line&0x01);
	digitalWrite(ROW1, (line&0x02)>>1);
	digitalWrite(ROW2, (line&0x04)>>2);
}

static void hw_shift_32bits(struct _rgb_line *rgb1, struct _rgb_line *rgb2)
{
	unsigned char i;
	for(i=0;i<4;i++) {
		hw_shift_8bits(rgb1->r[i],\
				rgb1->g[i],\
				rgb1->b[i],\
				rgb2->r[i],\
				rgb2->g[i],\
				rgb2->b[i]);
	}
}

static void hw_lock_data(void)
{
	digitalWrite(LOCK, 1);
	digitalWrite(LOCK, 0);
}

static void output_data(void)
{
	digitalWrite(OE, 0);
	delayMicroseconds(500);
	digitalWrite(OE, 1);
	//	delayMicroseconds(500);
}

static struct _rgb_line rgb, rgb1;
static void update_32x16(void)
{
	unsigned char line;
	for(line=0; line<8; line++) {
		//memcpy(rgb.r, (const void *)rgb_datas[line].r, 4);
		//memcpy(rgb.g, (const void *)rgb_datas[line].g, 4);
		//memcpy(rgb.b, (const void *)rgb_datas[line].b, 4);

	//	memcpy(rgb1.r, (const void *)rgb_datas[line+8].r, 4);
	//	memcpy(rgb1.g, (const void *)rgb_datas[line+8].g, 4);
	//	memcpy(rgb1.b, (const void *)rgb_datas[line+8].b, 4);

		//hw_shift_32bits(&rgb,&rgb1);
		hw_shift_32bits(&rgb_datas[line],&rgb_datas[line+8]);

		hw_select_row(line);
		hw_lock_data();
		output_data();
	}
}

unsigned char byte_ofst(unsigned short pixel_index)
{
	return pixel_index/8;
}

static void setpixel(unsigned char x, unsigned char y, struct pixel p)
{
	unsigned char byteofst;
	unsigned char bitofst;
	unsigned char dat;

	if(x>(H-1) || y>(W-1)) return;
	byteofst = x/8;
	bitofst = (7-x%8);

	dat = rgb_datas[y].r[byteofst];
	if (p.r==0)
		dat &=~(0x01<<bitofst);
	else
		dat |=(0x01<<bitofst);

	dat = rgb_datas[y].g[byteofst];
	if (p.g==0)
		dat &=~(0x01<<bitofst);
	else
		dat |=(0x01<<bitofst);

	dat = rgb_datas[y].b[byteofst];
	if (p.b==0)
		dat &=~(0x01<<bitofst);
	else
		dat |=(0x01<<bitofst);
}

static void draw_a_color_screen(struct pixel p)
{
	unsigned char x=0;
	unsigned char y=0;
	for (y=0;y<H;y++)
	for (x=0;x<W;x++)
		setpixel(x, y, p);
}

static void clear_framebuffer(void)
{
	unsigned char i=0;
	for (i=0;i<H;i++) {
		memset(rgb_datas[i].r, 0xff, W/8);
		memset(rgb_datas[i].g, 0xff, W/8);
		memset(rgb_datas[i].b, 0, W/8);

		memset(rgb_backup[i].r,0, W/8);
		memset(rgb_backup[i].g, 0, W/8);
		memset(rgb_backup[i].b, 0, W/8);
	}
}

static void draw_a_line(unsigned char line, struct _rgb_line rgbline)
{
	memcpy(rgb_datas[line].r, rgbline.r, W/8);
	memcpy(rgb_datas[line].g, rgbline.g, W/8);
	memcpy(rgb_datas[line].b, rgbline.b, W/8);
}

/*一个字有32的偏移, 一个字占的字节数为32*3=96个字节*/

/***************************

offset_8x16_fb:只能取如下值：0和4
****************************/
static void fill_32x16(unsigned char offset_8x16_fb, unsigned char count_8x16_zk)
{

	if (offset_8x16_fb == 1) return;
	if (offset_8x16_fb == 2) return;
	if (offset_8x16_fb == 3) return;
	if (offset_8x16_fb == 5) return;
	if (offset_8x16_fb == 6) return;
	if (offset_8x16_fb == 7) return;

	fill_8x16(offset_8x16_fb, count_8x16_zk);
	fill_8x16(offset_8x16_fb+1, count_8x16_zk+1);
	fill_8x16(offset_8x16_fb+2, count_8x16_zk+2);
	fill_8x16(offset_8x16_fb+3, count_8x16_zk+3);
}

/***************************

offset_8x16_fb:只能取如下值：
0,1,2或是4,5,6
3和7是不能取的

****************************/
static void fill_16x16(unsigned char offset_8x16_fb, unsigned char count_8x16_zk)
{
	if ((offset_8x16_fb == 3)||(offset_8x16_fb == 7))
		return;
	fill_8x16(offset_8x16_fb, count_8x16_zk);
	fill_8x16(offset_8x16_fb+1, count_8x16_zk+1);
}

/*
offset_8x16_fb:from 0,1,2,...7
count_8x16_zk:from 0,1,2,...7
*/
static void fill_8x16(unsigned char offset_8x16_fb, unsigned char count_8x16_zk)
{
	unsigned char i,j;
	unsigned char offset = count_8x16_zk*16;
	struct _rgb_line *line;
	char buf[16];
	if (offset_8x16_fb < 4) {
		line = rgb_datas;
	} else {
		line = rgb_backup;
		offset_8x16_fb -= 4;
	}
	/*i表示第几行*/
	for(i=0; i<16; i++) {
		/*j表示第几个８列*/
		line[i].r[offset_8x16_fb] = hz_mem[offset+i].r;
		line[i].g[offset_8x16_fb] = hz_mem[offset+i].g;
		line[i].b[offset_8x16_fb] = hz_mem[offset+i].b;
	}
}

/*
 *offset_8x16_fb:0\2\3\4\5\6\7
 *offset_flash  :0\1\2\3\4\5\6\7\8\9\10\11\12\13\14\15....
 * */
static void fill_8x16_from_flash(unsigned char offset_8x16_fb, unsigned char offset_flash)
{
	unsigned char i,j;
	struct _rgb_line *line;
	unsigned char offset = offset_flash*16;
	/*
		0 1x48  2x48


	*/
	if (offset_8x16_fb < 4) {
		line = rgb_datas;
	} else {
		line = rgb_backup;
		offset_8x16_fb -= 4;
	}
	/*i表示第几行*/
	for(i=0; i<16; i++) {
		/*j表示第几个８列*/
		line[i].r[offset_8x16_fb] = pgm_read_byte(&hz[i+offset].r);
		line[i].g[offset_8x16_fb] = pgm_read_byte(&hz[i+offset].g);
		line[i].b[offset_8x16_fb] = pgm_read_byte(&hz[i+offset].b);
	}
}

void fb64x1_shift_left_abit(unsigned char *l, unsigned char *l2)
{

	unsigned long t;
	unsigned char *tp;
	char buf[64];

	tp = (unsigned char *)&t;

	//sprintf(buf, "0x%02x 0x%02x 0x%02x 0x%02x",l[0],l[1],l[2],l[3]);
	//Serial.println(buf);

//	t = (l[0]<<24)|(l[1]<<16)|(l[2]<<8)|l[3];
	tp[0] = l[3];
	tp[1] = l[2];
	tp[2] = l[1];
	tp[3] = l[0];

	//sprintf(buf, "0x%02x 0x%02x 0x%02x 0x%02x",tp[0],tp[1],tp[2],tp[3]);
	//Serial.println(buf);
	t<<=1;
	t|=(l2[0]>>7);

//	l[0] = (unsigned char)(t>>24);
//	l[1] = (unsigned char)(t>>16);
//	l[2] = (unsigned char)(t>>8);
//	l[3] = (unsigned char)t;

	l[0] = tp[3];
	l[1] = tp[2];
	l[2] = tp[1];
	l[3] = tp[0];

//	l[0] = (t&0xff000000)>>24;
//	l[1] = (t&0x00ff0000)>>16;
//	l[2] = (t&0x0000ff00)>>8;
//	l[1] = (unsigned char)(t>>16);
//	l[2] = (unsigned char)(t>>8);
//	l[3] = (unsigned char)t;





//	sprintf(buf,"0x%02x 0x%02x 0x%02x 0x%02x",l[0],l[1],l[2],l[3]);
//	Serial.println(buf);

//	t = (l2[0]<<24)|(l2[1]<<16)|(l2[2]<<8)|l2[3];

	tp[0] = l2[3];
	tp[1] = l2[2];
	tp[2] = l2[1];
	tp[3] = l2[0];

	t<<=1;

	l2[0] = tp[3];
	l2[1] = tp[2];
	l2[2] = tp[1];
	l2[3] = tp[0];

	//l2[0] = (unsigned char)(t>>24);
	//l2[1] = (unsigned char)(t>>16);
	//l2[2] = (unsigned char)(t>>8);
	//l2[3] = (unsigned char)t;
}
/*
 *参数包括有：
 *１、移动资源定位
 *２、装载数据时机
 *3、真正移位　
 * */

static void fb64x16_shift_left_abit(void)
{
#if 0
	/*
	 * 12345678-12345678-12345678-12345678-|-12345678-12345678-12345678-12345678
	 */
	unsigned char i;
	unsigned char *temp0;
	unsigned char *temp1;
	unsigned char *temp2;
	unsigned char *temp3;
	char buf1[16];
		
	for (i=0;i<H;i++) {//全部移完1次,什么时候要补充rgb_backup的数据呢，只能是以最小单行8x16,所以移完
		//rrrrrrrrrrrrr
		temp0 = (unsigned char *)&rgb_datas[i].r[0];
		temp1 = (unsigned char *)&rgb_datas[i].r[1];
		temp2 = (unsigned char *)&rgb_datas[i].r[2];
		temp3 = (unsigned char *)&rgb_datas[i].r[3];
		*temp0<<=1;
		*temp0|=*temp1>>7;

		*temp1<<=1;
		*temp1|=*temp2>>7;

		*temp2<<=1;
		*temp2|=*temp3>>7;

		*temp3<<=1; //fille lowest bit

		temp0 = (unsigned char *)&rgb_backup[i].r[0];
		temp1 = (unsigned char *)&rgb_backup[i].r[1];
		temp2 = (unsigned char *)&rgb_backup[i].r[2];
		temp3 = (unsigned char *)&rgb_backup[i].r[3];
		rgb_datas[i].r[3]|=*temp0>>7;

		*temp0<<=1;
		*temp0|=*temp1>>7;

		*temp1<<=1;
		*temp1|=*temp2>>7;

		*temp2<<=1;
		*temp2|=*temp3>>7;

		*temp3<<=1; //fille lowest bit

		//gggggggggg

		temp0 = (unsigned char *)&rgb_datas[i].g[0];
		temp1 = (unsigned char *)&rgb_datas[i].g[1];
		temp2 = (unsigned char *)&rgb_datas[i].g[2];
		temp3 = (unsigned char *)&rgb_datas[i].g[3];
		*temp0<<=1;
		*temp0|=*temp1>>7;

		*temp1<<=1;
		*temp1|=*temp2>>7;

		*temp2<<=1;
		*temp2|=*temp3>>7;

		*temp3<<=1; //fille lowest bit

		temp0 = (unsigned char *)&rgb_backup[i].g[0];
		temp1 = (unsigned char *)&rgb_backup[i].g[1];
		temp2 = (unsigned char *)&rgb_backup[i].g[2];
		temp3 = (unsigned char *)&rgb_backup[i].g[3];
		rgb_datas[i].g[3]|=*temp0>>7;

		*temp0<<=1;
		*temp0|=*temp1>>7;

		*temp1<<=1;
		*temp1|=*temp2>>7;

		*temp2<<=1;
		*temp2|=*temp3>>7;

		*temp3<<=1; //fille lowest bit
		////bbbbbbbbbbb

		temp0 = (unsigned char *)&rgb_datas[i].b[0];
		temp1 = (unsigned char *)&rgb_datas[i].b[1];
		temp2 = (unsigned char *)&rgb_datas[i].b[2];
		temp3 = (unsigned char *)&rgb_datas[i].b[3];
		*temp0<<=1;
		*temp0|=*temp1>>7;

		*temp1<<=1;
		*temp1|=*temp2>>7;

		*temp2<<=1;
		*temp2|=*temp3>>7;

		*temp3<<=1; //fille lowest bit

		temp0 = (unsigned char *)&rgb_backup[i].b[0];
		temp1 = (unsigned char *)&rgb_backup[i].b[1];
		temp2 = (unsigned char *)&rgb_backup[i].b[2];
		temp3 = (unsigned char *)&rgb_backup[i].b[3];
		rgb_datas[i].b[3]|=*temp0>>7;

		*temp0<<=1;
		*temp0|=*temp1>>7;

		*temp1<<=1;
		*temp1|=*temp2>>7;

		*temp2<<=1;
		*temp2|=*temp3>>7;

		*temp3<<=1; //fille lowest bit









	}
#endif



#if 1
	unsigned char i;
	for (i=0;i<H;i++) {
		fb64x1_shift_left_abit(rgb_datas[i].r, rgb_backup[i].r);
		fb64x1_shift_left_abit(rgb_datas[i].g, rgb_backup[i].g);
		fb64x1_shift_left_abit(rgb_datas[i].b, rgb_backup[i].b);
	}
#endif
}
