/*
 * read_ascii.c
 *
 *  Created on: Oct 20, 2012
 *      Author: limosky
 */

#include <stdio.h> 
#include <string.h>  
#include <iconv.h>
#include <stdlib.h>

int main() 
{	
	char incode='Z'; 
	printf("%x\n",incode);
	unsigned int offset = 0;  
	char mat[16] = {0};  
	FILE *ascii_zk = 0;  
	int i,j,k;  

	offset = (incode-0x20)*16; //得到偏移位置 
	if((ascii_zk=fopen("ascii_zk", "rb")) == NULL)  
	{  
		printf("Can't Open HZK16\n");  
		getchar(); 
		return 0; 
	}  
	fseek(ascii_zk, offset, SEEK_SET);  
	fread(mat, 16, 1, ascii_zk); 
	fclose(ascii_zk); 

	//显示 
	for(i=0; i<16; i++) 
	{	
		for(k=0; k<8; k++) 
		{ 
			if(mat[i] & (0x80>>k))  
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
	getchar(); 
	return 1; 
}
