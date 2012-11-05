// Basic class implementation for access to USB HID devices
//
// (C) 2001 Copyright Cleware GmbH
// All rights reserved
//
// History:
// 05.01.01	ws	Initial coding

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "USBaccessBasic.h"
#include "USBaccess.h"

CUSBaccess::CUSBaccess() {
	cwInitCleware() ;
	}

CUSBaccess::~CUSBaccess() {
	}


// returns number of found Cleware devices
int
CUSBaccess::OpenCleware() {
	int rval = cwOpenCleware() ;

	return rval ;
	}

int
CUSBaccess::Recover(int devNum) {
	int rval = cwRecover(devNum) ;

	return rval ;
	}

// return true if ok, else false
int
CUSBaccess::CloseCleware() {
	cwCloseCleware() ;

	return 1 ;
	}

int
CUSBaccess::GetVersion(int deviceNo) {
	return cwGetVersion(deviceNo) ;
	}

int
CUSBaccess::GetUSBType(int deviceNo) {
	return cwGetUSBType(deviceNo) ;
	}

int
CUSBaccess::GetSerialNumber(int deviceNo) {
	return cwGetSerialNumber(deviceNo) ;
	}

// returns 1 if ok or 0 in case of an error
int
CUSBaccess::GetValue(int deviceNo, unsigned char *buf, int bufsize) {
	int rval = cwGetValue(deviceNo, buf, bufsize) ;

	return rval ;
	}


int
CUSBaccess::SetValue(int deviceNo, unsigned char *buf, int bufsize) {
	int rval = cwSetValue(deviceNo, buf, bufsize) ;

	return rval ;
	}

int
CUSBaccess::SetLED(int deviceNo, enum LED_IDs Led, int value) {
	unsigned char s[6] ;
	int rval = 0 ;

	USBtype_enum devType = (USBtype_enum)cwGetUSBType(deviceNo) ;
	int version = cwGetVersion(deviceNo) ;

	if (devType == LED_DEVICE && version <= 10) {
		s[0] = Led ;
		s[1] = value ;
		rval = SetValue(deviceNo, s, 2) ;
		}
	else if (devType == TEMPERATURE2_DEVICE || devType == HUMIDITY1_DEVICE) {
		s[0] = 0 ;
		s[1] = Led ;
		s[2] = value ;
		s[3] = 0 ;
		rval = SetValue(deviceNo, s, 4) ;
		}
	else if (devType == ENCODER01_DEVICE) {
		s[0] = 0 ;
		s[1] = Led ;
		s[2] = value ;
		s[3] = 0 ;
		s[4] = 0 ;
		s[5] = 0 ;
		rval = SetValue(deviceNo, s, 6) ;
		}
	else if (devType == CONTACT00_DEVICE && version > 6) {		// 5 bytes to send
		s[0] = 0 ;
		s[1] = Led ;
		s[2] = value ;
		s[3] = 0 ;
		s[4] = 0 ;
		rval = SetValue(deviceNo, s, 5) ;
		}
	else {
		s[0] = 0 ;
		s[1] = Led ;
		s[2] = value ;
		rval = SetValue(deviceNo, s, 3) ;
		}

	return rval ;
	}

