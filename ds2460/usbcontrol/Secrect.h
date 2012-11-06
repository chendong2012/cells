// DLL class definitions for access to USB HID devices
//
// (C) 2001-2003 Copyright Cleware GmbH
// All rights reserved
//
// History:
// 05.01.2001	ws	Initial coding
// 17.07.2001	ws	cleanup interface
// 10.12.2001	ws	cleanup interface again, basic class to hide implementation details
// 13.12.2001	ws	introduced versionNumber and virtual destructor
// 23.05.2002	ws	added switch access
// ...
// 03.02.2003	ws	added switch version 10
// 04.08.2003	ws	added humidity
// 21.01.2001	ws	fixed some humidity problems


// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the USBACCESS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// USBACCESS_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.


#ifndef __SECRECT_H__
#define __SECRECT_H__
class Secrect {
public:
	public:
		int read_rom_id(unsigned char *romid, int romid_len);
		int read_mac_code(unsigned char *mac, int mac_len);
		int read_eeprom(int addr, unsigned char *data, int len);

		int write_input_data(unsigned char *data, int data_len);
		int write_s_secrect(unsigned char *secrect, int secrect_len);
		int write_e_secrect(int index, unsigned char *secrect, int secrect_len)

		int write_compute_cmd(unsigned char compute_cmd);
		int write_trans_cmd(unsigned char trans_cmd);
		int write_eeprom(int addr, unsigned char *data, int len);
		
		int check_mac_device();
	};
#endif
