#include <stdio.h>
#include <string.h>
#include <usb.h>

int main(int argc, char *argv[])
{
	struct usb_bus *bus;
	struct usb_device *dev, *mydev;
	usb_dev_handle *handle;
	unsigned char bytes[64];
	int ret;
	int i; 

	for (ret = 0; ret < 64; ret++) {
		 bytes[ret] = 0;
	}

	bytes[0] = 0; 
	bytes[1] = 0; 
	bytes[2] = 0; 
	bytes[3] = 0; 
	bytes[4] = 0; 
	bytes[5] = 0; 
	bytes[6] = 0x01; 
	bytes[7] = 0xbb; 

	usb_init();
	usb_find_busses();
	usb_find_devices();

	for (bus = usb_busses; bus; bus = bus->next) {
		for (dev = bus->devices; dev; dev = dev->next) {
			if (dev->descriptor.idVendor == 0x1130 && dev->descriptor.idProduct == 0x6807) {
				printf("find my device\n");
				mydev = dev;
				break;
			}
		}
	}

	handle = usb_open(mydev);
	printf("handle = %d\n", handle);

	//usb_get_driver_np(handle, 0, bytes, 64); 
	//fprintf(stderr,  "return usb_get_driver name is %s\n",  bytes); 

#if 1
	ret = usb_detach_kernel_driver_np(handle, 1); 
	fprintf(stderr,  "usb_detach_kernel_driver_np return %d\n",  ret); 
	ret = usb_claim_interface(handle, 1);
	printf("claim interface return %d\n",  ret); 
#endif
	
#if 1
	ret = usb_detach_kernel_driver_np(handle, 0); 
	fprintf(stderr,  "return usb_detach_kernel_driver_np is %d\n",  ret); 
	ret = usb_claim_interface(handle, 0);
	printf("claim interface return %d\n",  ret); 
#endif

	ret = usb_control_msg(handle, 0x21, 0x09,
	0x0200, 1, bytes, 0x0008, 1000);
	printf("usb_control_msg return %d\n",  ret); 

	sleep(1); 
	ret = usb_interrupt_read(handle, 3, bytes, 64,  1000); 
	printf("usb_interrupt_read return %d\n",  ret); 
	for (i = 0;  i < 64;  i++) {
		printf("0x%02x ", bytes[i]); 
	}
	printf("\n"); 

	usb_release_interface(handle, 0); 
	usb_release_interface(handle, 1); 
	usb_close(handle); 
	return 0;
}