int
CUSBaccess::SetSwitch(int deviceNo, enum SWITCH_IDs Switch, int On) {
	unsigned char s[6] ;
	int rval = 0 ;
	int version = cwGetVersion(deviceNo) ;

	USBtype_enum devType = (USBtype_enum)cwGetUSBType(deviceNo) ;
	if (devType == SWITCH1_DEVICE || devType == AUTORESET_DEVICE || devType == WATCHDOG_DEVICE) {
		s[0] = 0 ;
		s[1] = Switch ;
		if (version < 4)	// old version do not invert
			s[2] = !On ;
		else
			s[2] = On ;
		rval = SetValue(deviceNo, s, 3) ;
		if (rval && Switch == SWITCH_0) {			// set LED for first switch
			if (On) {
				SetLED(deviceNo, LED_0, 0) ;	// USB Switch will invert LED
//				Sleep(100) ;						// Linux needs that
				SetLED(deviceNo, LED_1, 15) ;
				}
			else {
				SetLED(deviceNo, LED_0, 15) ;
//				Sleep(100) ;						// Linux needs that
				SetLED(deviceNo, LED_1, 0) ;
				}
			}
		}
	else if (devType == CONTACT00_DEVICE && version > 6) {		// 5 bytes to send
		int mask = 1 << (Switch - SWITCH_0) ;		// setup mask
		int data = 0 ;
		if (On)
			data = mask ;
		s[0] = 3 ;
		s[1] = data >> 8 ;
		s[2] = data & 0xff ;
		s[3] = mask >> 8 ;
		s[4] = mask & 0xff ;
		rval = SetValue(deviceNo, s, 5) ;
		}
	else if (devType == ENCODER01_DEVICE) {
		s[0] = 0 ;
		s[1] = Switch ;
		s[2] = On ;
		s[3] = 0 ;
		s[4] = 0 ;
		s[5] = 0 ;
		rval = SetValue(deviceNo, s, 6) ;
		}

	return rval ;
	}

int		// 0=error, else=ok
CUSBaccess::GetSwitchConfig(int deviceNo, int *switchCount, int *buttonAvailable) {
	const int bufSize = 6 ;
	unsigned char buf[bufSize] ;
	int ok = 1 ;
	USBtype_enum devType = (USBtype_enum)cwGetUSBType(deviceNo) ;

	if (devType == CONTACT00_DEVICE) {
		*switchCount = 16 ;
		if (buttonAvailable)
			*buttonAvailable = 0 ;
		return ok ;
		}

	if (devType != SWITCH1_DEVICE && devType != AUTORESET_DEVICE && devType != WATCHDOG_DEVICE)
		return 0 ;

	*switchCount = 1 ;
	*buttonAvailable = 0 ;
	int version = cwGetVersion(deviceNo) ;
	if (version >= 10) {
		if (ok = GetValue(deviceNo, buf, bufSize) && (buf[0] & 0x80)) {
			*switchCount = 1 ;
			if (buf[0] & 0x02)
				*switchCount = 2 ;
			if (buf[0] & 0x08)
				*switchCount = 3 ;
			if (buf[0] & 0x20) {
				if (*switchCount == 3)
					*switchCount = 4 ;				// only single switches may have a start button
				else
					*buttonAvailable = 1 ;
				}
			}
		}

	return ok ;
	}

int		// On 0=off, 1=on, -1=error
CUSBaccess::GetSwitch(int deviceNo, enum SWITCH_IDs Switch) {
	const int bufSize = 6 ;
	unsigned char buf[bufSize] ;
	int ok = 0 ;
	USBtype_enum devType = (USBtype_enum)cwGetUSBType(deviceNo) ;

	if (		devType != SWITCH1_DEVICE
			 && devType != AUTORESET_DEVICE
			 && devType != WATCHDOG_DEVICE
			 && devType != CONTACT00_DEVICE
			 && devType != ENCODER01_DEVICE)
		return -1 ;

	int version = cwGetVersion(deviceNo) ;

	if (devType == CONTACT00_DEVICE && version > 6) {		// 5 bytes to send
		unsigned long int mask = 1 << (Switch - SWITCH_0) ;		// setup mask
		unsigned long int data = 0 ;
		ok = GetMultiSwitch(deviceNo, &mask, &data, 0) ;
		if (ok >= 0)
			ok = data ? 1 : 0 ;
		}

	else if (1 || version < 10) {					// else only if in separate thread
		if (GetValue(deviceNo, buf, bufSize)) {
			int mask = 1 << ((Switch - SWITCH_0) * 2) ;
			if (version >= 10 || devType == CONTACT00_DEVICE)
				ok = (buf[0] & mask) ? 1 : 0 ;
			else	// old switch
				ok = (buf[2] & mask) ? 1 : 0 ;
			}
		else
			ok = -1 ;	// getvalue failed - may be disconnected

		if (ok >= 0 && version < 4 && devType != CONTACT00_DEVICE)
			ok = !ok ;
		}
	else {		// new version - ask for online count to get a fast answer (use this only if in separate thread)
		static int sequenceNumber = 1 ;

		buf[0] = GetInfo ;
		buf[1] = OnlineCount ;
		buf[2] = sequenceNumber ;
		SetValue(deviceNo, buf, 3) ;
		for (int timeout=25 ; timeout > 0 ; timeout--) {
			Sleep(25) ;
			if (GetValue(deviceNo, buf, bufSize)) {
				if ((buf[0] & 0x80) == 0)	// valid bit
					continue ;
				if (buf[1] != ( (sequenceNumber & 0x1f) << 3 ) + OnlineCount)
					continue ;
				ok = buf[0] & 1 ;
				break ;
				}
			}

		sequenceNumber = (++sequenceNumber) & 0x1f ;
		}

	return ok ;
	}

