#ifndef FLINGER_H
#define FLINGER_H

#include "screenFormat.h"

int initFlinger(void);
void closeFlinger(void);
unsigned char *readBufferFlinger(void);
screenFormat getScreenFormatFlinger(void);

#endif
