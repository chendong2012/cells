#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <syslog.h>
#include "USBaccess.h"

int retry_count = 10;
char brief = 0;

const char * device_id_to_string(int id)
{
	switch(id)
	{
	case CUSBaccess::LED_DEVICE:
		return "LED";
	case CUSBaccess::WATCHDOG_DEVICE:
		return "Watchdog";
	case CUSBaccess::AUTORESET_DEVICE:
		return "Autoreset device";
	case CUSBaccess::SWITCH1_DEVICE:
		return "Switch1";
	case CUSBaccess::SWITCH2_DEVICE:
		return "Switch2";
	case CUSBaccess::SWITCH3_DEVICE:
		return "Switch3";
	case CUSBaccess::SWITCH4_DEVICE:
		return "Switch4";
	case CUSBaccess::TEMPERATURE_DEVICE:
		return "Temperature sensor";
	case CUSBaccess::TEMPERATURE2_DEVICE:
		return "Temperature 2 sensor";
	case CUSBaccess::TEMPERATURE5_DEVICE:
		return "Temperature 5 sensor";
	case CUSBaccess::HUMIDITY1_DEVICE:
		return "Humidity sensor";
	case CUSBaccess::CONTACT00_DEVICE:
		return "Contact 00 device";
	case CUSBaccess::CONTACT01_DEVICE:
		return "Contact 01 device";
	case CUSBaccess::CONTACT02_DEVICE:
		return "Contact 02 device";
	case CUSBaccess::CONTACT03_DEVICE:
		return "Contact 03 device";
	case CUSBaccess::CONTACT04_DEVICE:
		return "Contact 04 device";
	case CUSBaccess::CONTACT05_DEVICE:
		return "Contact 05 device";
	case CUSBaccess::CONTACT06_DEVICE:
		return "Contact 06 device";
	case CUSBaccess::CONTACT07_DEVICE:
		return "Contact 07 device";
	case CUSBaccess::CONTACT08_DEVICE:
		return "Contact 08 device";
	case CUSBaccess::CONTACT09_DEVICE:
		return "Contact 09 device";
	case CUSBaccess::CONTACT10_DEVICE:
		return "Contact 10 device";
	case CUSBaccess::CONTACT11_DEVICE:
		return "Contact 11 device";
	case CUSBaccess::CONTACT12_DEVICE:
		return "Contact 12 device";
	case CUSBaccess::CONTACT13_DEVICE:
		return "Contact 13 device";
	case CUSBaccess::CONTACT14_DEVICE:
		return "Contact 14 device";
	case CUSBaccess::CONTACT15_DEVICE:
		return "Contact 15 device";
	case CUSBaccess::ENCODER01_DEVICE:
		return "Encoder 01 device";
	case CUSBaccess::SECRECT_DEVICE:
		return "secrect device";
	case CUSBaccess::BUTTON_NODEVICE:
		return "Button no device";
	}

	return "Device ID not recognised!";
}

void list_devices(void)
{
	CUSBaccess CWusb;
        int USBcount = CWusb.OpenCleware();

	printf("Cleware library version: %d\n", CWusb.GetDLLVersion());
        printf("Number of Cleware devices found: %d\n", USBcount);

        for (int devID=0; devID < USBcount; devID++)
	{
                int devType = CWusb.GetUSBType(devID);

		printf("Device: %d, type: %s (%d), version: %d, serial number: %d\n",
			devID,
			device_id_to_string(devType), devType,
			CWusb.GetVersion(devID),
			CWusb.GetSerialNumber(devID)
			);
	}

	CWusb.CloseCleware();
}

void version(void)
{
	fprintf(stderr, "clewarecontrol " VERSION ", (C) 2005-2012 by folkert@vanheusden.com\n");
}