int		// On 0=off, 1=on, -1=error	 ; the seqNum is generated by the Start command.
CUSBaccess::GetSeqSwitch(int deviceNo, enum SWITCH_IDs Switch, int seqNumber) {
	const int bufSize = 6 ;
	unsigned char buf[bufSize] ;
	int ok = 0 ;
	USBtype_enum devType = (USBtype_enum)cwGetUSBType(deviceNo) ;

	if (		devType != SWITCH1_DEVICE
			 && devType != AUTORESET_DEVICE
			 && devType != WATCHDOG_DEVICE
			 && devType != CONTACT00_DEVICE
			 && devType != ENCODER01_DEVICE)
		return -1 ;

	int version = cwGetVersion(deviceNo) ;
	if (version < 20 && devType != CONTACT00_DEVICE)
		return -1 ;

	if (seqNumber == 0)			// do this internally
		seqNumber = StartDevice(deviceNo) ;

	buf[1] = 0 ;
	for (int securityCnt=20 ; buf[1] != seqNumber && securityCnt > 0 ; securityCnt--) {
		if (GetValue(deviceNo, buf, bufSize)) {
			int mask = 1 << ((Switch - SWITCH_0) * 2) ;
			ok = (buf[0] & mask) ? 1 : 0 ;
			}
		else {
			ok = -1 ;	// getvalue failed - may be disconnected
			break ;
			}
		}

	return ok ;
	}

int		// rval seqNum = ok, -1 = error
CUSBaccess::GetMultiSwitch(int deviceNo, unsigned long int *mask, unsigned long int *value, int seqNumber) {
	const int bufSize = 6 ;
	unsigned char buf[bufSize] = { 0, 0, 0, 0, 0, 0 } ;
	int ok = -1 ;
	int automatic = 0 ;

	USBtype_enum devType = (USBtype_enum)cwGetUSBType(deviceNo) ;

	if (devType != CONTACT00_DEVICE)
		return -1 ;

	int version = cwGetVersion(deviceNo) ;
	if (version < 5)
		return -1 ;

	if (value == 0)
		return -1 ;

	if (seqNumber == 0)			// do this internally
		automatic = 1 ;

	int readMask = 0 ;
	if (mask)
		readMask = *mask ;
	if (readMask == 0)
		readMask = 0xffff ;		// get every single bit!!

	for (int autoCnt=4 ; autoCnt > 0 ; autoCnt--) {
		if (automatic) {
			seqNumber = SyncDevice(deviceNo, readMask) ;
			Sleep(20) ;
			}

		for (int securityCnt=50 ; seqNumber != 0 && securityCnt > 0 ; securityCnt--) {
			if (GetValue(deviceNo, buf, bufSize)) {
				if (mask != 0)
					*mask =  (buf[2] << 8) + buf[3] ;
				unsigned long int v = (buf[4] << 8) + buf[5] ;
				if (version < 7)
					*value = 0xffff & ~v ;
				else
					*value = v ;
				if (buf[1] == seqNumber) {
					ok = seqNumber ;
					break ;
					}
			//	Sleep(50) ;				don't sleep - we just killing the USB fifo
				}
			else {
				securityCnt /= 10 ;		// don't wait too long if GetValue failed
				Sleep(20) ;
				}
			}
		if (ok >= 0 || automatic == 0)
			break ;
		}

	return ok ;
	}

