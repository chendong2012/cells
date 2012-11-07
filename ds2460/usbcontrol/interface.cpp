#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <libusb-1.0/libusb.h>

#define CONTROL_INTERFACE	1
#define DATA_INTERFACE 	0

#define TIMEOUT 1000
#define RECEIVE_PIPE	0x83
#define SEND_PIPE	0x04
#define usberror libusb_error_name(rc)
#define DEBUG_LEVEL	3	// maximum logging

struct libusb_device_handle *usbhandle;

static libusb_context *context = NULL;
int uway_init_device(void)
{
	int rc;
	if ((rc = libusb_init(&context)) != 0) {
		fprintf(stderr, "%d:%sfailed: %s\n", __LINE__, __func__, libusb_error_name(rc));
		return rc;
	}
	libusb_set_debug(context, DEBUG_LEVEL);
	return rc;
}

void uway_close_device(void)
{
	if (usbhandle != NULL) {
		libusb_release_interface(usbhandle, CONTROL_INTERFACE);
		libusb_attach_kernel_driver(usbhandle, CONTROL_INTERFACE);

		libusb_release_interface(usbhandle, DATA_INTERFACE);
		libusb_attach_kernel_driver(usbhandle, DATA_INTERFACE);

		libusb_close(usbhandle);
		usbhandle = NULL ;
	}
}

int uway_open_device(int product)
{
	int n = 0, index;
	struct libusb_device **devs = NULL;
	uway_close_device();

	if (libusb_get_device_list(context, &devs) < 0) {
		fprintf(stderr, "%d:%s failed\n", __LINE__, __func__);
		return 0;
	}

	while(devs[n] != NULL) { n++; }

	for(index=0; index<n; index++) {
		int rc;
		struct libusb_device_descriptor desc;
		char serial[256];
		libusb_get_device_descriptor(devs[index], &desc);

		if (desc.idVendor != 0x1130 && desc.idProduct != product) // ignore
			continue;
		if ((rc = libusb_open(devs[index], &usbhandle)) < 0) {
			fprintf(stderr, "open device fail %s\n", usberror);
			continue;
		}

		if (libusb_kernel_driver_active(usbhandle, DATA_INTERFACE)) {
			if ((rc = libusb_detach_kernel_driver(usbhandle, DATA_INTERFACE)) < 0)
				fprintf(stderr, "Cannot detach driver: %s\n", usberror);
		}

		if (libusb_kernel_driver_active(usbhandle, CONTROL_INTERFACE)) {
			if ((rc = libusb_detach_kernel_driver(usbhandle, CONTROL_INTERFACE)) < 0)
				fprintf(stderr, "Cannot detach driver: %s\n", usberror);
		}
		if ((rc = libusb_set_configuration(usbhandle, 1)) < 0)
			fprintf(stderr, "Cannot set device configuration: %s\n", usberror);
		if ((rc = libusb_claim_interface(usbhandle, DATA_INTERFACE)) < 0)
			fprintf(stderr, "Cannot claim device: %s\n", usberror);
		if ((rc = libusb_claim_interface(usbhandle, CONTROL_INTERFACE)) < 0)
			fprintf(stderr, "Cannot claim device: %s\n", usberror);
		if ((rc = libusb_set_interface_alt_setting(usbhandle, 0, 0)) < 0)
			fprintf(stderr, "libusb_set_interface_alt_setting failed: %s\n", usberror);
		if ((rc = libusb_get_string_descriptor_ascii(usbhandle, desc.iSerialNumber, (unsigned char *)serial, sizeof serial)) < 0)
			fprintf(stderr, "libusb_get_string_descriptor_ascii(serial) failed: %s\n", usberror);
	}
	return 0;
}

int uway_reset(int devNo)
{
	int rc;

	if (usbhandle == NULL)
		return 0;

	if ((rc = libusb_reset_device(usbhandle)) != 0)
		return 0;

	return 1;
}
/***********************************************************/
/*
 *cmd:发送的命令缓冲
 *cmd_len:发送长度
 *返回实际传回来的字节数,或是错误码
 * */
int uway_issue_control_command(unsigned char *cmd, int cmd_len)
{
	int rc = 0;
	/*发送内容：0x21 0x09 0x0200 0x0001 cmd_len [cmd]*/
	if ((rc = libusb_control_transfer(usbhandle,
					0x21,	/*请求类型:HID类描述符*/
					0x09,	/*建立包的请求*/
					0x0200,	/*建立包的wValue*/
					0x0001,	/*建立包的wIndex*/
					cmd,	/*建立包的数据*/
					cmd_len,/*数据长度*/
					0)) < 0) { /*超时值*/
		fprintf(stderr, "%d:%s failed: %s\n", __LINE__, __func__, usberror);
	}
	return rc;
}

/*
 *data:接收的命令缓冲
 *len:接收长度
 *返回0 为成功,或是错误码:负数
 * */
int uway_recv_data(unsigned char *data, int data_len)
{
	int transferred = 0;
	int ret;
	ret = libusb_interrupt_transfer(usbhandle, RECEIVE_PIPE, data, data_len, &transferred, 0);
	return ret; 
}

/*
 *data:发送的命令缓冲
 *len:发送长度
 *返回实际传输字节数,或是错误码
 * */
int uway_send_data(unsigned char *data,int len)
{
	int transferred = 0;
	int ret;
	ret = libusb_interrupt_transfer(usbhandle, SEND_PIPE, data, len, &transferred, 1000);
	return ret; 
}

/*
 *包括：1、发送命令
 *	2、读回命令对应的数据包
 *	3、小于0,表示出错, 0 OK
 * */
int uway_get_command_package(unsigned char *cmd, int cmd_len,
			unsigned char *data, int data_len)
{
	int rc;
	rc = uway_issue_control_command(cmd, cmd_len);
	if (rc < 0) {
		fprintf(stderr, "%d:%s failed: %s\n", __LINE__, __func__, usberror);
		return rc;
	}
	rc = uway_recv_data(data, data_len);
	if (rc < 0) {
		fprintf(stderr, "%d:%s failed: %s\n", __LINE__, __func__, usberror);
	}
	return rc;

}

/*
 *包括：1、发送命令
 *	2、发送命令对应的数据包
 *	3、返回刚才USB设备操作数据包的状态到data缓冲
 * */
int uway_put_command_package(unsigned char *cmd, int cmd_len,
			unsigned char *data, int data_len)
{
	int rc;

	unsigned char trans_buf[64];
	memset(trans_buf, 0x00, 64);
	memcpy(trans_buf, data, data_len);

/*step 1*/
	rc = uway_issue_control_command(cmd, cmd_len);
	if (rc < 0) {
		fprintf(stdout, "%d:%s failed: %s\n", __LINE__, __func__, usberror);
		return rc;
	}

/*step 2*/
	rc = uway_send_data(trans_buf, 64);
	if (rc < 0) {
		fprintf(stdout, "%d:%s failed: %s\n", __LINE__, __func__, usberror);
		return rc;
	}

/*step 3*/
	rc = uway_recv_data(trans_buf, 64);
	if (rc < 0) {
		fprintf(stdout, "%d:%s failed: %s\n", __LINE__, __func__, usberror);
		return rc;
	}
	memcpy(data, trans_buf, data_len);
	return rc;
}
