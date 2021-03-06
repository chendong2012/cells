#include<stdio.h>
#include<stdlib.h>
typedef struct {
	unsigned char red,green,blue;
} PPMPixel;

typedef struct {
	int x, y;
	PPMPixel *data;
} PPMImage;

#define CREATOR "RPFELGUEIRAS"
#define RGB_COMPONENT_COLOR 255

//#define PRINT_RAW_DATAS
/*
   P3
# The P3 means colors are in ASCII, then 3 columns and 2 rows,
# then 255 for max color, then RGB triplets
# rgb数据从上到下，从左到右排放。
3 2
255
255   0   0     0 255   0     0   0 255
255 255   0   255 255 255     0   0   0
 *
 *
 * */
#define H	16
#define W	1024 //最大支持1024宽个像素，高为16像素

static unsigned char r_datas[H*W/8];
static unsigned char g_datas[H*W/8];
static unsigned char b_datas[H*W/8];

void printbyline(PPMImage *img)
{
	int i=0;

	printf("static struct _rgb_line rgb_datas[H] = {\n");
	for (i=0;i<img->x*img->y/8;i+=4) {
		printf("{0x%02x,0x%02x,0x%02x,0x%02x,\n",r_datas[i],r_datas[i+1],r_datas[i+2],r_datas[i+3]);
		printf("0x%02x,0x%02x,0x%02x,0x%02x,\n",g_datas[i],g_datas[i+1],g_datas[i+2],g_datas[i+3]);
		printf("0x%02x,0x%02x,0x%02x,0x%02x},",b_datas[i],b_datas[i+1],b_datas[i+2],b_datas[i+3]);
		printf("\n");
	}
	printf("};\n");
}

unsigned long count_8pixels_a_linecolor(unsigned long x)
{
	return x/8;
}

unsigned long total_bytes_a_color(unsigned long x, unsigned long y)
{
	return (x*y)/8;
}

/*from left to right*/
void print_by_8x16(PPMImage *img)
{
	unsigned long x;
	unsigned long y;
	unsigned long column_bytes_offset;
	unsigned long a_line_bytes;

	unsigned long total_bytes_acolor;
	unsigned long lines_begin_bytes;

	int line=0;
	x = img->x;
	y = img->y;
	a_line_bytes = count_8pixels_a_linecolor(x);
	total_bytes_acolor = total_bytes_a_color(x,y);

	printf("const struct _rgb_8pixels PROGMEM hz[]={/*8x16*/\n");

	for(column_bytes_offset = 0; column_bytes_offset < a_line_bytes; column_bytes_offset++) {

		printf("/*========the number:8x16:%d=========*/\n", column_bytes_offset);
		for (lines_begin_bytes=0; lines_begin_bytes<total_bytes_acolor; lines_begin_bytes+=a_line_bytes) {

			printf("{0x%02x,",r_datas[lines_begin_bytes + column_bytes_offset]);
			printf("0x%02x,",g_datas[lines_begin_bytes + column_bytes_offset]);
			printf("0x%02x},\n",b_datas[lines_begin_bytes + column_bytes_offset]);
		}
	}
	printf("};\n");
}

void print_datas(PPMImage *img, unsigned char *data)
{
	unsigned char w = img->x;
	unsigned char h = img->y;
	unsigned short i;

	for (i=0;i < w*h/8-1;i++) {
		printf("0x%02x,",data[i]);
	}
	printf("0x%02x",data[i]);
	printf("};\n");
}

unsigned char bit_ofst(unsigned short pixel_index)
{
	return (7-(pixel_index%8));
}

unsigned char byte_ofst(unsigned short pixel_index)
{
	return pixel_index/8;
}

void output_red_datas(PPMImage *img)
{
	unsigned short i;
	unsigned char w = img->x;
	unsigned char h = img->y;
	unsigned char dat;

	for(i=0;i<w*h;i++) {
		dat = (img->data[i].red&0x01);
		r_datas[byte_ofst(i)] |= (dat<<bit_ofst(i));
	}

#ifdef PRINT_RAW_DATAS
	printf("unsigned char rdata[%d] = {\n", w*h/8);
	print_datas(img, r_datas);
#endif
}

void output_green_datas(PPMImage *img)
{
	unsigned short i;
	unsigned char w = img->x;
	unsigned char h = img->y;
	unsigned char dat;

	for(i=0;i<w*h;i++) {
		dat = (img->data[i].green&0x01);
		g_datas[byte_ofst(i)] |= (dat << bit_ofst(i));
	}

#ifdef PRINT_RAW_DATAS
	printf("unsigned char gdata[%d] = {\n", w*h/8);
	print_datas(img, g_datas);
#endif
}

void output_blue_datas(PPMImage *img)
{
	unsigned short i;
	unsigned char w = img->x;
	unsigned char h = img->y;
	unsigned char dat;

	for(i=0;i<w*h;i++) {
		dat = (img->data[i].blue&0x01);
		b_datas[byte_ofst(i)] |= (dat << bit_ofst(i));
	}
#ifdef PRINT_RAW_DATAS
	printf("unsigned char bdata[%d] = {\n", w*h/8);
	print_datas(img, b_datas);
#endif
}

