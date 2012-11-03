#ifndef _GRALLOC_H
#define _GRALLOC_H

#include "screenFormat.h"

int initGralloc(void);
void closeGralloc(void);
unsigned char *readBufferGralloc(void);
screenFormat getScreenFormatGralloc(void);

#endif
