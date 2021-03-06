#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <syslog.h>
#include "Secrect.h"

#include <getopt.h>
#include <unistd.h>

#include "sha.h"
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

/*descript:[stanard sha1]
 * shabuf:input raw data include 64 bytes
 * secrectbuf: secrect datas[8 bytes]
 * mac_a to mac_e is output mac code together is 20 bytes
 * */
void sha1_compute_mac(unsigned char *shabuf, unsigned char *secrectbuf,
			 long *mac_a,
			 long *mac_b,
			 long *mac_c,
			 long *mac_d,
			 long *mac_e)
{
	unsigned char MT[64];
	int cnt = 0;
	long A, B, C, D, E;
	 A = *mac_a;
	 B = *mac_b;
	 C = *mac_c;
	 D = *mac_d;
	 E = *mac_e;

	for(cnt=0; cnt<55; cnt++)  //assume system secrets is 47-byte 0x55, you should replace
	{				  ////them with your own definition
		MT[cnt] = shabuf[cnt];
	}

	for (cnt = 0; cnt < 4; cnt++)
		MT[cnt] = secrectbuf[cnt];

	for (cnt = 0; cnt < 4; cnt ++)
		MT[48+cnt] = secrectbuf[cnt + 4];

	MT[55] = 0x80;
	for(cnt=56; cnt<62;cnt++)
	{
		MT[cnt] = 0x0;
	}

	MT[62]=0x01;
	MT[63]=0xB8;
	ComputeSHAEE(MT, &A, &B, &C, &D, &E);

	 *mac_a = A;
	 *mac_b = B;
	 *mac_c = C;
	 *mac_d = D;
	 *mac_e = E;
}

/*
 *inputdata[0~54-55~63]:64 bytes: input raw random datas join to compute and output mac code
 *notes:inputdata[55~63]:instead of 0x80 0x00 0x00 0x00 0x00 0x00 0x00 0x01 0x8b
 *secrectbuf[0~7]:8 bytes:  input secrect join to compute and output mac code
 *output: mac[0~19].
 *return:0 success
 * */
int test_sw_sha1_compute(unsigned char *inputdata, unsigned char *secrectbuf)
{
	unsigned char mac[20];
	int i;
	memset(mac, 0x00, 20);
	get_shamac(inputdata, secrectbuf, mac);
	for (i = 0; i < 20; i++) {
		fprintf(stdout, "0x%02x ", mac[i]);
	}
	fprintf(stdout, "\n");
	return 0;
}

/*
 *read device romid
 *return -1 fail
 *return 0 success
 * */
int test_read_romid()
{
	int i = 0;
	int rc;
	unsigned char romid[8];
	memset(romid, 0x00, 8);

	rc = mysecrect.read_rom_id(romid, sizeof(romid));
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n", __LINE__, __func__);
	}
	for (i = 0; i < 8; i++) {
		printf("%02x ", romid[i]);
	}
	printf("\n");
	return rc; 
}

/*
 *read device mac code [20 bytes]
 *return -1 fail
 *On success, the number of bytes read is returned
 * */
int test_read_mac_code(int cmd_len)
{
	int i = 0;
	int rc;
	unsigned char mac_code[20];
	memset(mac_code, 0x00, 20);

	rc = mysecrect.read_mac_code(mac_code, sizeof(mac_code));
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n", __LINE__, __func__);
	}

	for (i = 0; i < 20; i++) {
		fprintf(stdout, "%02x ", mac_code[i]);
	}
	printf("\n");
	return rc; 
}

/*
 *read eeprom data
 *page range: [0,1,2,3...13]
 *return -1 fail
 *On success, the number of bytes read is returned
 * */
int test_read_eeprom(int page, int len)
{
	int i = 0;
	int rc;
	unsigned char eeprom[128];
	memset(eeprom, 0x00, 128);
	rc = mysecrect.read_eeprom(0x80 + 8*page, eeprom, len);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n", __LINE__, __func__);
		return -1;
	}
	for (i = 0; i < len; i++) {
		fprintf(stdout, "%02x ", eeprom[i]);
	}
	printf("\n");
	return rc; 
}

/*
 *write eeprom data
 *one page size is 8 bytes
 *page range: [0,1,2,3...13]
 *return -1 fail
 *On success, the number of bytes read is returned
 * */
int test_write_eeprom(int page, unsigned char *data, int data_len)
{
	int i = 0;
	int rc;
	rc = mysecrect.write_eeprom(0x80 + page*8, data, data_len);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n", __LINE__, __func__);
		return -1;
	}

	for (i = 0; i < data_len; i++) {
		fprintf(stdout, "%02x ", data[i]);
	}
	printf("\n");
	return rc; 
}

/*
 *write s secrect len is 8 bytes
 *return -1 fail
 *On success, the number of bytes read is returned
 * */
int test_write_s_secrect(unsigned char *secrect, int secrect_len)
{
	int i;
	int rc;
	rc = mysecrect.write_s_secrect(secrect, secrect_len);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n", __LINE__, __func__);
		return -1;
	}
	for (i = 0; i < secrect_len; i++) {
		fprintf(stdout, "%02x ", secrect[i]);
	}
	printf("\n");
	return rc;
}