void output_rgb_datas(PPMImage *img)
{
	output_blue_datas(img);
	output_green_datas(img);
	output_red_datas(img);
//	printf("=========================\n");
//	printbyline(img);
	printf("/**********print_by_8x16*************/\n");
	print_by_8x16(img);
}


static PPMImage *readPPM(const char *filename)
{
	char buff[16];
	PPMImage *img;
	FILE *fp;
	unsigned char temp, t;
	int c, rgb_comp_color;
	int i=0, line, j;
	unsigned char *rgbdatas;
	//open PPM file for reading
	fp = fopen(filename, "rb");
	if (!fp) {
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		exit(1);
	}

	//read image format
	if (!fgets(buff, sizeof(buff), fp)) {
		perror(filename);
		exit(1);
	}

	//check the image format
	printf("/*%c%c*/",buff[0], buff[1]);/*P6 P10 and so on*/
	printf("\n");
	if (buff[0] != 'P' || buff[1] != '6') {
		fprintf(stderr, "Invalid image format (must be 'P6')\n");
		exit(1);
	}

	//alloc memory form image
	img = (PPMImage *)malloc(sizeof(PPMImage));
	if (!img) {
		fprintf(stderr, "Unable to allocate memory\n");
		exit(1);
	}

	//check for comments
	c = getc(fp);
	printf("/*%c", c);
	while (c == '#') {
		while ((c = getc(fp)) != '\n') {
			printf("%c", c);
		}
		c = getc(fp);
	}
	printf("*/\n");

	ungetc(c, fp);
	//read image size information
	if (fscanf(fp, "%d %d", &img->x, &img->y) != 2) {
		fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
		exit(1);
	}
	printf("/*w=%d h=%d*/", img->x, img->y);
	printf("\n");

	printf("#define HZ_LEN %d\n", img->x/8);
	//read rgb component
	if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
		fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
		exit(1);
	}
	printf("/*color=%d*/", rgb_comp_color);//255
	printf("\n");

	//check rgb component depth
	if (rgb_comp_color!= RGB_COMPONENT_COLOR) {
		fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
		exit(1);
	}
	while (fgetc(fp) != '\n');
	//memory allocation for pixel data
	img->data = (PPMPixel*)malloc(img->x * img->y * sizeof(PPMPixel));
	rgbdatas = (unsigned char *)malloc((img->x * img->y)/2 + 1 );//why
	if (!img) {
		fprintf(stderr, "Unable to allocate memory\n");
		exit(1);
	}
	//read pixel data from file
	if (fread(img->data, 3 * img->x, img->y, fp) != img->y) {
		fprintf(stderr, "Error loading image '%s'\n", filename);
		exit(1);
	}
	/* 8x16     8x16     8x16     8x16     8x16     8x16     8x16*/
	/*12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 *12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 *12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 *12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 *12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 *12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 *12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 *12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 *12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 *12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 *12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 *12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 *12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 *12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 *12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 *12345678 12345678 12345678 12345678 12345678 12345678 12345678
	 * */
	output_rgb_datas(img);
#if 0
	for(i=0; i < img->y;i++) { //一共有多少行
		for(j=0;j<img->x/2;j++) { //如果是一行有１６个像素的话，对应的字节数为8
			printf("0x%02x,",rgbdatas[i*img->x+j]);
		}
		printf("\n");
	}
	unsigned short rdatas;
	unsigned char current_line;
	unsigned char j=0;
	for(i=0; i<img->x; i++) {
		offset= i*img->x/8;//one line 2 bytes
		shift_16bits(&r_datas[offset], &g_datas[offset], &b_datas[offset]);
	}
#endif

	fclose(fp);
	return img;
}




void writePPM(const char *filename, PPMImage *img)
{
	FILE *fp;
	//open file for output
	fp = fopen(filename, "wb");
	if (!fp) {
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		exit(1);
	}

	//write the header file
	//image format
	fprintf(fp, "P6\n");

	//comments
	fprintf(fp, "# Created by %s\n",CREATOR);

	//image size
	fprintf(fp, "%d %d\n",img->x,img->y);

	// rgb component depth
	fprintf(fp, "%d\n",RGB_COMPONENT_COLOR);

	// pixel data
	fwrite(img->data, 3 * img->x, img->y, fp);
	fclose(fp);
}

void changeColorPPM(PPMImage *img)
{
	int i;
	if(img){

		for(i=0;i<img->x*img->y;i++){
			img->data[i].red=RGB_COMPONENT_COLOR-img->data[i].red;
			img->data[i].green=RGB_COMPONENT_COLOR-img->data[i].green;
			img->data[i].blue=RGB_COMPONENT_COLOR-img->data[i].blue;
		}
	}
}

int main(int argc, char **argv){
	PPMImage *image;

	memset((void *)r_datas, 0x00, H*W/8);
	memset((void *)g_datas, 0x00, H*W/8);
	memset((void *)b_datas, 0x00, H*W/8);

	if (argc != 2) {
		printf("input file name!!\n");
		return 0;
	}

	image = readPPM(argv[1]);
//	changeColorPPM(image);
//	writePPM("can_bottom2.ppm",image);
//	printf("Press any key...");
//	getchar();
}
