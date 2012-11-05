// Basic class definitions for access to USB HID devices
//
// (C) 2001 Copyright Cleware GmbH
// All rights reserved
//
// History:
// 05.01.01	ws	Initial coding
// 17.07.01	ws	cleanup interface
// 03.11.02	ws	small changes for Linux


#ifndef __USBACCESSBASIC_H__
#define __USBACCESSBASIC_H__

enum cwUSBtype_enum {	ILLEGAL_DEVICE=0,
			LED_DEVICE=0x01,
			WATCHDOG_DEVICE=0x05,
			AUTORESET_DEVICE=0x06,
			SWITCH1_DEVICE=0x08,
			TEMPERATURE_DEVICE=0x10,
			TEMPERATURE2_DEVICE=0x11,
			TEMPERATURE5_DEVICE=0x15, 
			HUMIDITY1_DEVICE=0x20,
			CONTACT1_DEVICE=0x30,
			SECRECT_DEVICE=0x6807
			};

typedef struct {
	struct libusb_device_handle *handle;
	int	gadgetVersionNo;
	enum cwUSBtype_enum	gadgettype;
	int	SerialNumber;
	int	report_type;
	} cwSUSBdata;

void cwInitCleware();
int	cwOpenCleware();	// returns number of found Cleware devices
int cwRecover(int devNo);
void cwCloseCleware();
int	cwGetValue(int deviceNo, unsigned char *buf, int bufsize);
int	cwSetValue(int deviceNo, unsigned char *buf, int bufsize);
struct libusb_device_handle * cwGetHandle(int deviceNo);
int	cwGetVersion(int deviceNo);
int	cwGetSerialNumber(int deviceNo);
enum cwUSBtype_enum	cwGetUSBType(int deviceNo);

#endif // __USBACCESS_H__