void usage(void)
{
	version();

	fprintf(stderr, "-l       list devices\n");
	fprintf(stderr, "-d x     use device with serial number 'x' for the next operations\n");
	fprintf(stderr, "-c x     set retry-count when accessing device\n");
	fprintf(stderr, "-rt      read temperature\n");
	fprintf(stderr, "-rh      read humidity\n");
	fprintf(stderr, "-rs x    read switch 'x'\n");
	fprintf(stderr, "-rp      read external switch\n");
	fprintf(stderr, "-rr      shows how often the auto-reset kicked in\n");
	fprintf(stderr, "-rm      read states of the USB-IO16 lines\n");
	fprintf(stderr, "-ar      reset device\n");
	fprintf(stderr, "-as x y  set switch x to y (0=off, 1=on)\n");
	fprintf(stderr, "-ag      start device\n");
	fprintf(stderr, "-al x y  set led x to y (0...15)\n");
	fprintf(stderr, "-am x    set the states of the USB-IO16 lines: x must be a hexvalue\n");
	fprintf(stderr, "-ad x    set the directions of the USB-IO16 lines (hexvalue)\n");
	fprintf(stderr, "-cfg x   configure the device to be a watchdog (0), autoreset (1), switch (2) or switch ATXX (3)\n");
	fprintf(stderr, "-w       become daemon-process that pats the watchdog\n");
	fprintf(stderr, "-b       Brief output: just the measured temperature etc. without any descriptions.\n");
}

#define NO_COMMAND	0
#define READ_TEMP	1
#define READ_SWITCH	2
#define RESET_DEVICE	3
#define SET_SWITCH	4
#define READ_HUMIDITY	5
#define START_DEVICE	6
#define SET_LED		7
#define READ_EXTERNAL_SWITCH	8
#define READ_AUTO_RESET	9
#define READ_MULTISWITCH	10
#define SET_MULTISWITCH	11
#define MULTISWITCH_DIRECTION	12
#define CONFIGURE_SWITCH	13

int find_usb_id(CUSBaccess *pCWusb, int dev_count, int device_id)
{
	int devID, usb_id = -1;

	for(devID=0; devID<dev_count; devID++)
	{
		if (device_id == -1 || pCWusb -> GetSerialNumber(devID) == device_id)
		{
			usb_id = devID;
			break;
		}
	}

	if (usb_id == -1)
	{
		fprintf(stderr, "Device with serial number %d not found.\n", device_id);
	}

	return usb_id;
}

int start_tapping_the_watchdog(int device_id)
{
	CUSBaccess CWusb;
        int dev_count = CWusb.OpenCleware();

	int usb_id = find_usb_id(&CWusb, dev_count, device_id);

	pid_t pid = fork();

	if (pid == -1)
	{
		fprintf(stderr, "Failed to fork");
		return 1;
	}

	if (pid == 0)
	{
		int err_cnt = 0;

		if (daemon(0, 0) == -1)
		{
			fprintf(stderr, "Failed to become daemon process\n");
			return 1;
		}

		for(;;)
		{
			int time1 = 1, time2 = 5;

			if (!CWusb.CalmWatchdog(usb_id, time1, time2))
			{
				err_cnt++;

				if (err_cnt == 10)
				{
					syslog(LOG_ERR, "Failed to tap the watchdog 10 times");
					exit(1);
				}
			}

			sleep(2);
		}
	}

	printf("Watchdog started\n");

	return 0;
}

