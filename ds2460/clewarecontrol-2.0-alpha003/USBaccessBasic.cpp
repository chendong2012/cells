// Basic class implementation for access to USB HID devices
//
// (C) 2001 Copyright Cleware GmbH
// All rights reserved
//
// History:
// 05.01.01	ws	Initial coding
// 01.11.01	ws	Linux coding
// 30.10.12	fvh	libusb version

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <libusb-1.0/libusb.h>

#include "USBaccessBasic.h"

#define TIMEOUT 1000

const int maxHID = 128 ;
cwSUSBdata data[128] ;
libusb_context *context = NULL;

void cwInitCleware()
{
	int h, rc;

	for(h=0 ; h < maxHID ; h++)
		data[h].handle = NULL;

	if ((rc = libusb_init(&context)) != 0)
		fprintf(stderr, "libusb_init() failed: %s\n", libusb_error_name(rc));

	libusb_set_debug(context, 3); // maximum logging
}

void cwCloseCleware()
{
	int h;

	for (h=0 ; h < maxHID; h++)
	{
		if (data[h].handle != NULL)
		{
			libusb_release_interface(data[h].handle, 0);

			libusb_attach_kernel_driver(data[h].handle, 0);

			libusb_close(data[h].handle);

			data[h].handle = NULL ;
		}
	}
}

// returns number of found Cleware devices
int cwOpenCleware()
{
	int handleCount = 0, n = 0, index;
	struct libusb_device **devs = NULL;

	cwCloseCleware();

	if (libusb_get_device_list(context, &devs) < 0)
	{
		fprintf(stderr, "libusb_get_device_list() failed\n");
		return 0;
	}

	while(devs[n] != NULL) { n++; }

	for(index=0; index<n; index++)
	{
		struct libusb_device_descriptor desc;
		char serial[256];
		int rc;

		libusb_get_device_descriptor(devs[index], &desc);

		if (desc.idVendor != 0x1130) // ignore
			continue;

		if ((rc = libusb_open(devs[index], &data[handleCount].handle)) < 0)
		{
			fprintf(stderr, "Cannot open device %04x:%04x: %s\n", desc.idVendor, desc.idProduct, libusb_error_name(rc));
			continue;
		}

		if (libusb_kernel_driver_active(data[handleCount].handle, 0))
		{
			if ((rc = libusb_detach_kernel_driver(data[handleCount].handle, 0)) < 0)
				fprintf(stderr, "Cannot detach driver: %s\n", libusb_error_name(rc));
		}

		if ((rc = libusb_set_configuration(data[handleCount].handle, 1)) < 0)
			fprintf(stderr, "Cannot set device configuration: %s\n", libusb_error_name(rc));

		if ((rc = libusb_claim_interface(data[handleCount].handle, 0)) < 0)
			fprintf(stderr, "Cannot claim device: %s\n", libusb_error_name(rc));

		if ((rc = libusb_set_interface_alt_setting(data[handleCount].handle, 0, 0)) < 0)
			fprintf(stderr, "libusb_set_interface_alt_setting failed: %s\n", libusb_error_name(rc));

		data[handleCount].gadgettype = (enum cwUSBtype_enum)desc.idProduct;
		data[handleCount].gadgetVersionNo = desc.bcdDevice;

		if ((rc = libusb_get_string_descriptor_ascii(data[handleCount].handle, desc.iSerialNumber, (unsigned char *)serial, sizeof serial)) < 0)
			fprintf(stderr, "libusb_get_string_descriptor_ascii(serial) failed: %s\n", libusb_error_name(rc));

		data[handleCount].SerialNumber = strtol(serial, NULL, 16);

		handleCount++;
	}

	return handleCount;
}

int cwRecover(int devNo)
{
	int rc;

	if (data[devNo].handle == NULL)
		return 0;

	if ((rc = libusb_reset_device(data[devNo].handle)) != 0)
		return 0;

	return 1;
}

int cwGetValue(int deviceNo, unsigned char *buf, int bufsize)
{
	int rc, transferred = 0;

	if (data[deviceNo].handle == NULL)
		return 0;

        if ((rc = libusb_bulk_transfer(data[deviceNo].handle, 0x81, buf, bufsize, &transferred, 0)) < 0)
        {
                fprintf(stderr, "libusb_interrupt_transfer(0x81) failed: %s\n", libusb_error_name(rc));

                return 0;
        }

	return 1;
}

int cwSetValue(int deviceNo, unsigned char *buf, int bufsize)
{
	int rc, transferred = 0;

	if (data[deviceNo].handle == NULL)
		return 0;

	if ((rc = libusb_control_transfer(data[deviceNo].handle, 0x21, 0x09, 0x200, 0x00, buf, bufsize, 0)) < 0)
	{
                fprintf(stderr, "libusb_interrupt_transfer(0x01) failed: %s\n", libusb_error_name(rc));

                return 0;
	}

	return 1;
}

struct libusb_device_handle * cwGetHandle(int deviceNo)
{ 
	return data[deviceNo].handle;
}

int cwGetVersion(int deviceNo)
{
	if (data[deviceNo].handle == NULL)
		return -1;

	return data[deviceNo].gadgetVersionNo;
}

int cwGetSerialNumber(int deviceNo)
{ 
	if (data[deviceNo].handle == NULL)
		return -1;

	return data[deviceNo].SerialNumber;
}

enum cwUSBtype_enum cwGetUSBType(int deviceNo)
{ 
	if (data[deviceNo].handle == NULL)
		return (cwUSBtype_enum)-1;

	return data[deviceNo].gadgettype;
}
