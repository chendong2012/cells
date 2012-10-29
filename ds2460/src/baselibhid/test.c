#include <hid.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> /* for getopt() */

int read_romid(void)
{
	int ret; 
	int i; 
	unsigned char cmd[8]; 
	unsigned char retbuf[64]; 

	memset(retbuf, 0x00, 64); 
	memset(cmd, 0x00, 8); 
	cmd[6] = 0x01; 
	cmd[7] = 0xbb; 

	ret = send_device_command(cmd);
	printf("================11111111111\n");
	ret = get_device_package(retbuf);
	if (ret) {
		fprintf(stderr, "read_a_packet error\n");
		return 1;
	}

	printf(" ==  ==  == read romid ==  ==  ==\n"); 
	for (i = 0;  i < 64;  i++) {
		printf("0x%02x ", retbuf[i]); 
	}
	printf("\n==  == end  ==  == \n"); 
	return 0; 
}

int write_input_info(void) 
{
	int ret; 
	int i; 
	unsigned char cmd[8]; 
	unsigned char sendbuf[64]; 
	unsigned char retbuf[64]; 

	memset(sendbuf, 0x00, 64); 
	memset(retbuf, 0x00, 64); 
	memset(cmd, 0x00, 8); 
	cmd[6] = 0x01; 
	cmd[7] = 0xbd; 
	sendbuf[0] = 0x12; 

	ret = send_device_command(cmd);
	ret = put_device_package(sendbuf);
	ret = get_device_package(retbuf);
	if (ret) {
		fprintf(stderr, "read_a_packet error\n");
		return 1;
	}

	printf(" ==  ==  == input info status==  ==  ==\n"); 
	for (i = 0;  i < 64;  i++) {
		printf("0x%02x ", retbuf[i]); 
	}
	printf("\n==  == end  ==  == \n"); 
	return 0; 
}

int read_input_info()
{
	int ret; 
	int i; 
	unsigned char cmd[8]; 
	unsigned char retbuf[64]; 

	memset(retbuf, 0x00, 64); 
	memset(cmd, 0x00, 8); 
	cmd[6] = 0x03; 
	cmd[7] = 0xbb; 

	ret = send_device_command(cmd);
	ret = get_device_package(retbuf);
	if (ret) {
		fprintf(stderr, "read_a_packet error\n");
		return 1;
	}

	printf(" ==  ==  == read input info==  ==  ==\n"); 
	for (i = 0;  i < 64;  i++) {
		printf("0x%02x ", retbuf[i]); 
	}
	printf("\n==  == end  ==  == \n"); 
	return 0; 
}
int main(int argc, char *argv[])
{
	int ret; 
	int i; 
	ret = open_usb_device(); 
	if (ret) {
		fprintf(stderr, "open_usb_device error\n");
		return 1;
	}
	//read_romid(); 
	write_input_info(); 
	read_input_info(); 
	ret = close_usb_device(); 
	if (ret) {
		fprintf(stderr, "read_a_packet error\n");
		return 1;
	}
	return 0;
}
