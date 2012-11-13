/*input format:
	read:
		<bus num> <dev addr> <reg addr> <r(2)> <count(4 bytes)>
	example for lis3dh:
		echo 0x00 0x19 0x20 2 0 0 0 1 > FILE
==================================================================================
	write:
		<bus num> <dev addr> <reg addr> <w(1)> <count(4 bytes)> <datas>
	example for lis3dh:
		echo 0x00 0x19 0x20 1 0 0 0 1 0x77 > FILE
*/

/*
 *实现的方法：写命令到接口，然后驱动会启动一个work去执行动作
 *，同时写接口会等待work完成。然后返回调用。
 *
 *
 *
 * */
#include <asm/atomic.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/ctype.h>

#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/wait.h>


#define FOO_TAG "i2ctool"

#define DBG
#ifdef DBG
#define FOO_PRINTK(fmt, args...)	printk(FOO_TAG" %d: %s: "fmt, __LINE__, __FUNCTION__, ##args)
#else
#define FOO_PRINTK(fmt, args...)
#endif

#define FOO_FUN(f)	printk("%s\n", __FUNCTION__)


static unsigned char all_i2c_data[4096];
static char buf_data[4096];

static struct i2c_adapter *adap;

#pragma pack(1)
struct cmd_format {
	unsigned char bus;
	unsigned char addr;
	unsigned char reg;
	unsigned char flag;
	unsigned long count;
	unsigned char *data;
};
#pragma pack()

#define CMD_HEAD_SIZE	sizeof(struct cmd_format) - sizeof(unsigned char *)

struct tool_data {
	struct mutex mutex;
	struct work_struct work;
	wait_queue_head_t waitq;
	int waitq_finish;
	struct cmd_format cmd;
};

struct tool_data ops;
static struct proc_dir_entry *dir = NULL;
static struct proc_dir_entry *entry = NULL;

unsigned char read_datas[1024];
#if 0
static int ops_i2c(void)
{
	int i;
	int ret;
	unsigned char *temp_ptr;
	struct i2c_msg msgs[2];

	memset(read_datas, 0x00, 1024);
	switch (ops.cmd.flag) {
	case 1: //write
		ops.cmd.data[0] = ops.cmd.reg;
		msgs[0].addr = ops.cmd.addr,
		msgs[0].flags = 0,
		msgs[0].len = ops.cmd.count + 1,
		msgs[0].buf = ops.cmd.data,
		msgs[0].timing = 100,
#ifdef DBG
		FOO_PRINTK("dev bus:0x%02x,addr:0x%02x,reg:0x%02x\n",
			ops.cmd.bus, ops.cmd.addr, ops.cmd.reg);
		for(i = 0; i < ops.cmd.count; i++) {
			FOO_PRINTK("write data[%d]=0x%02x\n", i, ops.cmd.data[i+1]);
		}
#endif
		adap = i2c_get_adapter(ops.cmd.bus);
		FOO_PRINTK("adap name: %s\n",adap->name);
		ret = i2c_transfer(adap, msgs, 1);
		if (ret < 0) {
			FOO_PRINTK("i2c_transfer return error: %d\n",ret);
			sprintf(read_datas,"write data retrun error %d\n", ret);
		} else {
			sprintf(read_datas,"write datas successful\n");
		}
		break;

	case 2:	//read
		msgs[0].addr=ops.cmd.addr,
		msgs[0].flags=0,
		msgs[0].len=1,
		msgs[0].buf=&ops.cmd.reg,
		msgs[0].timing=100,

		msgs[1].addr=ops.cmd.addr,
		msgs[1].flags=I2C_M_RD,
		msgs[1].len=ops.cmd.count,
		msgs[1].buf=ops.cmd.data,
		msgs[1].timing=100,
#ifdef DBG
		FOO_PRINTK("device addr=0x%02x\n",ops.cmd.addr);
		FOO_PRINTK("device reg=0x%02x\n",ops.cmd.reg);
		FOO_PRINTK("read length=0x%02x\n",ops.cmd.count);
#endif
		adap = i2c_get_adapter(ops.cmd.bus);
		FOO_PRINTK("adap name: %s\n",adap->name);

		ret = i2c_transfer(adap, msgs, 2);

		FOO_PRINTK("read msgs=0x%02x\n",ret);
		if (ret < 0) {
			FOO_PRINTK("i2c_transfer ret error: %d\n", ret);
			sprintf(read_datas,"read data retrun error %d\n", ret);
		}
		temp_ptr = read_datas;

		for(i = 0; i < ops.cmd.count; i++) {
			ret = sprintf(read_datas, "0x%02x\n", ops.cmd.data[i]);
			temp_ptr += ret;
		}
		break;
	default:
		break;

	}
}
#endif