int do_command(int device_id, int command, int par, int par2, double offset)
{
	int rc = 1;
        CUSBaccess CWusb;
        int USBcount = CWusb.OpenCleware();
	int usb_id = -1, retry;

	if (device_id == -1)
	{
		fprintf(stderr, "You did not select a device, using first device found.\n");
	}

	usb_id = find_usb_id(&CWusb, USBcount, device_id);
	if (usb_id == -1)
		return -1;

	if (device_id == -1)
	{
		if (!brief) printf("Using device with serial number: %d\n", CWusb.GetSerialNumber(usb_id));
	}

	for(retry=0; retry<retry_count; retry++)
	{
		if (command == READ_TEMP)
		{
			double temperature;
			static int prev_time = -1;
			int time;

			if (CWusb.GetTemperature(usb_id, &temperature, &time) && time != prev_time)
			{
				if (brief)
					printf("%f\n", temperature + offset);
				else
					printf("Temperature: %f\n", temperature + offset);
				rc = 0;
			}
		}
		else if (command == READ_SWITCH)
		{
			int status;

			if ((status = CWusb.GetSwitch(usb_id, (CUSBaccess::SWITCH_IDs)par)) != -1)
			// if ((status = CWusb.GetSeqSwitch(usb_id, (CUSBaccess::SWITCH_IDs)par, 0)) != -1)
			{
				if (brief)
					printf("%s\n", status?"On":"Off");
				else
					printf("Status: %s (%d)\n", status?"On":"Off", status);
				rc = 0;
			}
		}
		else if (command == RESET_DEVICE)
		{
			if (CWusb.ResetDevice(usb_id))
			{
				if (!brief) printf("Device resetted\n");
				rc = 0;
			}
		}
		else if (command == SET_SWITCH)
		{
			if (CWusb.SetSwitch(usb_id, (CUSBaccess::SWITCH_IDs)par, par2))
			{
				if (!brief) printf("Switch %d set to %s\n", par - 16, par2?"On":"Off");
				rc = 0;
			}
		}
		else if (command == READ_HUMIDITY)
		{
			double humidity;
			int time;

			if (CWusb.GetHumidity(usb_id, &humidity, &time))
			{
				if (brief)
					printf("%f\n", humidity + offset);
				else
					printf("Humidity: %f\n", humidity + offset);

				rc = 0;
			}
		}
		else if (command == START_DEVICE)
		{
			if (CWusb.StartDevice(usb_id))
			{
				if (!brief) printf("Device started\n");
				rc = 0;
			}
		}
		else if (command == SET_LED)
		{
			if (CWusb.SetLED(usb_id, (CUSBaccess::LED_IDs)par, par2))
			{
				if (!brief) printf("LED %d set to %d\n", par, par2);
				rc = 0;
			}
		}
		else if (command == READ_EXTERNAL_SWITCH)
		{
			if (brief)
				printf("%d %d\n", CWusb.GetManualOnCount(usb_id), CWusb.GetManualOnTime(usb_id));
			else
			{
				printf("The plug was switched %d times on.\n", CWusb.GetManualOnCount(usb_id));
				printf("The plug is switched on for %d seconds.\n", CWusb.GetManualOnTime(usb_id));
			}
			rc = 0;
		}
		else if (command == READ_AUTO_RESET)
		{
			if (brief)
				printf("%d %d\n", CWusb.GetOnlineOnCount(usb_id), CWusb.GetOnlineOnTime(usb_id));
			else
			{
				printf("The auto-reset has kicked in %d times on.\n", CWusb.GetOnlineOnCount(usb_id));
				printf("The auto-reset has resetted %d seconds ago.\n", CWusb.GetOnlineOnTime(usb_id));
			}
			rc = 0;
		}
		else if (command == READ_MULTISWITCH)
		{
			unsigned long int value = 0;

			if (CWusb.GetMultiSwitch(usb_id, NULL, &value, 0))
			{
				int loop;

				if (!brief) printf("States of lines: ");
				for(loop=15; loop>=0; loop--)
				{
					if (value & (1 << loop))
						printf("1");
					else
						printf("0");
				}
				printf("\n");

				rc = 0;
			}
		}
		else if (command == SET_MULTISWITCH)
		{
			if (CWusb.SetMultiSwitch(usb_id, par))
			{
				if (!brief) printf("Bit-pattern set.\n");
				rc = 0;
			}
		}
		else if (command == MULTISWITCH_DIRECTION)
		{
			if (CWusb.SetMultiConfig(usb_id, par))
			{
				if (!brief) printf("Directions set.\n");
				rc = 0;
			}
		}
		else if (command == CONFIGURE_SWITCH)
		{
			int type = CWusb.GetUSBType(usb_id);

			if (type == CUSBaccess::WATCHDOG_DEVICE  ||
			    type == CUSBaccess::AUTORESET_DEVICE ||
			    type == CUSBaccess::SWITCH1_DEVICE)
			{
				if (par == 0 ||
				    par == 1 ||
				    par == 2 ||
				    par == 3)
				{
					unsigned char buf[3];

					buf[0] = CUSBaccess::EEwrite;
					buf[1] = 2;
					buf[2] = par;  // =watchdog, 1=autoreset, 2=switch, 3=switch ATXX
					CWusb.SetLED(usb_id, CUSBaccess::LED_3, 7);
					CWusb.SetValue(usb_id, buf, 3);
					CWusb.SetLED(usb_id, CUSBaccess::LED_3, 0);

					fprintf(stderr, "No first unplug (and replug) the device before use!\n");

					rc = 0;
				}
				else
				{
					fprintf(stderr, "%d is not a valid setting. Valid parameters: 0 (watchdog), 1 (autoreset), 2 (switch) and 3 (switch ATXX)\n", par);
				}
			}
			else
			{
				fprintf(stderr, "One can only perform this action a watchdog-, an autoreset- or a switch device!\n");
			}
		}
		else
		{
			fprintf(stderr, "Internal error!\n");
			exit(1);
		}

		usleep(500000);
	}

	if (rc)
		fprintf(stderr, "Failed to access device %d\n", CWusb.GetSerialNumber(usb_id));

	CWusb.CloseCleware();

	return rc;
}