/*
 *write e esecrect1 len is 8 bytes
 *return -1 fail
 *On success, the number of bytes read is returned
 * */
int test_write_e_secrect1(unsigned char *secrect, int secrect_len)
{
	int i;
	int rc;
	rc = mysecrect.write_e_secrect1(secrect, secrect_len);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n",__LINE__, __func__);
		return -1;
	}
	for (i = 0; i < secrect_len; i++) {
		fprintf(stdout, "%02x ", secrect[i]);
	}
	printf("\n");
	return rc;
}

/*
 *write e esecrect2 len is 8 bytes
 *return -1 fail
 *On success, the number of bytes read is returned
 * */
int test_write_e_secrect2(unsigned char *secrect, int secrect_len)
{
	int i;
	int rc;
	rc = mysecrect.write_e_secrect2(secrect, secrect_len);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n",__LINE__, __func__);
		return -1;
	}
	for (i = 0; i < secrect_len; i++) {
		fprintf(stdout, "%02x ", secrect[i]);
	}
	printf("\n");
	return rc;
}

/*
 *write e esecrect3 len is 8 bytes
 *return -1 fail
 *On success, the number of bytes read is returned
 * */
int test_write_e_secrect3(unsigned char *secrect, int secrect_len)
{
	int i;
	int rc;
	rc = mysecrect.write_e_secrect3(secrect, secrect_len);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n",__LINE__, __func__);
		return -1;
	}
	for (i = 0; i < secrect_len; i++) {
		if (i%8 == 0 && i != 0)
			printf("\n");
		fprintf(stdout, "%02x ", secrect[i]);
	}
	printf("\n");
	return rc;
}

/*
 *write input raw datas for creating a mac code
 *return -1 fail
 *return 1 success
 * */
int test_write_input_data(unsigned char *data, int len)
{
	int i = 0;
	int rc;
	rc = mysecrect.write_input_data(data,len);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n", __LINE__, __func__);
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

/*
 *write compute command for trigger sha compute
 *return -1 fail
 *return 1 success
 *
 *compute_cmd[0]:
 *0x00:s-resect as input secrect for compute
 *0x01:e-resect1 as input secrect for compute
 *0x02:e-resect2 as input secrect for compute
 *0x03:e-resect3 as input secrect for compute
 *
 *compute_cmd[1]:
 *0x00: put first 8 bytes of result into s-secrect
 *0x01: put result into MAC Output Buffer
 *
 *compute_cmd_len fix to 2 bytes len
 * */
static int test_write_compute(unsigned char *compute_cmd, int compute_cmd_len)
{
	int rc;
	rc = mysecrect.write_compute(compute_cmd, compute_cmd_len);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n", __LINE__, __func__);
		return -1;
	}
	printf("ok\n");
	return 1;
}

/*
 *成功返回 1,错误返回-1
 *
 * trans_cmd[0]:
 *1:put s-secrect into e-secrect1
 *2:put s-secrect into e-secrect2
 *3:put s-secrect into e-secrect3
 *
 *trans_cmd_len fix to 1
 * */
static int test_write_trans(unsigned char *trans_cmd, int trans_cmd_len)
{
	int rc;
	rc = mysecrect.write_trans(trans_cmd, trans_cmd_len);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n", __LINE__, __func__);
		return -1;
	}
	return 1;
}
/*
 *read input raw data
 *return -1 fail
 *return 1 success
 * */
