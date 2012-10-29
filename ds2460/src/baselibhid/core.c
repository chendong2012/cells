/*
 * 写流程(通用)：
 * send_device_command->put_device_package->get_device_package
 * 读流程
 * send_device_command->get_device_package
 * */
#include <hid.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define VENDOR_ID	0x1130
#define PRODUCT_ID	0x6807
#define  IFACE_CONTROL	1
#define PATHLEN	2

static int const PATH_IN[PATHLEN] = { 0xffa00001, 0xffa00002};
static int const PATH_OUT[PATHLEN] = { 0xffa00001, 0xffa00002};
HIDInterface* hid = NULL;

int open_usb_device()
{
	hid_return ret;

	HIDInterfaceMatcher matcher = { VENDOR_ID, PRODUCT_ID, NULL, NULL, 0 };

	hid_set_debug(HID_DEBUG_NONE);
	hid_set_debug_stream(stderr);
	hid_set_usb_debug(0);

	ret = hid_init();
	if (ret != HID_RET_SUCCESS) {
		fprintf(stderr, "hid_init failed with return code %d\n", ret);
		return -1;
	}

	hid = hid_new_HIDInterface();
	if (hid == 0) {
		fprintf(stderr, "hid_new_HIDInterface() failed, out of memory?\n");
		return -1;
	}
	ret = hid_force_open(hid, IFACE_CONTROL, &matcher, 3);
	if (ret != HID_RET_SUCCESS) {
		fprintf(stderr, "hid_force_open failed with return code %d\n", ret);
		return -1;
	}

	ret = hid_os_force_claim(hid, 0, &matcher, 3); 
	if (ret != HID_RET_SUCCESS) {
		fprintf(stderr, "hid_os_force_claim failed with return code %d\n", ret);
		return -1;
	}
	hid->interface = 1; 
	return 0; 
}

int close_usb_device()
{
	hid_return ret; 
	/*step 1*/
	ret = hid_close(hid);
	if (ret != HID_RET_SUCCESS) {
		fprintf(stderr, "hid_close failed with return code %d\n", ret);
		return -1;
	}

	/*step 2*/
	hid_delete_HIDInterface(&hid);

	/*step 3*/
	ret = hid_cleanup();
	if (ret != HID_RET_SUCCESS) {
		fprintf(stderr, "hid_cleanup failed with return code %d\n", ret);
		return -1;
	}
	return 0;
}

 /*
  * set cmd len is 8
  * */
int send_device_command(unsigned char *cmd)
{
	hid_return ret;
	ret = hid_set_output_report(hid, PATH_IN, PATHLEN, cmd, 8);
	if (ret != HID_RET_SUCCESS) {
		fprintf(stderr, "hid_set_output_report failed with return code %d\n", ret);
	}
	return ret; 
}

/*
 *set cmd len is 8
 *set retbuf len is 64
 * */
int get_device_package( unsigned char *retbuf)
{
	hid_return ret;
	ret = hid_interrupt_read(hid, 3, retbuf, 64, 1000); 

	if (ret != HID_RET_SUCCESS) {
		fprintf(stderr, "hid_close failed with return code %d\n", ret);
		return -1;
	}
	return 0; 
}

/*
 *set cmd len is 8
 *set sendbuf len is 64
 * */
int put_device_package(unsigned char *sendbuf)
{
	hid_return ret;
	ret = hid_interrupt_write(hid, 4, sendbuf, 64, 1000); 

	if (ret != HID_RET_SUCCESS) {
		fprintf(stderr, "hid_close failed with return code %d\n", ret);
		return -1;
	}
	return 0; 
}
