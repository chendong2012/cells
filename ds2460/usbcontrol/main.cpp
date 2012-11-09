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
int test_read_romid(void)
{
	int i = 0;
	int rc;
	printf("\n======>secrect rom id test:\n");
	rc = mysecrect.read_rom_id(ctx.romid, sizeof(ctx.romid));
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n",__LINE__, __func__);
	}
	for (i = 0; i < 8; i++) {
		printf("%d:0x%02x ", i, ctx.romid[i]);
	}
	printf("\n");
	return rc; 
}

/*
 *读设备计算出来的mac认证码
 * */
int test_read_mac_code(void)
{
	int i = 0;
	int rc;
	printf("\n======>secrect read mac id test:\n");
	rc = mysecrect.read_mac_code(ctx.mac_code, sizeof(ctx.mac_code));
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
int test_read_eeprom(void)
{
	int i = 0;
	int rc;
	unsigned char eeprom[128];
	memset(eeprom, 0x00, 128);
	rc = mysecrect.read_eeprom(0x80, eeprom, 16);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n",__LINE__, __func__);
		return -1;
	}
	for (i = 0; i < 16; i++) {
		fprintf(stdout, "%d:0x%02x ", i, eeprom[i]);
	}
	printf("\n");
	return rc; 
}

/*
 *写eeprom数据
 *
 * */
int test_write_eeprom(void)
{
	int i = 0;
	int rc;
	unsigned char eeprom[128];
	memset(eeprom, 0x55, 128);
	rc = mysecrect.write_eeprom(0x80, eeprom, 8);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n",__LINE__, __func__);
		return -1;
	}

	for (i = 0; i < 8; i++) {
		if (i%8 == 0 && i != 0)
			printf("\n");
		fprintf(stdout, "%d:0x%02x ", i, eeprom[i]);
	}
	printf("\n");
	return rc; 
}

/*
 *写用于计算mac码的原始数据
 * */
int test_write_input_data(void)
{
	int i = 0;
	int rc;
	unsigned char input[47];
	memset(input, 0x2, 47);
	rc = mysecrect.write_input_data(input,47);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n",__LINE__, __func__);
		return -1;
	}

	printf("write input data to device as below:\n");
	for (i = 0; i < 47; i++) {
		if (i%8 == 0 && i != 0)
			printf("\n");
		fprintf(stdout, "%02x ", input[i]);
	}
	printf("\n");
	return rc; 
}

/*
 *读用于计算mac认证码的原始数据
 *
 * */
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
	{"type", required_argument, NULL, 't'},
	{"read", required_argument, NULL, 'r'},
	{0, 0, 0, 0},
};

int main(int argc, char *argv[])
{

	int c;
	char *l_opt_arg;
	memset(&ctx, 0x00, sizeof(ctx));
	while((c = getopt_long(argc, argv, ":r:t:", options, NULL)) != -1){
		switch (c){
			case 't':
				l_opt_arg = optarg;
				printf("arg is %s\n", optarg);
				test_read_eeprom();
				printf("\n");
				break;
			case 'r':
				l_opt_arg = optarg;
				printf("arg is %s\n", optarg);
				test_read_eeprom();
				printf("\n");
				break;
			default:
				printf("input error\n");
				break;
		}
	}


//	test_read_romid();
//	test_read_mac_code();
	//
//	test_write_eeprom();
//	test_write_input_data();
//	test_read_input_data();

	return 1;
}
