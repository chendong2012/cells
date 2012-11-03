#ifndef GRALLOC_H
#define GRALLOC_H

#include "screenFormat.h"

int init_gralloc();
unsigned char *readfb_gralloc();
void close_gralloc();
screenFormat getscreenformat_gralloc();

#endif