static int ops_i2c(void)
{
	int i;
	int ret;
	unsigned char *temp_ptr;
	struct i2c_msg msgs[2];

	memset(read_datas, 0x00, 1024);
	switch (ops.cmd.flag) {
	case 1: //write
		ops.cmd.data[0] = ops.cmd.reg;
		msgs[0].addr = ops.cmd.addr,
		msgs[0].flags = 0,
		msgs[0].ext_flag = 0,
		msgs[0].len = ops.cmd.count + 1,
		msgs[0].buf = ops.cmd.data,
		msgs[0].timing = 100,
#ifdef DBG
		FOO_PRINTK("dev bus:0x%02x,addr:0x%02x,reg:0x%02x\n",
			ops.cmd.bus, ops.cmd.addr, ops.cmd.reg);
		for(i = 0; i < ops.cmd.count; i++) {
			FOO_PRINTK("write data[%d]=0x%02x\n", i, ops.cmd.data[i+1]);
		}
#endif
		adap = i2c_get_adapter(ops.cmd.bus);
		FOO_PRINTK("adap name: %s\n",adap->name);
		ret = i2c_transfer(adap, msgs, 1);
		if (ret < 0) {
			FOO_PRINTK("i2c_transfer return error: %d\n",ret);
			sprintf(read_datas,"write data retrun error %d\n", ret);
		} else {
			sprintf(read_datas,"write datas successful\n");
		}
		break;

	case 2:	/*read*/
		/*step 1:write reg to device*/
		msgs[0].addr=ops.cmd.addr,
		msgs[0].flags=0,
		msgs[0].ext_flag=0,
		msgs[0].len=1,
		msgs[0].buf=&ops.cmd.reg,
		msgs[0].timing=100,

		/*read datas from device*/
		msgs[1].addr=ops.cmd.addr,
		msgs[1].flags=I2C_M_RD,
		msgs[1].len=ops.cmd.count,
		msgs[1].buf=ops.cmd.data,
		msgs[1].ext_flag=0,
		msgs[1].timing=100,
#ifdef DBG
		FOO_PRINTK("device addr=0x%02x\n",ops.cmd.addr);
		FOO_PRINTK("device reg=0x%02x\n",ops.cmd.reg);
		FOO_PRINTK("read length=0x%02x\n",ops.cmd.count);
#endif
		adap = i2c_get_adapter(ops.cmd.bus);
		FOO_PRINTK("adap name: %s\n",adap->name);
//write begin
		ret = i2c_transfer(adap, &msgs[0], 1);
		FOO_PRINTK("read msgs 1=0x%02x\n",ret);
		if (ret < 0) {
			FOO_PRINTK("i2c_transfer send ret error: %d\n", ret);
			sprintf(read_datas,"read data retrun error %d\n", ret);
			break;
		}
//read begin
		ret = i2c_transfer(adap, &msgs[1], 1);

		if (ret < 0) {
			FOO_PRINTK("i2c_transfer recv ret error: %d\n", ret);
			sprintf(read_datas,"read data retrun error %d\n", ret);
			break;
		}

		temp_ptr = read_datas;
		for(i = 0; i < ops.cmd.count; i++) {
			ret = sprintf(temp_ptr, "0x%02x\n", ops.cmd.data[i]);
			temp_ptr += ret;
		}
#ifdef DBG
		FOO_PRINTK("read datas from device %s\n",read_datas);
#endif
		break;
	default:
		break;

	}
}

static void work_function(struct work_struct *work)
{
	printk("run work func\n");
	ops_i2c();
	ops.waitq_finish = 1;
	wake_up(&ops.waitq);
}

static int run_work(void)
{
	schedule_work(&ops.work);
}

static int create_work(void)
{
	ops.waitq_finish = 0;
	INIT_WORK(&ops.work, work_function);
}

