#ifndef __ZK_MEM__
#define __ZK_MEM__
#include "public.h"
#include "zkcvt.h"

class zkmem : public zkcvt {
public:
	struct _rgb_8pixels rgb_8x16[16]; //3x16=48 bytes
	unsigned char raw_8x1[16];
};
#endif
