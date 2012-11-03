#ifndef COMMON_H
#define COMMON_H

#include <android/log.h> 
#ifndef __cplusplus

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>     
 
#include <sys/stat.h>
#include <sys/sysmacros.h>             /* For makedev() */

#include <fcntl.h>
#include <linux/fb.h>
#include <linux/input.h>

#include <assert.h>
#include <errno.h>

#include "screenFormat.h"

#define L(...) do{ __android_log_print(ANDROID_LOG_INFO,"VNCserver",__VA_ARGS__);printf(__VA_ARGS__); } while (0);
#endif

struct fbinfo {
    unsigned int version;
    unsigned int bpp;
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned int red_offset;
    unsigned int red_length;
    unsigned int blue_offset;
    unsigned int blue_length;
    unsigned int green_offset;
    unsigned int green_length;
    unsigned int alpha_offset;
    unsigned int alpha_length;
} __attribute__((packed));

typedef int (*init_fn_type)(void);
typedef int (*close_fn_type)(void);
typedef unsigned char * (*readfb_fn_type)(void);
typedef screenFormat (*getscreenformat_fn_type)(void);

void rotate(int);
int getCurrentRotation();
int isIdle();
void setIdle(int i);
void close_app();
screenFormat screenformat;

#define DVNC_FILES_PATH "/data/data/com.llc24x7lab.console24x7/files/"
#define DVNC_LIB_PATH "/data/data/com.llc24x7lab.console24x7/lib/"

#define ARR_LEN(a) (sizeof(a)/sizeof(a)[0])
static int compiled_sdks[] = {10, 15, 14};

#endif
