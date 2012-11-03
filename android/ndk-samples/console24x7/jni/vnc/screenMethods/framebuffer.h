#ifndef ADB_FRAMEBUFFER_METHOD
#define ADB_FRAMEBUFFER_METHOD

#include "common.h"

int initFB(void);
void closeFB(void);
unsigned int *readBufferFB(void);
void FB_setDevice(char *);
struct fb_var_screeninfo FB_getscrinfo(void);

#endif