int		// On 0=ok, -1=error
CUSBaccess::SetMultiSwitch(int deviceNo, unsigned long int value) {
	const int bufSize = 5 ;
	unsigned char buf[bufSize] ;
	int ok = -1 ;
	USBtype_enum devType = (USBtype_enum)cwGetUSBType(deviceNo) ;

	if (devType != CONTACT00_DEVICE)
		return -1 ;

	int version = cwGetVersion(deviceNo) ;
	if (version < 5)
		return -1 ;

	buf[0] = 3 ;
	buf[1] = (unsigned char)(value >> 8) ;
	buf[2] = (unsigned char)(value & 0xff) ;
	buf[3] = 0xff ;
	buf[4] = 0xff ;
	if (SetValue(deviceNo, buf, version > 6 ? 5 : 3))
		ok = 0 ;

	return ok ;
	}

int		// On 0=ok, -1=error
CUSBaccess::SetMultiConfig(int deviceNo, unsigned long int directions) {	// 0=input, 1=output
	const int bufSize = 5 ;
	unsigned char buf[bufSize] ;
	int ok = -1 ;
	USBtype_enum devType = (USBtype_enum)cwGetUSBType(deviceNo) ;

	if (devType != CONTACT00_DEVICE)
		return -1 ;

	int version = cwGetVersion(deviceNo) ;
	if (version < 5)
		return -1 ;

	buf[0] = 4 ;
	buf[1] = (unsigned char)(directions >> 8) ;
	buf[2] = (unsigned char)(directions & 0xff) ;
	buf[3] = 0 ;
	buf[4] = 0 ;
	if (SetValue(deviceNo, buf, version > 6 ? 5 : 3))
		ok = 0 ;

	return ok ;
	}

int		// // return value of counter (0 or 1 for USB-IO16) or -1 in case of an error
CUSBaccess::GetCounter(int deviceNo, enum COUNTER_IDs counter) {
	const int bufSize = 6 ;
	unsigned char buf[bufSize] ;
	int rval = -1 ;
	int automatic = 0 ;
	static int sequenceNumber = 1 ;

	sequenceNumber = (++sequenceNumber) & 0xff ;

	USBtype_enum devType = (USBtype_enum)cwGetUSBType(deviceNo) ;

	if (devType != CONTACT00_DEVICE)
		return -1 ;

	int version = cwGetVersion(deviceNo) ;

	for (int autoCnt=4 ; autoCnt > 0 ; autoCnt--) {
		buf[0] = CUSBaccess::GetInfo ;
		buf[1] = sequenceNumber ;
		buf[2] = 0 ;
		buf[3] = 0 ;
		buf[4] = 0 ;

		if (!SetValue(deviceNo, buf, (version < 6) ? 3 : 5)) {
			Sleep(50) ;
			continue ;
			}
		Sleep(20) ;

		buf[1] = 0 ;
		for (int securityCnt=50 ; securityCnt > 0 ; securityCnt--) {
			if (GetValue(deviceNo, buf, bufSize)) {
				if (version >= 6 && buf[0] != 0xff) {			// 0xff indicates that the counters are prepared
					Sleep(10) ;
					continue ;
					}
				if (buf[1] != sequenceNumber) {
					Sleep(10) ;
					continue ;
					}
				if (version <= 6)
					rval = (buf[2] << 24) + (buf[3] << 16) + (buf[4] << 8) + buf[5] ;
				else {
					if (counter == 0)
						rval = (buf[2] << 8) + buf[3] ;
					else
						rval = (buf[4] << 8) + buf[5] ;
					}
				break ;
			//	Sleep(50) ;				don't sleep - we just killing the USB fifo
				}
			else {
				securityCnt /= 10 ;		// don't wait too long if GetValue failed
				Sleep(20) ;
				}
			}
		if (rval >= 0)
			break ;
		}

	return rval ;
	}

