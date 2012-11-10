#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <syslog.h>
#include "Secrect.h"

#include <getopt.h>
#include <unistd.h>

#define EEPROM_PAGE 14
#define EEPROM_PAGE_SIZE 8

struct secrect_device_context {
	unsigned char romid[8];
	unsigned char mac_code[20];
	unsigned char input_data[64];
	unsigned char s_secrect[8];
	unsigned char e_secrect1[8];
	unsigned char e_secrect2[8];
	unsigned char e_secrect3[8];
	unsigned char eeprom[EEPROM_PAGE][EEPROM_PAGE_SIZE];
};

/*=================全局变量================*/
Secrect mysecrect;
struct secrect_device_context ctx;
/*=========================================*/
/*
 * 读设备的ROMID
 * */
int test_read_romid()
{
	int i = 0;
	int rc;
	rc = mysecrect.read_rom_id(ctx.romid, sizeof(ctx.romid));
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n",__LINE__, __func__);
	}
	for (i = 0; i < 8; i++) {
		printf("%02x ", ctx.romid[i]);
	}
	printf("\n");
	return rc; 
}

int test_read_mac_code(void)
{
	int i = 0;
	int rc;
	printf("\n======>secrect read mac code test:\n");
	rc = mysecrect.read_mac_code(ctx.mac_code, sizeof(ctx.mac_code));
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n",__LINE__, __func__);
	}
	for (i = 0; i < 20; i++) {
		if (i%8 == 0)
			printf("\n");
		fprintf(stdout, "%02x ", ctx.mac_code[i]);
	}
	printf("\n");
	return rc; 
}

/*
 *读eeprom数据
 *
 * */
int test_read_eeprom(int page, int len)
{
	int i = 0;
	int rc;
	unsigned char eeprom[128];
	memset(eeprom, 0x00, 128);
	rc = mysecrect.read_eeprom(0x80 + 8*page, eeprom, len);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n",__LINE__, __func__);
		return -1;
	}
	for (i = 0; i < len; i++) {
		fprintf(stdout, "%02x ", eeprom[i]);
	}
	printf("\n");
	return rc; 
}

/*
 *
 *写eeprom数据
 * 0 <= page <=10  8 bytes per page
 * 
 * */
int test_write_eeprom(int page, unsigned char *data, int data_len)
{
	int i = 0;
	int rc;
	rc = mysecrect.write_eeprom(0x80 + page*8, data, data_len);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n",__LINE__, __func__);
		return -1;
	}

	for (i = 0; i < data_len; i++) {
		if (i%8 == 0 && i != 0)
			printf("\n");
		fprintf(stdout, "%02x ", data[i]);
	}
	printf("\n");
	return rc; 
}

/*
 *写用于计算mac码的原始数据
 * */
int test_write_input_data(unsigned char *data, int len)
{
	int i = 0;
	int rc;
	rc = mysecrect.write_input_data(data,len);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n",__LINE__, __func__);
		return -1;
	}

	printf("write input data to device as below:\n");
	for (i = 0; i < len; i++) {
		if (i%8 == 0 && i != 0)
			printf("\n");
		fprintf(stdout, "%02x ", data[i]);
	}
	printf("\n");
	return rc; 
}

int test_read_input_data(void)
{
	int i = 0;
	int rc;
	unsigned char input[47];
	memset(input, 0x00, 47);
	rc = mysecrect.read_input_data(input, 47);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n",__LINE__, __func__);
		return -1;
	}

	printf("read input data from device as below:\n");
	for (i = 0; i < 47; i++) {
		if (i%8 == 0 && i != 0)
			printf("\n");
		fprintf(stdout, "%02x ", input[i]);
	}
	printf("\n");
	return rc; 
}


struct option options[] = {
	{"read", required_argument, NULL, 'r'},
	{"write", required_argument, NULL, 'w'},
	{"page", required_argument, NULL, 'p'},
	{"length", required_argument, NULL, 'l'},
	{0, 0, 0, 0},
};

int analyse_read_eeprom(int begin, int end, char **av)
{
	int i;
	for (i = begin; i < end; i++) {
		printf("args is %d:%s\n", i, av[i]);
	}
	return 0;
}

struct cmdline {
	unsigned char data[255];
};

int main(int argc, char *argv[])
{

	int c;
	int i,j;
	int cmd_page = 0xff;
	
	unsigned char argc_count = 0xff;

	unsigned char w_type = 0xff;
	unsigned char w_pos = 0xff;
	unsigned char r_pos = 0xff;
	unsigned char r_type = 0xff;
	unsigned char cmd_data[255];
	int cmd_len = 0xff;
	
	memset(&ctx, 0x00, sizeof(ctx));
	while((c = getopt_long(argc, argv, ":r:w:p:l:", options, NULL)) != -1){
		switch (c){
		case 'w':
			if (strcmp(optarg, "eeprom") == 0) {
				w_type = 1;
			} else if (strcmp(optarg, "mac") == 0){
				w_type = 2;

			} else {
				printf("error type\n");
			}
			w_pos = optind;
			break;

		case 'r':
			if (strcmp(optarg, "romid") == 0) {
				r_type = 1;
			} else if (strcmp(optarg, "mac") == 0) {
				r_type = 2;
			} else if (strcmp(optarg, "eeprom") == 0) {
				r_type = 3;
			} else if (strcmp(optarg, "input") == 0) {
				r_type = 4;
			}
			r_pos = optind;
			break;

		case 'p':
			cmd_page = strtol(optarg, NULL, 0);
			break;

		case 'l':
			cmd_len = strtol(optarg, NULL, 0);
			break;

		default:
			fprintf(stderr, "input error\n");
			break;
		}
	}

	for (argc_count = 0; argc_count < argc; argc_count++) {
		if (w_pos < r_pos) {
			switch (w_type) {
			case 1:
				if (cmd_len  != (argc - optind)) {
					fprintf(stderr, "please input lenght not in range\n");
					return 1;
				}

				for (i = optind, j=0; i < argc; i++, j++) {
					cmd_data[j] = strtol(argv[i], NULL, 0); 
				}

				if (cmd_page > 10 || cmd_page < 0) {
					fprintf(stderr, "please input page number:[-p 0 or -p 1 ... or -p 10]\n");
					return 1;
				}
				test_write_eeprom(cmd_page, cmd_data, cmd_len);
				break;
			case 2:
				if (cmd_len  != (argc - optind)) {
					fprintf(stderr, "please input lenght not in range\n");
					return 1;
				}

				for (i = optind, j=0; i < argc; i++, j++) {
					cmd_data[j] = strtol(argv[i], NULL, 0); 
				}
				test_write_input_data(cmd_data, cmd_len);

				break;
			case 3:
				break;
			default:
				break;
			} 
			w_pos = 0xff;
		} else if (w_pos > r_pos) {
			switch (r_type) {
			case 1:
				test_read_romid();
				break;
			case 2:
				test_read_mac_code();
				break;
			case 3:
				if (cmd_page > 10 || cmd_page < 0) {
					fprintf(stderr, "please input page number:[-p 0 or -p 1 ... or -p 10]\n");
					return 1;
				}
				test_read_eeprom(cmd_page, cmd_len);
				break;
			case 4:
				test_read_input_data();
				break;
			default:
				break;
			}
		r_pos = 0xff;
		} else {
			;
		}
	}
	return 1;
}