int main(int argc, char *argv[])
{
	int loop, device_id = -1;
	int par = -1, par2 = -1;
	double offset = 0.0;

	if (argc < 2)
	{
		usage();
		return 1;
	}

	signal(SIGCHLD, SIG_IGN);

	// FIXME use getopt
	for(loop=1; loop<argc; loop++)
	{
		if (strcmp(argv[loop], "-l") == 0)
		{
			list_devices();
			return 0;
		}
		else if (strcmp(argv[loop], "-o") == 0)
		{
			offset = atof(argv[++loop]);
		}
		else if (strcmp(argv[loop], "-d") == 0)
		{
			device_id = atoi(argv[++loop]);
		}
		else if (strcmp(argv[loop], "-c") == 0)
		{
			retry_count = atoi(argv[++loop]);
		}
		else if (strcmp(argv[loop], "-rt") == 0)
		{
			if (do_command(device_id, READ_TEMP, -1, -1, offset))
				return 1;
		}
		else if (strcmp(argv[loop], "-rs") == 0)
		{
			par = atoi(argv[++loop]);
			if (par < 16)
				par += 16;
			if (do_command(device_id, READ_SWITCH, par, -1, -1000.0))
				return 1;
		}
		else if (strcmp(argv[loop], "-ar") == 0)
		{
			if (do_command(device_id, RESET_DEVICE, -1, -1, -1000.0))
				return 1;
		}
		else if (strcmp(argv[loop], "-as") == 0)
		{
			par = atoi(argv[++loop]);
			if (par < 16)
				par += 16;
			par2 = atoi(argv[++loop]);
			if (do_command(device_id, SET_SWITCH, par, par2, -1000.0))
				return 1;
		}
		else if (strcmp(argv[loop], "-al") == 0)
		{
			par = atoi(argv[++loop]);
			par2 = atoi(argv[++loop]);
			if (do_command(device_id, SET_LED, par, par2, -1000.0))
				return 1;
		}
		else if (strcmp(argv[loop], "-rh") == 0)
		{
			if (do_command(device_id, READ_HUMIDITY, -1, -1, offset))
				return 1;
		}
		else if (strcmp(argv[loop], "-ag") == 0)
		{
			if (do_command(device_id, START_DEVICE, -1, -1, -1000.0))
				return 1;
		}
		else if (strcmp(argv[loop], "-w") == 0)
		{
			if (start_tapping_the_watchdog(device_id))
				return 1;
		}
		else if (strcmp(argv[loop], "-rp") == 0)
		{
			if (do_command(device_id, READ_EXTERNAL_SWITCH, -1, -1, -1000.0))
				return 1;
		}
		else if (strcmp(argv[loop], "-rr") == 0)
		{
			if (do_command(device_id, READ_AUTO_RESET, -1, -1, -1000.0))
				return 1;
		}
		else if (strcmp(argv[loop], "-rm") == 0)
		{
			if (do_command(device_id, READ_MULTISWITCH, -1, -1, -1000.0))
				return 1;
		}
		else if (strcmp(argv[loop], "-am") == 0)
		{
			par = strtol(argv[++loop], NULL, 16);
			if (do_command(device_id, SET_MULTISWITCH, par, -1, -1000.0))
				return 1;
		}
		else if (strcmp(argv[loop], "-ad") == 0)
		{
			par = strtol(argv[++loop], NULL, 16);
			if (do_command(device_id, MULTISWITCH_DIRECTION, par, -1, -1000.0))
				return 1;
		}
		else if (strcmp(argv[loop], "-cfg") == 0)
		{
			par = atoi(argv[++loop]);
			if (do_command(device_id, CONFIGURE_SWITCH, par, -1, -1000.0))
				return 1;
		}
		else if (strcmp(argv[loop], "-b") == 0)
		{
			brief = 1;
		}
		else if (strcmp(argv[loop], "-h") == 0)
		{
			usage();
			return 0;
		}
		else if (strcmp(argv[loop], "-V") == 0)
		{
			version();
			return 0;
		}
		else
		{
			fprintf(stderr, "Switch '%s' is not recognized.\n", argv[loop]);
			return 1;
		}
	}

	return 0;
}