int		// returns how often switch is manually turned on, -1 in case of an error
CUSBaccess::GetManualOnCount(int deviceNo) {
	const int bufSize = 6 ;
	unsigned char buf[bufSize] ;
	int rval = -1 ;
	USBtype_enum devType = (USBtype_enum)cwGetUSBType(deviceNo) ;
	static int sequenceNumber = 1 ;

	if (	(	devType == SWITCH1_DEVICE
			 || devType == AUTORESET_DEVICE
			 || devType == WATCHDOG_DEVICE)
			&& cwGetVersion(deviceNo) >= 10) {
		for (int timeout=5 ; timeout > 0 ; timeout--) {
			buf[0] = GetInfo ;
			buf[1] = ManualCount ;
			buf[2] = sequenceNumber ;
			SetValue(deviceNo, buf, 3) ;
			for (int timeout2=3 ; timeout2 > 0 ; timeout2--) {
				Sleep(50) ;
				if (GetValue(deviceNo, buf, bufSize)) {
					if ((buf[0] & 0x80) == 0)	// valid bit
						continue ;
					if (buf[1] != ( (sequenceNumber & 0x1f) << 3 ) + ManualCount)
						continue ;
					if ((buf[5] & 0x80) == 0)	// valid data bit
						continue ;
					rval = buf[2] + (buf[3] << 8) + (buf[4] << 16) + ((buf[5] & 0x7f) << 24) ;
					break ;
					}
				}
			if (rval != -1)
				break ;
			Sleep(250) ;
			}
		}

	sequenceNumber = (++sequenceNumber) & 0x1f ;

	return rval ;
	}

int		// returns how long (seconds) switch is manually turned on, -1 in case of an error
CUSBaccess::GetManualOnTime(int deviceNo) {
	const int bufSize = 6 ;
	unsigned char buf[bufSize] ;
	int rval = -1 ;
	USBtype_enum devType = (USBtype_enum)cwGetUSBType(deviceNo) ;
	static int sequenceNumber = 1 ;

	if (	(	devType == SWITCH1_DEVICE
			 || devType == AUTORESET_DEVICE
			 || devType == WATCHDOG_DEVICE)
			&& cwGetVersion(deviceNo) >= 10) {
		for (int timeout=5 ; timeout > 0 ; timeout--) {
			buf[0] = GetInfo ;
			buf[1] = ManualTime ;
			buf[2] = sequenceNumber ;
			SetValue(deviceNo, buf, 3) ;
			for (int timeout2=3 ; timeout2 > 0 ; timeout2--) {
				Sleep(50) ;
				if (GetValue(deviceNo, buf, bufSize)) {
					if ((buf[0] & 0x80) == 0)	// valid bit
						continue ;
					if (buf[1] != ( (sequenceNumber & 0x1f) << 3 ) + ManualTime)
						continue ;
					if ((buf[5] & 0x80) == 0)	// valid data bit
						continue ;
					rval = buf[2] + (buf[3] << 8) + (buf[4] << 16) + ((buf[5] & 0x7f) << 24) ;
					break ;
					}
				}
			if (rval != -1)
				break ;
			Sleep(250) ;
			}
		}

	if (rval >= 0) {	// rval is 256 * 1,024 ms
		double u_seconds = 256. * 1024. ;
		u_seconds *= rval ;
		rval = (int) (u_seconds / 1000000) ;
		}

	sequenceNumber = (++sequenceNumber) & 0x1f ;

	return rval ;
	}

