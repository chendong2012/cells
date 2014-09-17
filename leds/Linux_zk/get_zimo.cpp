#include <iostream>
#include "word_stock.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;
void help(char **argv) {
	printf("usage:\n \
	%s a ascii\n \
	%s h qh wh\n", argv[0], argv[0]);
}

int main(int argc, char *argv[]) {
	unsigned char buf_r[32];
	if((argc < 3) || (argc > 4)) {
		help(argv);
		return -1;
	} else if(argc == 3) {
		if(argv[1][0] != 'a') {
			help(argv);
			return -1;
		}
		printf("%c:\n", argv[2][0]);
		GetChar(argv[2][0], buf_r);

	} else if(argc == 4) {
		if(argv[1][0] != 'h') {
			help(argv);
			return -1;
		}
		GetCode(atoi(argv[2]), atoi(argv[3]), buf_r);

	}
	
	return 0;
}
