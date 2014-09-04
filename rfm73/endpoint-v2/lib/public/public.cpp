#include "public.h"
void dispinfo(const char *dat)
{
	sprintf(g_debug, "%s", dat);
	Serial.println((const char *)g_debug);
}