int		// returns how often switch is turned on by USB command, -1 in case of an error
CUSBaccess::GetOnlineOnCount(int deviceNo) {
	const int bufSize = 6 ;
	unsigned char buf[bufSize] ;
	int rval = -1 ;
	USBtype_enum devType = (USBtype_enum)cwGetUSBType(deviceNo) ;
	static int sequenceNumber = 1 ;
	int timeout=-1, timeout2=-1 ;

//	cwDebugWrite("GetOnlineCout\n") ;

	if (	(	devType == SWITCH1_DEVICE
			 || devType == AUTORESET_DEVICE
			 || devType == WATCHDOG_DEVICE)
			&& cwGetVersion(deviceNo) >= 10) {
		for (timeout=5 ; timeout > 0 ; timeout--) {
			buf[0] = GetInfo ;
			buf[1] = OnlineCount ;
			buf[2] = sequenceNumber ;
			SetValue(deviceNo, buf, 3) ;
			for (timeout2=3 ; timeout2 > 0 ; timeout2--) {
				Sleep(50) ;
				if (GetValue(deviceNo, buf, bufSize)) {
					if ((buf[0] & 0x80) == 0)	// valid bit
						continue ;
					if (buf[1] != ( (sequenceNumber & 0x1f) << 3 ) + OnlineCount)
						continue ;
					if ((buf[5] & 0x80) == 0)	// valid data bit
						continue ;
					rval = buf[2] + (buf[3] << 8) + (buf[4] << 16) + ((buf[5] & 0x7f) << 24) ;
					break ;
					}
				}
			if (rval != -1)
				break ;
			Sleep(250) ;
			}
		}

//	CString ds ;
//	ds.Format("GetOnlineOnCount(%d) %s, seq=%d, time1=%d, time2=%d\n",
//				deviceNo, (rval==-1)?"failed":"ok", sequenceNumber, timeout, timeout2) ;
//	cwDebugWrite(ds) ;

	sequenceNumber = (++sequenceNumber) & 0x1f ;

//	cwDebugWrite("GetOnlineCout\n") ;

	return rval ;
	}

int		// returns how long (seconds) switch is turned on by USB command, -1 in case of an error
CUSBaccess::GetOnlineOnTime(int deviceNo) {
	const int bufSize = 6 ;
	unsigned char buf[bufSize] ;
	int rval = -1 ;
	USBtype_enum devType = (USBtype_enum)cwGetUSBType(deviceNo) ;
	static int sequenceNumber = 1 ;

	if (	(	devType == SWITCH1_DEVICE
			 || devType == AUTORESET_DEVICE
			 || devType == WATCHDOG_DEVICE)
			&& cwGetVersion(deviceNo) >= 10) {
		for (int timeout=5 ; timeout > 0 ; timeout--) {
			buf[0] = GetInfo ;
			buf[1] = OnlineTime ;
			buf[2] = sequenceNumber ;
			SetValue(deviceNo, buf, 3) ;
			for (int timeout2=3 ; timeout2 > 0 ; timeout2--) {
				Sleep(50) ;
				if (GetValue(deviceNo, buf, bufSize)) {
					if ((buf[0] & 0x80) == 0)	// valid bit
						continue ;
					if (buf[1] != ( (sequenceNumber & 0x1f) << 3 ) + OnlineTime)
						continue ;
					if ((buf[5] & 0x80) == 0)	// valid data bit
						continue ;
					rval = buf[2] + (buf[3] << 8) + (buf[4] << 16) + ((buf[5] & 0x7f) << 24) ;
					break ;
					}
				}
			if (rval != -1)
				break ;
			Sleep(250) ;
			}
		}

	if (rval >= 0) {	// rval is 256 * 1,024 ms
		double u_seconds = 256. * 1024. ;
		u_seconds *= rval ;
		rval = (int) (u_seconds / 1000000) ;
		}

	sequenceNumber = (++sequenceNumber) & 0x1f ;

	return rval ;
	}

