/*
 * word_stock.cpp
 *
 *  Created on: Oct 20, 2012
 *      Author: limosky
 */

#include <stdio.h>
#include "word_stock.h"

using namespace std;

void GetCode(int qh, int wh, unsigned char *outbuf) {
	//initialize buf[] with a laughing face,that means function return error.
	unsigned char buf[32] = { 0x00, 0x00, 0x7F, 0xFC, 0x44, 0x84, 0x46, 0x44,
		0x44, 0x24, 0x48, 0x34, 0x50, 0x14, 0x6F, 0xE4, 0x48, 0x24, 0x48,
		0x24, 0x48, 0x24, 0x48, 0x24, 0x48, 0x24, 0x48, 0x24, 0x7F, 0xFC,
		0x00, 0x00 };
	FILE *HZK;
	int i, j, k;
	int offset = (94 * (qh - 1) + (wh - 1)) * 32;
	//fixme:At last,you should open this file at the beginning of program,so
	//to avoid open and close file too often.
	if ((HZK = fopen("HZK16", "rb")) == NULL) {
		printf("Can't Open HZK16\n");
		for (i = 0; i < 32; i++) {
			*(outbuf + i) = buf[i];
		}
	}

	fseek(HZK, offset, SEEK_SET);
	fread(outbuf, 32, 1, HZK);
	for(i = 0; i < 32; i++){
		printf("0x%02x, ", outbuf[i]);
	}
	printf("\n");
	for(i=0; i<16; i++) 
	{	
		for(j=0; j<2; j++) 
		{ 
			for(k=0; k<8; k++) 
			{ 
				if(*(outbuf + i * 2 + j) & (0x80>>k))  
				{//测试为1的位则显示 
					printf("%c",'#'); 
				} 
				else 
				{ 
					printf("%c",'-'); 
				}	
			}	
		} 
		printf("\n"); 
	} 
	fclose(HZK);
}

void GetChar(char c, unsigned char *outbuf) {
	unsigned char buf[16] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	FILE *ascii_zk;
	int i, k;
	//fixme:At last,you should open this file at the beginning of program,so
	//to avoid open and close file too often.
	if ((ascii_zk = fopen("ascii_zk", "rb")) == NULL) {
		printf("Cant open ascii_zk\n");
		for (i = 0; i < 16; i++) {
			*(outbuf + i) = buf[i];
			cout << *(outbuf + i) << endl;
		}
	}

	int offset = (c - 0x20) * 16;
	fseek(ascii_zk, offset, SEEK_SET);
	fread(outbuf, 16, 1, ascii_zk);
	for(i = 0; i < 16; i++){
		printf("0x%02x, ", outbuf[i]);
	}
	printf("\n");
	for(i=0; i<16; i++) 
	{	
		for(k=0; k<8; k++) 
		{ 
			if(outbuf[i] & (0x80>>k))  
			{//测试为1的位则显示 
				printf("%c",'#'); 
			} 
			else 
			{ 
				printf("%c",'-'); 
			}	
		}	
		printf("\n"); 
	} 
	fclose(ascii_zk);
}