static int trans(int i, unsigned char dat)
{
	switch (i) {
		case 0:	/*i2c bus*/
			ops.cmd.bus = dat;
			FOO_PRINTK("ops.cmd.bus:0x%08x\n",ops.cmd.bus);
			break;
		case 1: /*device address*/
			ops.cmd.addr = dat;
			FOO_PRINTK("ops.cmd.addr:0x%08x\n",ops.cmd.addr);
			break;
		case 2: /*device register address*/
			ops.cmd.reg = dat;
			FOO_PRINTK("ops.cmd.reg:0x%08x\n",ops.cmd.reg);
			break;
		case 3: //flag (w/r)
			ops.cmd.flag = dat;
			FOO_PRINTK("ops.cmd.flag:0x%08x\n",ops.cmd.flag);
			break;
		case 4:	//count
			ops.cmd.count = 0;
			ops.cmd.count |= (dat << 24);
			break;
		case 5:	
			ops.cmd.count |= (dat << 16);
			break;
		case 6:
			ops.cmd.count |= (dat << 8);
			break;
		case 7:
			ops.cmd.count |= dat;
			FOO_PRINTK("ops.cmd.count:0x%08x\n",ops.cmd.count);
			break;
		default: //data
			if (i >= 7) {
			ops.cmd.data[i-7] = dat;
			FOO_PRINTK("ops.cmd.data[%d]:0x%08x\n",i, ops.cmd.data[i]);
			}
			break;
	}
	return 0;
}

static int tool_proc_read(char *buf, char **start, off_t off,
		int count, int *eof, void *data)
{
    int len = 0;
    char *p = buf;
    p += sprintf(p, "%s", read_datas);

    *start = buf + off;
    len = p - buf;
    if (len > off)
	    len -= off;
    else
	    len = 0;

    return len < count ? len  : count;
}


static ssize_t tool_proc_write(struct file *file, const char __user *buffer,
		unsigned long count, void *data)
{
	int i = 0;
	int len = 0;
	unsigned long chunk;
	char *buf;
	char *end;

	mutex_lock(&ops.mutex);
	buf = buf_data;
	if (count > 2048) {
		FOO_PRINTK("chend:too big data\n");
		return -EINVAL;
	}
	len = count;
	if (copy_from_user(buf, buffer, len)) {
		return 0;
	}
	buf[len] = '\0';
	memset(ops.cmd.data, 0, PAGE_SIZE);
	/* buf should be <chunk> <chunk> ... or <chunk>-<chunk> ... (range) */
	while (*buf) {
		chunk = simple_strtoul(buf, &end, 0);
		if (buf == end)
			break;

		if (*end && !isspace(*end))
			break;

		trans(i, (unsigned char)chunk);
		i++;
		buf = skip_spaces(end);
	}
	//return count;
//	if ( i < 7)
//		strcpy((char *)ops.cmd.data, "error\n");
//	else {
		run_work();
		wait_event(ops.waitq, ops.waitq_finish != 0);
		ops.waitq_finish = 0;
//	}
	mutex_unlock(&ops.mutex);
	//ops_i2c();
	return count;
}

static int create_proc_dir(void)
{
	dir = proc_mkdir("DIR", NULL);
	if (!dir)
		goto fail;

        entry = create_proc_entry("FILE", S_IRUGO | S_IWUSR, dir);
	if (!entry)
		goto fail;

	entry->read_proc = tool_proc_read;
	entry->write_proc = tool_proc_write;
	return 0;
fail:
	return -EINVAL;
}

static int init_data()
{

	memset(read_datas, 0, 1024);
	memcpy(read_datas, "hello i2ctool\n", 14);
	mutex_init(&ops.mutex);
	init_waitqueue_head(&ops.waitq);
	ops.cmd.data = kzalloc(PAGE_SIZE, GFP_KERNEL);
	if (!ops.cmd.data)
		return -ENOMEM;
	return 0;

}

static int uninit_data()
{
	cancel_work_sync(&ops.work);
	remove_proc_entry("FILE", dir);
	remove_proc_entry("DIR", NULL);
	kfree(ops.cmd.data);
}

static int __init i2ctool_init(void)
{
	int ret;
	ret = init_data();
	if (ret) {
		return ret;
	}
	create_proc_dir();
	create_work();
	return 0;
}

static void __exit i2ctool_exit(void)
{
	FOO_FUN(f);
	uninit_data();
}

module_init(i2ctool_init);
module_exit(i2ctool_exit);

MODULE_DESCRIPTION("i2c read write test");
MODULE_AUTHOR("chendong <china.gd.sz.cd@gmail.com>");
MODULE_LICENSE("GPL");