int
CUSBaccess::ResetDevice(int deviceNo) {
	int ok = 1 ;
	const int bufsize = 6 ;
	unsigned char buf[bufsize] ;
	int version = cwGetVersion(deviceNo) ;

	buf[0] = CUSBaccess::Reset ;
	buf[1] = 0 ;
	buf[2] = 0 ;
	buf[3] = 0 ;
	buf[4] = 0 ;
	buf[5] = 0 ;
	int type = (USBtype_enum)cwGetUSBType(deviceNo) ;
	if (type == TEMPERATURE2_DEVICE || type == HUMIDITY1_DEVICE)
		ok = SetValue(deviceNo, buf, 4) ;
	else if (type == CONTACT00_DEVICE && version > 6)
		ok = SetValue(deviceNo, buf, bufsize) ;
	else if (type == ENCODER01_DEVICE)
		ok = SetValue(deviceNo, buf, bufsize) ;
	else
		ok = SetValue(deviceNo, buf, 3) ;

	return ok ;
	}

int
CUSBaccess::StartDevice(int deviceNo) {		// mask in case of CONTACT00-device
	int ok = 1 ;
	const int bufsize = 5 ;
	unsigned char buf[bufsize] ;
	static int sequenceNumber = 1 ;

	sequenceNumber = (++sequenceNumber) & 0xff ;
	if (sequenceNumber == 0)
		sequenceNumber = 1 ;

	buf[0] = CUSBaccess::StartMeasuring ;
	buf[1] = sequenceNumber ;
	buf[2] = 0 ;
	buf[3] = 0 ;
	buf[4] = 0 ;

	int type = (USBtype_enum)cwGetUSBType(deviceNo) ;
	int version = cwGetVersion(deviceNo) ;

	if (type == TEMPERATURE2_DEVICE || type == HUMIDITY1_DEVICE)
		ok = SetValue(deviceNo, buf, 4) ;
	else if (type == CONTACT00_DEVICE && version > 6)
		ok = SetValue(deviceNo, buf, 5) ;
	else
		ok = SetValue(deviceNo, buf, 3) ;

	return (ok ? sequenceNumber : 0) ;
	}

int
CUSBaccess::SyncDevice(int deviceNo, unsigned long int mask) {		// mask in case of CONTACT00-device
	int ok = 1 ;
	const int bufsize = 5 ;
	unsigned char buf[bufsize] ;
	static int sequenceNumber = 1 ;

	sequenceNumber = (++sequenceNumber) & 0xff ;
	if (sequenceNumber == 0)
		sequenceNumber = 1 ;

	if (mask == 0)
		mask = 0xffff ;		// get every single bit!!

	buf[0] = CUSBaccess::StartMeasuring ;
	buf[1] = sequenceNumber ;
	buf[2] = 0 ;
	buf[3] = (unsigned char)(mask >> 8) ;
	buf[4] = (unsigned char)(mask & 0xff) ;

	int type = (USBtype_enum)cwGetUSBType(deviceNo) ;
	int version = cwGetVersion(deviceNo) ;

	if (type == CONTACT00_DEVICE && version > 6)
		ok = SetValue(deviceNo, buf, 5) ;

	return (ok ? sequenceNumber : 0) ;
	}

int
CUSBaccess::CalmWatchdog(int deviceNo, int minutes, int minutes2restart) {
	int ok = 0 ;
	const int bufsize = 3 ;
	unsigned char buf[bufsize] ;
	USBtype_enum devType = (USBtype_enum)cwGetUSBType(deviceNo) ;

	if (devType == AUTORESET_DEVICE || devType == WATCHDOG_DEVICE) {
		buf[0] = CUSBaccess::KeepCalm ;
		buf[1] = minutes ;
		buf[2] = minutes2restart ;
		ok = SetValue(deviceNo, buf, bufsize) ;
		}

	return ok ;
	}

