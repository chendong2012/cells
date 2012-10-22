#include <stdio.h>
int main(int argc, char *argv[])
{
	int i=1;
	unsigned char hex;
	printf("说明：字符转为16进制的应用程序，支持多个字符转16进制\n");
	printf("例:chartodigter e b c\n");
	printf("结果：\n");
	printf("16进制：\n");
	for(i=1; i < argc; i++) {
		sscanf(argv[i], "%c", &hex);
		printf("0x%02x ", hex);
	}
	printf("\n");
	printf("10进制：\n");
	for(i=1; i < argc; i++) {
		sscanf(argv[i], "%c", &hex);
		printf("%02d ", hex);
	}
	printf("\n");
	return 0;
}
