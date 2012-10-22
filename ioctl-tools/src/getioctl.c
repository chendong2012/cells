#include <stdio.h>
#include <linux/ioctl.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
    
#if 0
#define _IOC(dir,type,nr,size) \
        (((dir)  << 30) | \
         ((type) << 8) | \
         ((nr)   << 0) | \
         ((size) << 16))
#endif

#define  io_w_cmd       _IOW('i', 0, unsigned long)
#define  io_r_cmd       _IOR('i', 1, unsigned long[5])

int main(int argc, char *argv[])
{
        int fd = 0;
        int handle = 0x0;

	unsigned long dir = 0;
	unsigned long size = 0;
	unsigned long type = 0;
	unsigned long nr = 0;

	printf("请输入参数:方向(dir[0,none,1为写，2为读]) 尺寸(size) (类型)type (序号)number\n");
	if (argc != 5) {
		return -1;
	}
	
	dir = strtoul(argv[1], NULL, 10);
	size = strtoul(argv[2], NULL, 10);
	type = strtoul(argv[3], NULL, 10);
	nr = strtoul(argv[4], NULL, 10);
	printf("方向(dir):%d\n", dir);
	printf("尺寸(size):%d\n", size);
	printf("类型(type):%d\n", type);
	printf("索引(number):%d\n", nr);
	printf("结果(result):0x%08x\n", _IOC(dir,type,nr,size));
        return 0;
}