int
CUSBaccess::GetTemperature(int deviceNo, double *Temperature, int *timeID) {
	int ok = 1 ;

	switch ((USBtype_enum)cwGetUSBType(deviceNo)) {
		case TEMPERATURE_DEVICE: {
			const int bufSize = 6 ;
			unsigned char buf[bufSize] ;
			// read temperature
			if (GetValue(deviceNo, buf, bufSize) == 0) {
				ok = 0 ;
				break ;
				}
			*timeID  = ((buf[0] & 0x7f) << 8) + buf[1] ;
			int value = (buf[2] << 5) + (buf[3] >> 3) ;
			if (value & 0x1000)		// negativ!
				value = (value & 0xfff) - 0x1000 ;
			int valid = (buf[0] & 0x80) ;	// MSB = valid-bit
			if (!valid) { // invalid time
				ok = 0 ;
				break ;
				}
			*Temperature = value * 0.0625 ;
			break ;
			}
		case TEMPERATURE2_DEVICE: {
			const int bufSize = 7 ;
			unsigned char buf[bufSize] ;
			// read temperature
			if (GetValue(deviceNo, buf, bufSize) == 0) {
				ok = 0 ;
				break ;
				}
			*timeID  = ((buf[0] & 0x7f) << 8) + buf[1] ;
			int value = (buf[2] << 5) + (buf[3] >> 3) ;
			if (value & 0x1000)		// negativ!
				value = (value & 0xfff) - 0x1000 ;
			int valid = (buf[0] & 0x80) ;	// MSB = valid-bit
			if (!valid) { // invalid time
				ok = 0 ;
				break ;
				}
			*Temperature = value * 0.0625 ;
			break ;
			}
		case HUMIDITY1_DEVICE:
		case TEMPERATURE5_DEVICE: {
			const int bufSize = 7 ;
			unsigned char buf[bufSize] ;
			// read temperature
			if (GetValue(deviceNo, buf, bufSize) == 0) {
				ok = 0 ;
				break ;
				}

			int version = cwGetVersion(deviceNo) ;

			*timeID  = ((buf[0] & 0x3f) << 8) + buf[1] ;
			int humi = (buf[2] << 8) + buf[3] ;
			int temp = (buf[4] << 8) + buf[5] ;
			int valid = ((buf[0] & 0xc0) == 0xc0) ;	// MSB = valid-bit
			if (valid)
				valid = ((buf[4] & 0x80) == 0) ;	// MSB must be 0
			if (valid)
				valid = (buf[4] != 0) ;				// if value is > 0x100 (temp=-37,5C) there must be an error
			if (!valid) { // invalid time
				ok = 0 ;
				break ;
				}
		//	double humidity = -4. + 0.0405 * humi - 2.8 * humi * humi / 1000000 ;
			if (version < 5)		// 14 bit
				*Temperature = -40. + 0.01 * temp ;
			else					// 12 bit
				*Temperature = -40. + 0.04 * temp ;
			if (*Temperature <= -39.99)
				ok = 0 ;					// can't happen!
			break ;
			}
		default:
			ok = 0 ;
			break ;
		}

	return ok ;
	}

int
CUSBaccess::GetHumidity(int deviceNo, double *Humidity, int *timeID) {
	int ok = 1 ;

	switch ((USBtype_enum)cwGetUSBType(deviceNo)) {
		case HUMIDITY1_DEVICE: {
			const int bufSize = 7 ;
			unsigned char buf[bufSize] ;
			// read temperature
			if (GetValue(deviceNo, buf, bufSize) == 0) {
				ok = 0 ;
				break ;
				}

			int version = cwGetVersion(deviceNo) ;

			*timeID  = ((buf[0] & 0x3f) << 8) + buf[1] ;
			int humi = (buf[2] << 8) + buf[3] ;
			int valid = ((buf[0] & 0xc0) == 0xc0) ;	// MSB = valid-bit
			if (valid)
				valid = ((buf[2] & 0x80) == 0) ;	// MSB must be 0
			if (!valid) { // invalid time
				ok = 0 ;
				break ;
				}
		//	*Temperature = -40. + 0.01 * temp ;
			if (version < 5)		// 12 bit
				*Humidity = -4. + 0.0405 * humi - 2.8 * humi * humi / 1000000 ;
			else					//  8 bit
				*Humidity = -4. + 0.648 * humi - 7.2 * humi * humi / 10000 ;
			if (*Humidity < 0.)
				ok = 0 ;				// this is not possible!!
			break ;
			}
		default:
			ok = 0 ;
			break ;
		}
	return ok ;
	}

