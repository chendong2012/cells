#include <stdio.h>
int main(int argc, char *argv[])
{
	int i=1;
	printf("说明：16进制转为字符的应用程序，支持多个16进制转字符串\n");
	printf("hextochar 0xxx 0xxx 0xxx\n");
	printf("结果：\n");
	for(i=1; i < argc; i++)
		printf("%c", strtoul(argv[i], NULL, 16));
	printf("\n");
	return 0;
}