int test_read_inputdata(int len)
{
	int i = 0;
	int rc;
	unsigned char input[64];
	memset(input, 0x00, 64);
	rc = mysecrect.read_input_data(input, len);
	if (rc < 0) {
		fprintf(stderr, "%d:%s error\n", __LINE__, __func__);
		return -1;
	}

	printf("read input data from device as below:\n");
	for (i = 0; i < len; i++) {
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
	{"help", no_argument, NULL, 'h'},
	{0, 0, 0, 0},
};

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
	
	if (argc == 1) {
		printf("usage: \n"
			"	uway [-w types] [-r types] [-l size] [-p page] [datas]\n");
		printf("types:\n"
			"	romid\n"
			"	eeprom\n"
			"	mac\n"
			"	ssecrect\n"
			"	esecrect1\n"
			"	esecrect2\n"
			"	esecrect3\n"
			"	compute\n"
			"	sw_compute\n"
			"	inputdata\n");
		printf("size:\n"
			"	how many datas write into device or read from device\n");
		printf("page:\n"
			"	eeprom page number\n");
		printf("datas:\n"
			"	datas for writing into device\n");
		return 0;
	}
	if (mysecrect.is_device_online()== 0) {
		fprintf(stderr, "\ndevice not found!!!\n\n");
		return 1;
	}

	memset(&ctx, 0x00, sizeof(ctx));
	while((c = getopt_long(argc, argv, ":r:w:p:l:-h", options, NULL)) != -1){
		switch (c){
		case 'w':	/*list write types*/
			if (strcmp(optarg, "eeprom") == 0) {
				w_type = 1;

			} else if (strcmp(optarg, "inputdata") == 0){
				w_type = 2;

			} else if (strcmp(optarg, "ssecrect") == 0){
				w_type = 3;

			} else if (strcmp(optarg, "esecrect1") == 0){
				w_type = 4;

			} else if (strcmp(optarg, "esecrect2") == 0){
				w_type = 5;

			} else if (strcmp(optarg, "esecrect3") == 0){
				w_type = 6;

			} else if (strcmp(optarg, "compute") == 0){
				w_type = 7;

			} else if (strcmp(optarg, "sw_compute") == 0){
				w_type = 8;

			} else {
				printf("error type\n");
			}
			w_pos = optind;
			break;

		case 'r':	/*list read types*/
			if (strcmp(optarg, "romid") == 0) {
				r_type = 1;
			} else if (strcmp(optarg, "mac") == 0) {
				r_type = 2;
			} else if (strcmp(optarg, "eeprom") == 0) {
				r_type = 3;
			} else if (strcmp(optarg, "inputdata") == 0) {
				r_type = 4;
			}
			r_pos = optind;
			break;

		case 'p':	/*page for eeprom data area*/
			cmd_page = strtol(optarg, NULL, 0);
			break;

		case 'l':	/*write into device data lenght*/
			cmd_len = strtol(optarg, NULL, 0);
			break;
		case 'h':
			printf("usage: \n"
			"	uway [-w types] [-r types] [-l size] [-p page] [datas]\n");
			printf("types:\n"
				"	romid\n"
				"	eeprom\n"
				"	mac\n"
				"	ssecrect\n"
				"	esecrect1\n"
				"	esecrect2\n"
				"	esecrect3\n"
				"	compute\n"
				"	sw_compute\n"
				"	inputdata\n");
			printf("size:\n"
				"	write how many datas into device\n");
			printf("page:\n"
				"	eeprom page number\n");
			printf("datas:\n"
				"	datas for writing into device\n");
			return 0;

		default:	/*illeage parameters*/
			fprintf(stderr, "input error\n");
			return 1;
		}
	}

	/*run the commands on the command line in order*/
	for (argc_count = 0; argc_count < argc; argc_count++) {
		if (w_pos < r_pos) {

				if (cmd_len  != (argc - optind)) {
					fprintf(stderr, "please input lenght not in range\n");
					return 1;
				}
			switch (w_type) {
			case 1:
				for (i = optind, j=0; i < argc; i++, j++) {
					cmd_data[j] = strtol(argv[i], NULL, 0); 
				}

				if (cmd_page > 13 || cmd_page < 0) {
					fprintf(stderr, "please input page number:[-p 0 or -p 1 ... or -p 10]\n");
					return 1;
				}
				test_write_eeprom(cmd_page, cmd_data, cmd_len);
				break;

			case 2:
				for (i = optind, j=0; i < argc; i++, j++) {
					cmd_data[j] = strtol(argv[i], NULL, 0); 
				}
				test_write_input_data(cmd_data, cmd_len);
				break;
			case 3:
				for (i = optind, j=0; i < argc; i++, j++) {
					cmd_data[j] = strtol(argv[i], NULL, 0); 
				}
				test_write_s_secrect(cmd_data, cmd_len);
				break;
			case 4:
				for (i = optind, j=0; i < argc; i++, j++) {
					cmd_data[j] = strtol(argv[i], NULL, 0); 
				}
				test_write_e_secrect1(cmd_data, cmd_len);
				break;
			case 5:
				for (i = optind, j=0; i < argc; i++, j++) {
					cmd_data[j] = strtol(argv[i], NULL, 0); 
				}
				test_write_e_secrect2(cmd_data, cmd_len);
				break;
			case 6:
				for (i = optind, j=0; i < argc; i++, j++) {
					cmd_data[j] = strtol(argv[i], NULL, 0); 
				}
				test_write_e_secrect3(cmd_data, cmd_len);
				break;
			case 7:
				for (i = optind, j=0; i < argc; i++, j++) {
					cmd_data[j] = strtol(argv[i], NULL, 0); 
				}
				test_write_compute(cmd_data, cmd_len);
				break;
			case 8:
				for (i = optind, j=0; i < argc; i++, j++) {
					cmd_data[j] = strtol(argv[i], NULL, 0); 
				}
				test_sw_sha1_compute(&cmd_data[0], &cmd_data[64]);
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
				test_read_mac_code(cmd_len);
				break;
			case 3:
				if (cmd_page > 13 || cmd_page < 0) {
					fprintf(stderr, "please input page number:[-p 0 or -p 1 ... or -p 10]\n");
					return 1;
				}
				test_read_eeprom(cmd_page, cmd_len);
				break;
			case 4:
				if (cmd_len > 64) {
					fprintf(stderr, "input cmd_len exceeded range[0 < len <=64]\n");
					return 1;
				}
				test_read_inputdata(cmd_len);
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
